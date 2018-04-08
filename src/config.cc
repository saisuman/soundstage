#include "config.h"
#include "wavfile.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <string>

const char kSamples[] = "samples";
const char kSampleName[] = "name";
const char kSampleFile[] = "file";

Config::Config(const std::string &configFileName) {
  std::unique_ptr<Buffer> buffer(Buffer::readFromFile(configFileName));
  std::cerr << "Full config: " << buffer->getContent() << std::endl;
  document_.Parse(buffer->getContent());
}

std::string Config::load() {
  if (!document_.HasMember(kSamples)) {
    return "No samples entry in config.";
  }
  const rapidjson::Value &samples = document_[kSamples];
  if (!samples.IsArray()) {
    return "samples entry is not an array.";
  }

  for (rapidjson::SizeType i = 0; i < samples.Size(); ++i) {
    const rapidjson::Value &sample = samples[i];
    // Each sample has a name and a file associated with it.
    const std::string &name = sample[kSampleName].GetString();
    const std::string &filename = sample[kSampleFile].GetString();
    if (name.empty() || filename.empty()) {
      return "Samples must have name and file entries.";
    }
    std::cout << "Loading " << name << " as " << filename << std::endl;
    auto waveFileContents = readWavFile(filename.c_str());
    if (waveFileContents.get() == nullptr) {
        return "Couldn't read file.";
    }
    sampleMap_.insert(std::make_pair(name, readWavFile(filename.c_str())));
  }
  return "";
}