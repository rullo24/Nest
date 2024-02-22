#ifndef STRUCTS_H
#define STRUCTS_H
#include <windows.h>
#include <gtk/gtk.h>

// Stores the file data that is given by windows.h -> FindNextFile()
typedef struct {
    TCHAR       fullPathName[MAX_PATH];
    TCHAR       cAlternateFileName[14];
    TCHAR       cFileName[MAX_PATH];
    DWORD       dwFileAttributes;
    FILETIME    ftCreationTime;
    FILETIME    ftLastAccessTime;
    FILETIME    ftLastWriteTime;
    DWORD       nFileSizeHigh;
    DWORD       nFileSizeLow;
} WINDOWSFILEDATA;

// Linked-list node
typedef struct LLNode {
    WINDOWSFILEDATA *fileData;
    struct LLNode *nextNode;
    struct LLNode *prevNode;
} LLNode;

// A structure that is used to hold the window's width and height
struct WINDOWSIZESTRUCT {
  int windowWidth;
  int windowHeight;
};

// Structure to hold the data of all .png icons in a hashmap
typedef struct {
  GtkWidget *extIconImage;
} IconImageData;

#endif