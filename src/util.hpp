#pragma once
#include "debug.hpp"
#include "unistd.h"

#ifdef IODEBUG
#define S(X) X
#else
#define S(X)
#endif

inline int writeall(int fd, const void *data, int size) {
  int current = 0;
  while (current < size) {
    int step = write(fd, (char *)data + current, size - current);
    S(LOG("-> Write command %d for %d size: %d ==> step: %d\n", iter++, current,
          size - current, step);)
    if (step < 1)
      return step;
    current += step;
  }
  S(LOG("-> Written %d/%d\n", current, size);)
  return current;
}

inline int readall(int fd, void *data, int size) {
  int current = 0;
  while (current < size) {
    int step = read(fd, (char *)data + current, size - current);
    S(LOG("[READER] -> Read command %d for %d size: %d ==> step: %d\n", iter++,
          current, size - current, step);)
    if (step < 1)
      return step;
    current += step;
  }

  S(LOG("-> [READER] Read %d/%d\n", current, size);)
  return current;
}

#undef S

#include <fstream>
#include <sstream>
#include <string>
using namespace std;
inline string readFile(const char *path) {
  ifstream ifs(path);
  ifs.open(path);
  stringstream strStream;
  strStream << ifs.rdbuf();
  LOG("-> File read %s\n", path);
  return strStream.str();
}
