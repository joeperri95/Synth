#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct wavData
{
    char _fmt[5];
    char _fmtLen[5];
    char _fmtType[3];
    char _channels[3];
    char _sampleRate[5];
    char _numBytes[5];
    char _audiofmt[3];
    char _bitspersample[3];

} wavData;

bool isValidWAVFile(std::ifstream &fs);
wavData getWAVData(std::ifstream &fs);
void processWavFile(std::string filename);