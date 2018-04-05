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
    ALenum error;                                                              \
    if ((error = alcGetError(device)) != AL_NO_ERROR) {                        \
      fprintf(stderr, "ERROR: %s: %s", msg, alcGetString(device, error));      \
      return retval;                                                           \
    }                                                                          \
  }

#define RETURN_ON_AL_ERROR(msg, retval)                                        \
  {                                                                            \
    ALenum error;                                                              \
    if ((error = alGetError()) != AL_NO_ERROR) {                               \
      fprintf(stderr, "ERROR: %s: %s", msg, alGetString(error));               \
      return retval;                                                           \
    }                                                                          \
  }

class ALDevice {
private:
  ALDevice(ALCdevice *device, ALCcontext *context)
      : device_(device), context_(context) {}

public:
  static ALDevice *open() {
    alGetError();
    ALCdevice *device = alcOpenDevice(nullptr);
    RETURN_ON_ALC_ERROR(device, "Could not open.", nullptr);
    ALCcontext *context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    RETURN_ON_ALC_ERROR(device, "Could not setup context.", nullptr);
    return new ALDevice(device, context);
  }

  ~ALDevice() {
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

ALenum toALFormat(Header *header) {
  if (header->numChannels == 2) {
    if (header->bitsPerSample == 16) {
      return AL_FORMAT_STEREO16;
    } else if (header->bitsPerSample == 8) {
      return AL_FORMAT_STEREO8;
    }
  } else if (header->numChannels == 1) {
    if (header->bitsPerSample == 16) {
      return AL_FORMAT_MONO16;
    } else if (header->bitsPerSample == 8) {
      return AL_FORMAT_MONO8;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::unique_ptr<WavFileContents> contents = readWavFile(argv[1]);
  printf("OK: Assuming that %s is a 16-bpp WAV file.\n", argv[1]);
  std::unique_ptr<ALDevice> device(ALDevice::open());
  if (device.get() == nullptr) {
    return -1;
  }
  ALuint *buffers = new ALuint;
  alGenBuffers(1, buffers);
  RETURN_ON_AL_ERROR("Could not gen buffer.", -1);

  alBufferData(buffers[0], toALFormat(&contents->header), contents->data.get(),
               contents->dataLength, contents->header.sampleRate);
  RETURN_ON_AL_ERROR("Could not buffer data.", -1);

  // Position a listener at 0,0,0.
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  RETURN_ON_AL_ERROR("Could not position listener.", -1);

  // Position a source each at a unit distance on each axis.
  ALuint *sources = new ALuint;
  alGenSources(3, sources);
  RETURN_ON_AL_ERROR("Could not gen sources.", -1);

  alSourcei(sources[0], AL_BUFFER, buffers[0]);
  alSourcei(sources[0], AL_PITCH, 1);
  alSource3f(sources[0], AL_POSITION, 1.0f, 0.0f, 0.0f);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcei(sources[1], AL_BUFFER, buffers[0]);
  alSourcei(sources[1], AL_PITCH, 2);
  alSource3f(sources[1], AL_POSITION, 0.0f, 1.0f, 0.0f);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcei(sources[2], AL_BUFFER, buffers[0]);
  alSourcei(sources[2], AL_PITCH, 3);
  alSource3f(sources[2], AL_POSITION, 0.0f, 0.0f, 1.0f);
  RETURN_ON_AL_ERROR("Could not associate buffer with sources.", -1);

  alSourcePlay(sources[0]);
  sleep(1);
  alSourcePlay(sources[1]);
  sleep(1);
  alSourcePlay(sources[2]);
  sleep(10);
  return 0;
}