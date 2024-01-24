#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include <gtk/gtk.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <limits.h>
#include <unistd.h>
#endif

// The code below is O/S specific
char* getExecAbsolutePath() {
  // Dynamically (using the heap) getting the size of the executable's path (as a string)
  #ifdef _WIN32
    DWORD pathSize = GetModuleFileName(NULL, NULL, 0);
  #elif __linux__
    long pathSize = pathconf("/proc/self/exe", _PC_PATH_MAX);
    if((pathSize <= 0) || (pathSize == -1)) {
      g_print("ERROR: No memory allocated for executable's true, absolute path");
      return NULL; // Unable to return path size
    }
  #endif

  // Allocating the memory for the executables absolute path (as a string)
  char *absPath = (char*)malloc(pathSize);
  if(absPath == NULL) {
    g_print("ERROR: Could not allocate memory based on exe pathSize");
    return NULL;
  }

  // Pushing the absolute path string to the absPath char*
  #ifdef _WIN32
    GetModuleFileName(NULL, absPath, pathSize);
  #elif __linux__
    // readlink returns the number of characters copied excluding the null terminator
    ssize_t pathStringLetterCount = readlink("/proc/self/exe", absPath, pathSize - 1);

    if(pathStringLetterCount < 0) {
      free(absPath);
      g_print("ERROR: Unable to read the path string");
      return NULL;
    }

    absPath[pathStringLetterCount] = '\0'; // Null-terminate the string
  #endif

  // Removing the .exe part of the absPath
  char* lastSlash = strrchr(absPath, '/');
  if (lastSlash != NULL) {
    *lastSlash = '\0';
  }

  // Allocating a new buffer for the exe parent directory location (copying the contents)
  char *dirPath = strdup(absPath);

  // Freeing the original buffer
  free(absPath);

  return dirPath;
}

