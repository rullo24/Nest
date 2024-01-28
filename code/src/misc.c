// OS specific .h includes
#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <limits.h>
#include <unistd.h>
#endif

// Basic .h includes
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include "log.h"
#include <gtk/gtk.h>
#include <string.h>

// The code below is O/S specific
void getExecDirPathWindows(char pathBuffer[], size_t pathBufferMaxSize) {
  // Run this code if the compilation system is windows-based
  #ifdef _WIN32
    // Apparently the windows max dir address size is 260 bytes
    DWORD pathSize = GetModuleFileName(NULL, pathBuffer, pathBufferMaxSize); // Windows.h functions will not work correctly if pathBufferMaxSize > 260 bytes

    if (pathSize < 1) {
      logMessage("ERROR: Failed to get module file name (misc.h)");
      g_print("ERROR: Failed to get module file name (misc.h)");
    }

    // Removing the .exe part of the address by switching final \ for a \0 char
    char *lastBackslashInExeAddress = strrchr(pathBuffer, '\\');
    if (lastBackslashInExeAddress == NULL) {
      logMessage("ERROR: Failed to find the last backslash in the .exe address");
    }
    *lastBackslashInExeAddress = '\0';
}

void appendFileToExecDirWindowsWDoubleBackslash(char *fileLocation, char pathBuffer[], size_t pathBufferMaxSize) {
  // Getting the dir of the .exe
  getExecDirPathWindows(pathBuffer, pathBufferMaxSize);

  // Appending the file location to the .exe's parent directory --> This updates the pathBuffer string
  strcat(pathBuffer, fileLocation);

}

void getExecDirPathLinux(char pathBuffer[], size_t pathBufferMaxSize) {
  // Run this code if the compilation system is linux-based
  #elif __linux__
    long pathSize = pathconf("/proc/self/exe", _PC_PATH_MAX);
    if((pathSize <= 0) || (pathSize == -1)) {
      g_print("ERROR: No memory allocated for executable's true, absolute path");
      return NULL; // Unable to return path size
    }

    // Allocating the memory for the executables absolute path (as a string)
    char *absPath = (char*)malloc(pathSize+1);
    if(absPath == NULL) {
      g_print("ERROR: Could not allocate memory based on exe pathSize");
      return NULL;
    }

    // readlink returns the number of characters copied excluding the null terminator
    ssize_t pathStringLetterCount = readlink("/proc/self/exe", absPath, pathSize - 1);

    if(pathStringLetterCount < 0) {
      free(absPath);
      g_print("ERROR: Unable to read the path string");
      return NULL;
    }

    absPath[pathStringLetterCount] = '\0'; // Null-terminate the string 

    char* dirPath = strdup(absPath); // strdup mallocs new memory for the string. This must be freed as soon as it is not longer in use.

    // Freeing the original buffer
    free(absPath);
    return dirPath;

  #endif
}