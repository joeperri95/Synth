#include "Volume.h"

namespace audio { namespace effects {

Volume::Volume(std::shared_ptr<float> volume) : _volume(volume) {}
Volume::~Volume() {}

AudioBuffer<sample_type> Volume::operator()(AudioBuffer<sample_type> input)
{    
    AudioFormat format = input.getFormat();
    std::vector<sample_type> inData = input.getData();
    std::vector<sample_type> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++)
    {
        for(int j = 0; j < format.channels; j++)
        {
            ret.at(i * format.channels + j) = *this->_volume * inData.at(i * format.channels + j);
        }
    }

    return AudioBuffer(format, ret);
}

}}