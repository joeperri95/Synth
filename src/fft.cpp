#include "fft.h"

std::vector<std::complex<double>> fft(const std::vector<double> x) {

    int N = x.size();


    // Taking advantage of symmetry the FFT of a real signal can be computed
    // using a single N/2-point complex FFT. Split the input signal into its
    // even and odd components and load the data into a single complex vector.
    std::vector<std::complex<double>> x_p(N / 2);
    for (size_t n = 0; n < N / 2; ++n)
    {
        // x_p[n] = x[2n] + jx[2n + 1]
        x_p[n] = std::complex<double>(x[2 * n], x[2 * n + 1]);
    }

    // Pre-calculate twiddle factors
    std::vector<std::complex<double>> W(N / 2);
    std::vector<std::complex<double>> W_p(N / 4);
    for (size_t k = 0; k < N / 2; ++k)
    {
        W[k] = std::polar(1.0, -2 * M_PI * k / N);

        // The N/2-point complex DFT uses only the even twiddle factors
        if (k % 2 == 0)
        {
            W_p[k / 2] = W[k];
        }
    }

    // Perform the N/2-point complex FFT
    std::vector<std::complex<double>> X_p = FFTRadix2(x_p, W_p);

    // Extract the N-point FFT of the real signal from the results 
    std::vector<std::complex<double>> X(N);
    X[0] = X_p[0].real() + X_p[0].imag();
    for (size_t k = 1; k < N / 2; ++k)
    {
        // Extract the FFT of the even components
        auto A = std::complex<double>(
            (X_p[k].real() + X_p[N / 2 - k].real()) / 2,
            (X_p[k].imag() - X_p[N / 2 - k].imag()) / 2);

        // Extract the FFT of the odd components
        auto B = std::complex<double>(
            (X_p[N / 2 - k].imag() + X_p[k].imag()) / 2,
            (X_p[N / 2 - k].real() - X_p[k].real()) / 2);

        // Sum the results and take advantage of symmetry
        X[k] = A + W[k] * B;
        X[k + N / 2] = A - W[k] * B;
    }

    return X;

}

static std::vector<std::complex<double>> FFTRadix2(const std::vector<std::complex<double>>& x, const std::vector<std::complex<double>>& W)
{
    size_t N = x.size();

    // Calculate how many stages the FFT must compute
    size_t stages = static_cast<size_t>(log2(N));

    // Pre-load the output vector with the input data using bit-reversed indexes
    std::vector<std::complex<double>> X(N);
    for (size_t n = 0; n < N; ++n)
    {
        X[n] = x[ReverseBits(n, stages)];
    }

    // Calculate the FFT one stage at a time and sum the results
    for (size_t stage = 1; stage <= stages; ++stage)
    {
        size_t N_stage = static_cast<size_t>(std::pow(2, stage));
        size_t W_offset = static_cast<size_t>(std::pow(2, stages - stage));
        for (size_t k = 0; k < N; k += N_stage)
        {
            for (size_t n = 0; n < N_stage / 2; ++n)
            {
                auto tmp = X[k + n];
                X[k + n] = tmp + W[n * W_offset] * X[k + n + N_stage / 2];
                X[k + n + N_stage / 2] = tmp - W[n * W_offset] * X[k + n + N_stage / 2];
            }
        }
    }

    return X;
}

bool isPowerOfTwo(int input) {
    return input && (!(input & (input - 1)));
}

// Given x composed of n bits, returns x with the bits reversed
static size_t ReverseBits(const size_t x, const size_t n)
{
    size_t xReversed = 0;
    for (size_t i = 0; i < n; ++i)
    {
        xReversed = (xReversed << 1U) | ((x >> i) & 1U);
    }

    return xReversed;
}