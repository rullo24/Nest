#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "log.h"
#include "linkedlist.h"
#include <gtk/gtk.h> // Used exclusively for debugging (printing LL to console)

// Function to create a new node
LLNode *createLLNode(WINDOWSFILEDATA *fileData) {
    // LLNode *ptr_newNode = (LLNode*)malloc(sizeof(LLNode));
    LLNode *ptr_newNode = (LLNode*)malloc(sizeof(LLNode));

    if (ptr_newNode == NULL) {
        logMessage("ERROR: Failed to Malloc for the LL");
    }

    ptr_newNode->fileData = fileData;
    ptr_newNode->nextNode = NULL;
    ptr_newNode->prevNode = NULL;

    return ptr_newNode;
}

// Insert a node at the end of the LL -> Require a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope
// void insertAtEndLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
void insertAtEndLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Using malloc from the parsed nodeSize
    LLNode *ptr_newNode = createLLNode(fileData);

    if(uniHeapMem->ptr_headLL == NULL) {
        uniHeapMem->ptr_headLL = ptr_newNode;
    }
    else {
        uniHeapMem->ptr_tailLL->nextNode = ptr_newNode;
        ptr_newNode->prevNode = uniHeapMem->ptr_tailLL;
    }

    // Moving the tail further down
    uniHeapMem->ptr_tailLL = ptr_newNode;
}

// Insert a node at the beginning of the LL -> Requires a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope 
// void insertAtStartLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
void insertAtStartLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Using malloc from the parsed nodeSize
    LLNode *ptr_newNode= createLLNode(fileData);

    if (uniHeapMem->ptr_headLL == NULL) {
        uniHeapMem->ptr_headLL = ptr_newNode;
    }
    else {
        uniHeapMem->ptr_headLL->prevNode = ptr_newNode;
        ptr_newNode->nextNode = uniHeapMem->ptr_headLL;
    }

    // Making room at the LL head for the new node
    uniHeapMem->ptr_headLL = ptr_newNode;
}

// Function to alphabetically compare two strings 
uint8_t whichStringIsFirstLexically(char *str1, char *str2) {
    // Returns: result < 0: str2 before | 0: str1 == str2 | result > 0: str1 before
    int results = strcmp(str1, str2);
    
    if (results > 0) { // str2 comes before str1 alphabetically
        return 2; 
    }
    else if (results < 0) { // str1 comes before str2 alphabetically
        return 1;
    }
    else { // str1 and str2 are the same
        return 0;
    }
}

// Converts all letters in a string to lowercase by iterating through the string and lowerising each letter
void stringToAllLowercase(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// Checking if the file/folder name should be included in the file explorer app
bool checkIfFileOrFolderShownInExplorer(char *filename) {
    if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
        return false;
    }
    return true;
}

// Inserting a node in a LL in alphabetical order --> Takes filename as alphabetical basis
// void insertInAlphabeticalFilenameLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
void insertInAlphabeticalFilenameLL(PROGRAMHEAPMEM **ptr_uniHeapMem, WINDOWSFILEDATA *fileData) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;   

    // Not adding . or .. to the LL
    bool isAbleToUseInFileExplorerBool = checkIfFileOrFolderShownInExplorer(fileData->cFileName);
    if (!isAbleToUseInFileExplorerBool) {
        return; // Don't insert if the file name is "." or ".."
    }

    // Using malloc from the parsed nodeSize
    LLNode *ptr_newNode = createLLNode(fileData);

    // Making the entire string locally lowercase so that it can be compared via ASCII   
    size_t filenameLen = strlen(fileData->cFileName);
    char bufferLowercaseName[filenameLen];
    strcpy(bufferLowercaseName, fileData->cFileName); // Copying the filename into another buffer to be manipulated
    stringToAllLowercase(bufferLowercaseName);

    // If LL is empty, insert at beginning 
    if (uniHeapMem->ptr_headLL == NULL) {
        uniHeapMem->ptr_headLL = uniHeapMem->ptr_tailLL = ptr_newNode;
        return;
    }

    // Insert in its alphabetical position if the LL is of size > 1
    LLNode *currentNode = uniHeapMem->ptr_headLL;
    LLNode *prevNode = NULL;
    while(currentNode != NULL
    && (whichStringIsFirstLexically(currentNode->fileData->cFileName, bufferLowercaseName) == 1)) { // Checking if the newNode should be moved further down
        prevNode = currentNode;
        currentNode = currentNode->nextNode;
    }

    if (prevNode == NULL) { // If inserting into a 1 node LL and ptr_newNode name is greater than the old node
        ptr_newNode->nextNode = uniHeapMem->ptr_headLL;
        uniHeapMem->ptr_tailLL = uniHeapMem->ptr_headLL;
        uniHeapMem->ptr_headLL = ptr_newNode;
        return;
    }
    else {
        // Insert the new node between prevNode and currentNode
        ptr_newNode->nextNode = prevNode->nextNode;
        prevNode->nextNode = ptr_newNode;
    }

    // Adjusting the tail if this is pushed to the end
    if (ptr_newNode->nextNode == NULL) {
        uniHeapMem->ptr_tailLL = ptr_newNode; 
    }
}

