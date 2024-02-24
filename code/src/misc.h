#ifndef MISC_H
#define MISC_H

#include "cStructs.h"
#include <stdint.h>

NESTSETTINGS grabNestSettings();
void getExecDirPathWindows(char pathBuffer[], size_t pathBufferMaxSize);
void appendFileToExecDirWindowsWDoubleBackslash(char *fileLocation, char pathBuffer[], size_t pathBufferMaxSize);
uint8_t grabParentDirFromDirectory(char **ptr_parsedDirectory);
uint8_t isPowershellInstalledOnTargetComp();
uint8_t doesDirOrFileExistOnMachine(char *directory);
uint8_t isDirLocationValidDir(char *directory);

#endif // 
