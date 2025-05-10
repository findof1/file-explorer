#include "fileUtils.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// returns amount of files found; returns -1 when the directory does not exist
int listDirectoryContents(const char *directory, File *filesOut, const int filesOutLength, const int start)
{
  // filesOutLength has to be more than 0 or we can't put any files in filesOut
  assert(filesOutLength > 0 && "listDirectoryContents failed");

  WIN32_FIND_DATA winFile;
  HANDLE fileFinder = NULL;
  char strPath[MAX_PATH];

  assert(sprintf(strPath, "%s\\*.*", directory) >= 0 && "sprintf failed");

  fileFinder = FindFirstFile(strPath, &winFile);

  if (fileFinder == INVALID_HANDLE_VALUE)
  {
    return -1;
  }

  for (int i = 0; i < start; i++)
  {
    if (FindNextFile(fileFinder, &winFile) == 0)
    {
      return 0;
    }
    bool hasValidName = strcmp(winFile.cFileName, ".") != 0 && strcmp(winFile.cFileName, "..") != 0;
    if (!hasValidName)
    {
      i--;
    }
  }

  int i = 0;
  do
  {
    // double checks that we can access filesOut[i] without accessing invalid memory
    assert(i < filesOutLength && "listDirectoryContents failed");

    bool hasValidName = strcmp(winFile.cFileName, ".") != 0 && strcmp(winFile.cFileName, "..") != 0;
    if (hasValidName)
    {

      if (winFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        filesOut[i].isDirectory = true;
        assert(sprintf(filesOut[i].filename, "%s", winFile.cFileName) >= 0 && "sprintf failed");
      }
      else
      {
        filesOut[i].isDirectory = false;
        assert(sprintf(filesOut[i].filename, "%s", winFile.cFileName) >= 0 && "sprintf failed");
      }
      i++;
    }
  } while (FindNextFile(fileFinder, &winFile) != 0 && i < filesOutLength);

  assert(FindClose(fileFinder) != 0 && "FindClose failed");

  return i;
}

// returns false when the path is invalid
bool moveUpDirectory(char *path)
{
  size_t pathLength = strlen(path);
  if (pathLength <= 0)
  {
    return false;
  }

  // gets the index of the last \ so it can remove that and everything after it
  int lastSlashIndex = -1;
  for (int i = 0; i < (int)pathLength; i++)
  {
    if (path[i] == '\\')
    {
      lastSlashIndex = i;
    }
  }

  if (lastSlashIndex == -1)
  {
    return true;
  }

  path[lastSlashIndex] = '\0';
  return true;
}

// returns amount of files in the directory; returns -1 when the directory does not exist
int getDirectoryFileAmount(const char *directory)
{
  WIN32_FIND_DATA winFile;
  HANDLE fileFinder = NULL;
  char strPath[MAX_PATH];

  assert(sprintf(strPath, "%s\\*.*", directory) >= 0 && "sprintf failed");

  fileFinder = FindFirstFile(strPath, &winFile);

  if (fileFinder == INVALID_HANDLE_VALUE)
  {
    return -1;
  }

  int i = 0;
  do
  {
    i++;
  } while (FindNextFile(fileFinder, &winFile) != 0);

  assert(FindClose(fileFinder) != 0 && "FindClose failed");

  return i;
}

void moveIntoDirectory(char *currentDirectory, const char *directoryName)
{
  strcat(currentDirectory, "\\");
  strcat(currentDirectory, directoryName);
}