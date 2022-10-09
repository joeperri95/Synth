#include "wav.h"

bool isValidWAVFile(std::ifstream &fs)
{
    char _RIFF[5];
    char _FILE_SIZE[5];
    char _WAVE[5];
    bool ret = true;

    fs.get(_RIFF, 5);
    fs.get(_FILE_SIZE, 5); //total file size including header. Not used to validate file
    fs.get(_WAVE, 5);

    //WAV file must start with 'RIFF'
    if (std::string("RIFF").compare(_RIFF) != 0)
    {
        std::cerr << "not a valid wave file" << std::endl;
        ret = false;
    }

    //start of header must be 'WAVE'
    if (std::string("WAVE").compare(_WAVE) != 0)
    {
        std::cerr << "not a valid wave file" << std::endl;
        ret = false;
    }

    fs.close();

    return ret;
}

wavData getWAVData(std::ifstream &fs)
{
    wavData metaData;

    //read header data
    fs.get(metaData._fmt, 5);
    fs.get(metaData._fmtLen, 5);
    fs.get(metaData._fmtType, 3);
    fs.get(metaData._channels, 3);
    fs.get(metaData._sampleRate, 5);
    fs.get(metaData._numBytes, 5);
    fs.get(metaData._audiofmt, 3);
    fs.get(metaData._bitspersample, 3);

    return metaData;
}

void processWavFile(std::string filename)
{
    std::ifstream fs;

    std::cout << "starting to read " << filename << std::endl;

    isValidWAVFile(fs);
    wavData metaData = getWAVData(fs);

    char _data[5];
    char _fileSize[5];

    fs.get(_data, 5);
    fs.get(_fileSize, 5);

    //look for 'data' section there may be other sections we don't care about
    while (std::string("data").compare(_data) != 0)
    {
        fs.ignore(*(int *)_fileSize);
        fs.get(_data, 5);
        fs.get(_fileSize, 5);
    }

    int length = *(int *)_fileSize + 1;
    std::vector<short> buffer(512);
    buffer.resize(length); //already takes into account size

    char temp[2];

    //only handle the case the data is a 16 bit int for now
    //TODO implement other data types
    std::cout << length << std::endl;

    for (int i = 0; i < (length >> 1); i++)
    {
        fs.read(temp, 2);
        buffer[i] = *(short *)temp;
        //std::cout << this->buffer[i] << ' ' << temp << std::endl;
    }

    std::cout << "done" << std::endl;
    fs.close();
}

