#ifndef LL_H
#define LL_H
#include "cStructs.h"
#include <stdint.h>

// Example of LL setup
/*
  LLNode *ptr_headLL = NULL;
  LLNode *ptr_tailLL = NULL;

  insertAtEndLL(&ptr_headLL, &ptr_tailLL, 22);
  insertAtStartLL(&ptr_headLL, &ptr_tailLL, 1);
  insertAtEndLL(&ptr_headLL, &ptr_tailLL, 24);
  insertAtStartLL(&ptr_headLL, &ptr_tailLL, 11);

  debug_printEntireLL(ptr_headLL);
*/

void insertAtEndLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
void insertAtStartLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
uint8_t whichStringIsFirstLexically(char *str1, char *str2);
void insertInAlphabeticalFilenameLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
void removeLastNodeLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem);
void reverseEntireLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem);
void freeAllFileMemoryLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem);
void checkAndClearLLFiles(PROGRAMHEAPMEM **ptr_uniHeapMem);
void debug_printEntireLLFilesName(LLNode *ptr_headLL);

#endif