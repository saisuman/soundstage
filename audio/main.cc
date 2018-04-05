#include "wavfile.h"
#include <fcntl.h>
#include <pulse/error.h>
#include <pulse/simple.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {
  std::unique_ptr<WavFileContents> contents = readWavFile("horndive.wav");
  pa_sample_spec spec;
  spec.format = PA_SAMPLE_S16LE;
  spec.rate = contents->header.sampleRate;
  spec.channels = contents->header.numChannels;
  int error;
  pa_simple *handle =
      pa_simple_new(NULL,       // Use the default server.
                    "SuperSub", // Our application's name.
                    PA_STREAM_PLAYBACK,
                    NULL,           // Use the default device.
                    "AudioSamples", // Description of our stream.
                    &spec,          // Our sample format.
                    NULL,           // Use default channel map
                    NULL, &error);
  if (handle == NULL) {
    printf("ERROR: Couldn't open: %s\n", pa_strerror(error));
    return -1;
  }
  int sampleSize =
      contents->header.numChannels * contents->header.bitsPerSample / 8;
  for (int i = 0; i + sampleSize < contents->dataLength; ++i) {
    char *sampleStart = contents->data.get() + i * sampleSize;
    if (pa_simple_write(handle, sampleStart, sampleSize, &error)) {
      printf("ERROR: Couldn't write: %s\n", pa_strerror(error));
      return -1;
    } else {
    }
  }
  return 0;
}