// Removing the end LL node -> Requires a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope 
// void removeLastNodeLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
void removeLastNodeLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    if (uniHeapMem->ptr_headLL == NULL) {
        logMessage("ERROR: Tried to remove a node from an empty LL [linkedlist.c]");
        return;
    }

    if(uniHeapMem->ptr_tailLL == NULL) {
        logMessage("ERROR: NULL value LL tail [linkedlist.c]");
        return;
    }

    // Working on a single-node LL
    if (uniHeapMem->ptr_tailLL->prevNode == NULL) {
        WINDOWSFILEDATA *tempFileData = uniHeapMem->ptr_tailLL->fileData;
        free(tempFileData); // Freeing the file data memory that is stored in the last node of the LL
        free(uniHeapMem->ptr_tailLL); // Freeing the memory allocated to the struct that the pointer points to
        uniHeapMem->ptr_tailLL = uniHeapMem->ptr_headLL = NULL; // Don't need to free() the head as it is the same memory as the tail
    }
    // Working on a LL larger than 1 node
    else {
        WINDOWSFILEDATA *tempFileData = uniHeapMem->ptr_tailLL->fileData;
        uniHeapMem->ptr_tailLL = uniHeapMem->ptr_tailLL->prevNode;
        uniHeapMem->ptr_tailLL->nextNode = NULL;
        free(tempFileData);
    }
}

// Function to reverse a linked list.
// void reverseEntireLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
void reverseEntireLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    LLNode *ptr_tempPrev = NULL;
    LLNode *ptr_tempCurrent = uniHeapMem->ptr_headLL;
    LLNode *ptr_tempNext = NULL;

    while (ptr_tempCurrent != NULL) {
        ptr_tempNext = ptr_tempCurrent->nextNode; // Store next node
        ptr_tempCurrent->nextNode = ptr_tempPrev; // Change next of current node
        ptr_tempPrev = ptr_tempCurrent;       // Move prev to current node
        ptr_tempCurrent = ptr_tempNext;       // Move current to next node
    }

    uniHeapMem->ptr_tailLL = uniHeapMem->ptr_headLL; // New tail is old head
    uniHeapMem->ptr_headLL = ptr_tempPrev; // New head is last node (prev)
}

// Removing all nodes in the LL (freeing the memory while doing so)
// void freeAllFileMemoryLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
void freeAllFileMemoryLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    if (uniHeapMem->ptr_tailLL == NULL) {
        logMessage("ERROR: Tried to free an empty LL");
        return;
    }

    // Iterating through LL to free memory
    LLNode *ptr_currentNode = uniHeapMem->ptr_tailLL; 
    while(ptr_currentNode != NULL) {
        removeLastNodeLL(&uniHeapMem); // This function free()s all allocated memory (node and windows data) whilst removing the tail node
        ptr_currentNode = uniHeapMem->ptr_tailLL;
    }
}

// Clearing a LL if it is currently in use
// void checkAndClearLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
void checkAndClearLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Do nothing if the LL is empty --> in an empty LL, tail ptr = head ptr = NULL
    if(uniHeapMem->ptr_tailLL == NULL) {
        return;
    }

    // Clearing the LL only of there is memory in it
    LLNode *ptr_currentNode = uniHeapMem->ptr_tailLL;
    while(ptr_currentNode != NULL) {
        removeLastNodeLL(&uniHeapMem); // This function free()s all allocated memory (node and windows data) whilst removing the tail node
        ptr_currentNode = uniHeapMem->ptr_tailLL;
    }

    uniHeapMem->ptr_headLL = uniHeapMem->ptr_tailLL = NULL;
}

// Printing the name of each file that is in the linkedlist
void debug_printEntireLLFileName(LLNode *ptr_headLL) {
    if(ptr_headLL == NULL) {
        g_print("\nThe LL is empty");
        return;
    }
    
    // Printing the file name of the first node
    g_print("\n=== START OF LL ===\n");
    g_print("\nHead Data: %s", ptr_headLL->fileData->fullPathName);

    // Assigning an iterating pointer to cycle through all nodes in the LL
    LLNode *ptr_iterHead = ptr_headLL;
    while(ptr_iterHead->nextNode != NULL) {
        ptr_iterHead = ptr_iterHead->nextNode;
        g_print("\nNode Data: %s", ptr_iterHead->fileData->fullPathName);
    }
    g_print("\n\n=== END OF LL ===\n");
}