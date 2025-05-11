#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <stdbool.h>

typedef struct
{
  char filename[256];
  bool isDirectory;
} File;

int listDirectoryContents(const char *directory, File *filesOut, const int filesOutLength, const int start);
bool moveUpDirectory(char *path);
int getDirectoryFileAmount(const char *directory);
void moveIntoDirectory(char *currentDirectory, const char *directoryName);

#endif