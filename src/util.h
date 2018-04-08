#include <sys/types.h>
#include <string>

class Buffer {
private:
  Buffer(char *contents, size_t length)
      : contents_(contents), length_(length) {}

public:
  static Buffer *readFromFile(const std::string &filename);
  const char *getContent() { return contents_; }
  const size_t getContentLength() { return length_; }
  ~Buffer() {
      delete contents_;
  }

private:
  char *contents_;
  size_t length_;
};