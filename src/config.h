#include <rapidjson/document.h>
#include <string>
#include <map>
#include "wavfile.h"
#include "util.h"

class Config {
public:
  Config(const std::string &configFileName);
  std::string load();

private:
  rapidjson::Document document_;
  std::map<std::string, std::unique_ptr<WavFileContents>> sampleMap_;
};