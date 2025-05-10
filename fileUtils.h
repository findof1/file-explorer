#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <stdbool.h>

typedef struct
{
  char filename[256];
  bool isDirectory;
} File;

int listDirectoryContents(const char *directory, File *filesOut, int filesOutLength);
bool moveUpDirectory(char *path);

#endif