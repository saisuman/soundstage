#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "wavfile.h"

std::unique_ptr<WavFileContents> readWavFile(const char *filename) {
  int fd = open(filename, S_IRWXG, O_RDONLY);
  if (fd < 0) {
    perror("Could not open file.");
    return nullptr;
  }
  std::unique_ptr<WavFileContents> contents(new WavFileContents);
  if (read(fd, &contents->header, sizeof(Header)) != sizeof(Header)) {
    perror("Didn't read the headers.");
    return nullptr;
  }
  printf("ChunkId: %.4s\nChunkSize: %d bytes\nFormat: %.4s\n",
         contents->header.chunkId, contents->header.chunkSize,
         contents->header.format);
  printf("SubChunk1Id: %.4s SubChunk1Size: %d\n", contents->header.subChunk1.id,
         contents->header.subChunk1.size);
  printf("AudoFormat: %d NumChannels: %d SampleRate: %d\n",
         contents->header.audioFormat, contents->header.numChannels,
         contents->header.sampleRate);
  if (contents->header.audioFormat != 1) {
    printf("ERROR: This is compressed audio and is unsupported.\n");
    return nullptr;
  }
  int bytesRead;
  SubChunk nextChunk;
  int bytesToRead = -1;
  while ((bytesRead =
              read(fd, &nextChunk, sizeof(SubChunk)) == sizeof(SubChunk))) {
    if (strncmp(nextChunk.id, "data", 4) == 0) {
      bytesToRead = nextChunk.size;
      break;
    }
    printf("Ignoring chunk of type: %.4s\n", nextChunk.id);
    printf("Skipping %d bytes.\n", nextChunk.size);
    if (lseek(fd, nextChunk.size, SEEK_CUR) == -1) {
      perror("Seek failed.");
      return nullptr;
    }
  }
  if (bytesToRead == -1) {
    printf("ERROR: Couldn't find a data chunk.\n");
    return nullptr;
  }

  printf("Total data size: %d bytes\n", bytesToRead);
  char *buffer = new char[bytesToRead];
  contents->data = std::unique_ptr<char>(buffer);
  contents->dataLength = bytesToRead;
  if (read(fd, buffer, bytesToRead) != bytesToRead) {
    perror("Could not read wav file.");
    return nullptr;
  }
  for (int i = 0; i < bytesToRead; i++) {
    buffer[i] -= (20 - random()  % 20);
  }
  return contents;
}