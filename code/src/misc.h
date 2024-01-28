#ifndef MISC_H
#define MISC_H

void getExecDirPathWindows(char pathBuffer[], size_t pathBufferMaxSize);
void getExecDirPathLinux(char pathBuffer[], size_t pathBufferMaxSize);
void appendFileToExecDirWindowsWDoubleBackslash(char *fileLocation, char pathBuffer[], size_t pathBufferMaxSize);

#endif // 
