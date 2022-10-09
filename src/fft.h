#pragma once

#include <complex>
#include <vector>

#include "sample_type.h"
#include "AudioBuffer.h"


std::vector<std::complex<double>> fft(const std::vector<double> x);
static std::vector<std::complex<double>> FFTRadix2(const std::vector<std::complex<double>>& x, const std::vector<std::complex<double>>& W);
bool isPowerOfTwo(int input); 
static size_t ReverseBits(const size_t x, const size_t n);