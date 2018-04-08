#include "util.h" //static
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// static
Buffer *Buffer::readFromFile(const std::string &fileName) {
  int fd = open(fileName.c_str(), S_IRWXG, O_RDONLY);
  if (fd < 0) {
    perror("Could not open file.");
    return nullptr;
  }
  struct stat statbuf;
  if (fstat(fd, &statbuf) != 0) {
      perror("Could not stat file.");
      return nullptr;
  }
  char *buffer = new char[statbuf.st_size];
  if (buffer == nullptr) {
      perror("Could not allocate memory.");
      return nullptr;
  }
  if (read(fd, buffer, statbuf.st_size) != statbuf.st_size) {
      perror("Could not read data.");
      delete buffer;
      return nullptr;
  }
  return new Buffer(buffer, statbuf.st_size);  
}