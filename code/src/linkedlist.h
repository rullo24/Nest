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

void insertAtEndLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *data);
void insertAtStartLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *data);
uint8_t whichStringIsFirstLexically(char *str1, char *str2);
void insertInAlphabeticalFilenameLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData);
void removeLastNodeLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void removeFirstNodeLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void reverseEntireLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void freeAllFileMemoryLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void checkAndClearLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void debug_printEntireLLFileName(LLNode *ptr_headLL);

#endif