#ifndef MISC_H
#define MISC_H

#include "structs.h"
#include <stdint.h>

NESTSETTINGS grabNestSettings();
void getExecDirPathWindows(char pathBuffer[], size_t pathBufferMaxSize);
void appendFileToExecDirWindowsWDoubleBackslash(char *fileLocation, char pathBuffer[], size_t pathBufferMaxSize);
uint8_t isPowershellInstalledOnTargetComp();
uint8_t doesDirOrFileExistOnMachine(char *directory);
uint8_t isDirLocationValidDir(char *directory);

#endif // 
