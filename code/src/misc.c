// OS specific .h includes
#include <windows.h>

// Basic .h includes
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include "log.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdint.h>
#include "cStructs.h"

// Grabbing settings from the TOML file
NESTSETTINGS grabNestSettings() {
  NESTSETTINGS nestSettings = {0}; // Sets all instances to NULL to start off with
  
  // Getting the exact location of the settings file
  char filepathBuffer[260];
  appendFileToExecDirWindowsWDoubleBackslash("\\settings.toml", filepathBuffer, sizeof(filepathBuffer));

  FILE *settingsFileObj = fopen(filepathBuffer, "r");
  if (!settingsFileObj) {
    logMessage("ERROR: Could not retrieve the settings file [misc.c]");
  }
  // Accessing values of the TOML file and assigning to their counterpart in the settings struct

  return nestSettings;

}

// The code below is O/S specific
void getExecDirPathWindows(char pathBuffer[], size_t pathBufferMaxSize) {
  // Apparently the windows max dir address size is 260 bytes
  DWORD pathSize = GetModuleFileName(NULL, pathBuffer, pathBufferMaxSize); // Windows.h functions will not work correctly if pathBufferMaxSize > 260 bytes

  if (pathSize < 1) {
    logMessage("ERROR: Failed to get module file name (misc.h)");
    g_print("ERROR: Failed to get module file name (misc.h)");
  }

  // Removing the .exe part of the address by switching final \ for a \0 char
  char *lastBackslashInExeAddress = strrchr(pathBuffer, '\\');
  if (lastBackslashInExeAddress == NULL) {
    logMessage("ERROR: Failed to find the last backslash in the .exe address [misc.c]");
  }
  *lastBackslashInExeAddress = '\0';
}

void appendFileToExecDirWindowsWDoubleBackslash(char *fileLocation, char pathBuffer[], size_t pathBufferMaxSize) {
  // Getting the dir of the .exe
  getExecDirPathWindows(pathBuffer, pathBufferMaxSize);

  // Appending the file location to the .exe's parent directory --> This updates the pathBuffer string
  strcat(pathBuffer, fileLocation);

}

uint8_t grabParentDirFromDirectory(char **ptr_parsedDirectory) {
  // Creating an alias for the double pointer
  char *parsedDirectory = *ptr_parsedDirectory;
  char *lastSeparator = strrchr(parsedDirectory, '\\'); // Find the last occurrence of '\\'

  // Check if '\\' is found and comes after the last null terminator
  if (lastSeparator != NULL) {
    char *lastNull = strrchr(parsedDirectory, '\0'); // Find the last null terminator
    *lastSeparator = '\0'; // Replace '\\' with the null byte (shrinking the original string)

    if (lastNull != NULL) {
      *lastNull = 'a'; // Setting the null byte to any random character (as to avoid causing issues if the string is then enlargened later on)
    }
    else {
      logMessage("Could not find the null byte in the parent dir string. String not parsed correctly? [misc.c]");
      return -1;
    }
  }
  else {
    return 0; // Did not change the directory as \\ could not be found in the name (presumed top of tree i.e. C:)
  }

  return 1; // Return success
}

// Checks if powershell is installed on the user's computer | returns 1 = YES | 0 = NO
uint8_t isPowershellInstalledOnTargetComp() {
  int result = system("where powershell.exe > nul");

  if (result == 0) {
    return 1; // Successfully located powershell
  }
  return 0;
}

// Check if a directory exists | returns 1 = YES | 0 = NO | -1 = ERROR
uint8_t doesDirOrFileExistOnMachine(char *directory) {
  // Does not care about case sensitivity
  DWORD fileAttr = GetFileAttributes(directory);

  if (fileAttr != INVALID_FILE_ATTRIBUTES) {
    return 1; // DOES exist
  }

  return 0; // DOES NOT exist
}

// Check if a file location is a directory | returns 1 = YES | 2 = not dir | 0 = NO | -1 = ERROR
uint8_t isDirLocationValidDir(char *directory) {
  // Does not care about case sensitivity
  DWORD fileAttr = GetFileAttributes(directory);

  if (fileAttr != INVALID_FILE_ATTRIBUTES) {
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
      return 1; // DOES exist and is DIR
    }
    return 2; // DOES exist but is a FILE
  }

  return 0; // DOES NOT exist OR is NOT DIR
}