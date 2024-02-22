#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "linkedlist.h"
#include <gtk/gtk.h> // Used exclusively for debugging (printing LL to console)
#include <stdbool.h>

// Function to create a new node
LLNode *createLLNode(WINDOWSFILEDATA *fileData) {
    // LLNode *ptr_newNode = (LLNode*)malloc(sizeof(LLNode));
    LLNode *ptr_newNode = (LLNode*)malloc(sizeof(LLNode));

    if (ptr_newNode == NULL) {
        g_print("ERROR: Failed to Malloc for LL");
        logMessage("ERROR: Failed to Malloc for the LL");
    }

    ptr_newNode->fileData = fileData;
    ptr_newNode->nextNode = NULL;
    ptr_newNode->prevNode = NULL;

    return ptr_newNode;
}

// Insert a node at the end of the LL -> Require a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope
void insertAtEndLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
    // Using malloc from the parsed nodeSize
    LLNode *ptr_newNode = createLLNode(fileData);

    if(*ptrptr_headLL == NULL) {
        *ptrptr_headLL = ptr_newNode;
    }
    else {
        (*ptrptr_tailLL)->nextNode = ptr_newNode;
        ptr_newNode->prevNode = (*ptrptr_tailLL);
    }

    // Moving the tail further down
    (*ptrptr_tailLL) = ptr_newNode;
}

// Insert a node at the beginning of the LL -> Requires a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope 
void insertAtStartLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
    // Using malloc from the parsed nodeSize
    LLNode *ptr_newNode= createLLNode(fileData);

    if(*ptrptr_headLL == NULL) {
        *ptrptr_headLL = ptr_newNode;
    }
    else {
        (*ptrptr_headLL)->prevNode = ptr_newNode;
        ptr_newNode->nextNode = *ptrptr_headLL;
    }

    // Making room at the LL head for the new node
    *ptrptr_headLL = ptr_newNode;
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
void insertInAlphabeticalFilenameLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, WINDOWSFILEDATA *fileData) {
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
    if (*ptrptr_headLL == NULL) {
        *ptrptr_headLL = *ptrptr_tailLL = ptr_newNode;
        return;
    }

    // Insert in its alphabetical position if the LL is of size > 1
    LLNode *currentNode = *ptrptr_headLL;
    LLNode *prevNode = NULL;
    while(currentNode != NULL
    && (whichStringIsFirstLexically(currentNode->fileData->cFileName, bufferLowercaseName) == 1)) { // Checking if the newNode should be moved further down
        prevNode = currentNode;
        currentNode = currentNode->nextNode;
    }

    if (prevNode == NULL) { // If inserting into a 1 node LL and ptr_newNode name is greater than the old node
        ptr_newNode->nextNode = *ptrptr_headLL;
        *ptrptr_tailLL = *ptrptr_headLL;
        *ptrptr_headLL = ptr_newNode;
        return;
    }
    else {
        // Insert the new node between prevNode and currentNode
        ptr_newNode->nextNode = prevNode->nextNode;
        prevNode->nextNode = ptr_newNode;
    }

    // Adjusting the tail if this is pushed to the end
    if (ptr_newNode->nextNode == NULL) {
        *ptrptr_tailLL = ptr_newNode; 
    }
}

// Removing the end LL node -> Requires a double pointer otherwise the ptr_headLL & tail will not be changed outside of the function scope 
void removeLastNodeLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
    if (*ptrptr_headLL == NULL) {
        logMessage("ERROR: Tried to remove a node from an empty LL");
        return;
    }

    if(*ptrptr_tailLL == NULL) {
        logMessage("ERROR: NULL value LL tail [linkedlist.c]");
        return;
    }

    // Working on a single-node LL
    if((*ptrptr_tailLL)->prevNode == NULL) {
        free(*ptrptr_tailLL); // Freeing the memory allocated to the struct that the pointer points to
        // Don't need to free() the head as it is the same memory as the tail
        *ptrptr_tailLL = *ptrptr_headLL = NULL;
    }
    // Working on a LL larger than 1 node
    else {
        WINDOWSFILEDATA *tempFileData = (*ptrptr_tailLL)->fileData;
        *ptrptr_tailLL = (*ptrptr_tailLL)->prevNode;
        (*ptrptr_tailLL)->nextNode = NULL;
        free(tempFileData);
    }
}

// Function to reverse a linked list.
void reverseEntireLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
    LLNode *ptr_tempPrev = NULL;
    LLNode *ptr_tempCurrent = *ptrptr_headLL;
    LLNode *ptr_tempNext = NULL;

    while (ptr_tempCurrent != NULL) {
        ptr_tempNext = ptr_tempCurrent->nextNode; // Store next node
        ptr_tempCurrent->nextNode = ptr_tempPrev; // Change next of current node
        ptr_tempPrev = ptr_tempCurrent;       // Move prev to current node
        ptr_tempCurrent = ptr_tempNext;       // Move current to next node
    }

    *ptrptr_tailLL = *ptrptr_headLL; // New tail is old head
    *ptrptr_headLL = ptr_tempPrev; // New head is last node (prev)
}

// Removing all nodes in the LL (freeing the memory while doing so)
void freeAllFileMemoryLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
    if(*ptrptr_tailLL == NULL) {
        logMessage("ERROR: Tried to free an empty LL");
        return;
    }

    // Iterating through LL to free memory
    LLNode *ptr_currentNode = *ptrptr_tailLL; 
    while(ptr_currentNode != NULL) {
        removeLastNodeLL(ptrptr_headLL, ptrptr_tailLL); // This function free()s all allocated memory (node and windows data) whilst removing the tail node
        ptr_currentNode = *ptrptr_tailLL;
    }
}

// Clearing a LL if it is currently in use
void checkAndClearLL(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
    // Do nothing if the LL is empty --> in an empty LL, tail ptr = head ptr = NULL
    if(*ptrptr_tailLL == NULL) {
        return;
    }

    // Clearing the LL only of there is memory in it
    LLNode *ptr_currentNode = *ptrptr_tailLL; 
    while(ptr_currentNode != NULL) {
        removeLastNodeLL(ptrptr_headLL, ptrptr_tailLL); // This function free()s all allocated memory (node and windows data) whilst removing the tail node
        ptr_currentNode = *ptrptr_tailLL;
    }

    *ptrptr_headLL = *ptrptr_tailLL = NULL;
}

// Printing the name of each file that is in the linkedlist
void debug_printEntireLLFileName(LLNode *ptr_headLL) {
    if(ptr_headLL == NULL) {
        g_print("\nThe LL is empty");
        return;
    }
    
    // Printing the file name of the first node
    g_print("\n=== START OF LL ===\n");
    g_print("\nHead Data: %s", ptr_headLL->fileData->cFileName);

    // Assigning an iterating pointer to cycle through all nodes in the LL
    LLNode *ptr_iterHead = ptr_headLL;
    while(ptr_iterHead->nextNode != NULL) {
        ptr_iterHead = ptr_iterHead->nextNode;
        g_print("\nNode Data: %s", ptr_iterHead->fileData->cFileName);
    }
    g_print("\n\n=== END OF LL ===\n");
}