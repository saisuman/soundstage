#include <memory>

#ifndef __WAVFILE_H__
#define __WAVFILE_H__

struct SubChunk {
    char id[4];
    int size;
};

struct Header {
    char chunkId[4];
    int chunkSize;
    char format[4];
    SubChunk subChunk1;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct WavFileContents {
    Header header;
    int dataLength;
    std::unique_ptr<char> data;
};

std::unique_ptr<WavFileContents> readWavFile(const char* filename);

#endif /* __WAVFILE_H__ */