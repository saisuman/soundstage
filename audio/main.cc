#include "wavfile.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <fcntl.h>
#include <pulse/error.h>
#include <pulse/simple.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define RETURN_ON_ALC_ERROR(device, msg, retval)                               \
  {                                                                            \
    if ((error = alcGetError(device)) != AL_NO_ERROR) {                        \
      fprintf(stderr, "ERROR: %s: %s", msg, alcGetString(device, error));      \
      return retval;                                                           \
    }                                                                          \
  }

#define RETURN_ON_AL_ERROR(msg, retval)                                        \
  {                                                                            \
    if ((error = alGetError()) != AL_NO_ERROR) {                               \
      fprintf(stderr, "ERROR: %s: %s", msg, alGetString(error));               \
      return retval;                                                           \
    }                                                                          \
  }

class OpenAlDevice {
private:
  OpenAlDevice(ALCdevice *device, ALCcontext *context)
      : device_(device), context_(context) {}

public:
  static OpenAlDevice *open() {
    alGetError();
    ALCenum error;
    ALCdevice *device = alcOpenDevice(nullptr);
    RETURN_ON_ALC_ERROR(device, "Could not open.", nullptr);
    ALCcontext *context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    RETURN_ON_ALC_ERROR(device, "Could not setup context.", nullptr);
    return new OpenAlDevice(device, context);
  }

  ~OpenAlDevice() {
    alGetError();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context_);
    if (alcCloseDevice(device_) != AL_TRUE) {
      ALCenum error = alcGetError(device_);
      printf("ERROR: Could not close device: %s", alcGetString(device_, error));
    }
  }

private:
  ALCdevice *device_;
  ALCcontext *context_;
};

int main(int argc, char *argv[]) {
  std::unique_ptr<WavFileContents> contents = readWavFile("horndive.wav");
  ALenum error;
  std::unique_ptr<OpenAlDevice> device(OpenAlDevice::open());
  if (device.get() == nullptr) {
    return -1;
  }
  ALuint *buffers = new ALuint;
  alGenBuffers(1, buffers);
  RETURN_ON_AL_ERROR("Could not gen buffer.", -1);

  alBufferData(buffers[0], AL_FORMAT_STEREO16, contents->data.get(),
               contents->dataLength, contents->header.sampleRate);
  RETURN_ON_AL_ERROR("Could not buffer data.", -1);

  ALuint *sources = new ALuint;
  alGenSources(3, sources);
  RETURN_ON_AL_ERROR("Could not gen sources.", -1);

  alSourcei(sources[0], AL_BUFFER, buffers[0]);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcei(sources[1], AL_BUFFER, buffers[0]);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcei(sources[2], AL_BUFFER, buffers[0]);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcePlay(sources[0]);
  sleep(1);
  alSourcePlay(sources[1]);
  sleep(1);
  alSourcePlay(sources[2]);
  sleep(10);
  return 0;
}