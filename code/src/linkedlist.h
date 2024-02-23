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

void insertAtEndLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
void insertAtStartLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
uint8_t whichStringIsFirstLexically(char *str1, char *str2);
void insertInAlphabeticalFilenameLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData);
void removeLastNodeLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
void reverseEntireLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
void freeAllFileMemoryLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
void checkAndClearLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
void debug_printEntireLLFileName(LLNode *ptr_headLL);

#endif