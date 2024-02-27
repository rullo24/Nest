#ifndef CSTRUCTS_H
#define CSTRUCTS_H
#include <windows.h>
#include <gtk/gtk.h>
#include <stdbool.h>

//////////////////////////////////////////////////////
// Forward declaration of all structs to allow for use

typedef struct WINDOWSFILEDATA WINDOWSFILEDATA;
typedef struct NESTSETTINGS NESTSETTINGS;
typedef struct LLNode LLNode;
typedef struct WINDOWSIZESTRUCT WINDOWSIZESTRUCT;
typedef struct IconImageData IconImageData;
typedef struct DEFAULTWINDOWSLOCATIONS DEFAULTWINDOWSLOCATIONS;
typedef struct PROGRAMHEAPMEM PROGRAMHEAPMEM;
typedef struct prevDirsNode prevDirsNode;
typedef struct FREEMEMORYNODE FREEMEMORYNODE;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

struct WINDOWSFILEDATA {
    TCHAR       fullPathName[MAX_PATH];
    TCHAR       cAlternateFileName[14];
    TCHAR       cFileName[MAX_PATH];
    DWORD       dwFileAttributes;
    FILETIME    ftCreationTime;
    FILETIME    ftLastAccessTime;
    FILETIME    ftLastWriteTime;
    DWORD       nFileSizeHigh;
    DWORD       nFileSizeLow;
    uint64_t    fileSizeInBytes;
    bool        isFolder;
};

struct NESTSETTINGS {
  int             startWindowSizeX;
  int             startWindowSizeY;
  float           startLeftRightBoxRatio;
};

struct LLNode {
    WINDOWSFILEDATA   *fileData;
    LLNode            *nextNode;
    LLNode            *prevNode;
};

struct WINDOWSIZESTRUCT {
  int windowWidth;
  int windowHeight;
};

struct IconImageData {
  GtkWidget *extIconImage;
};

struct PROGRAMHEAPMEM {
  FREEMEMORYNODE  *ptr_memToFreeHead_LL;
  FREEMEMORYNODE  *ptr_memToFreeTail_LL;
  NESTSETTINGS    *ptr_nestSettings;
  LLNode          *ptr_headLL;
  LLNode          *ptr_tailLL;
  char            *nestAppDirectory;
  GtkWidget       *mainWindow; // Pointer controlled by GTK
  GtkCssProvider  *mainCssProvider; // Pointer controlled by GTK
  GtkWidget       *fileListBox; // Holds the file buttons and displayed necessary info
  DEFAULTWINDOWSLOCATIONS *defaultLocations;
};

struct DEFAULTWINDOWSLOCATIONS {
  char *desktopLoc;
  char *documentsLoc;
  char *downloadsLoc;
  char *musicLoc;
  char *picturesLoc;
  char *videosLoc;
};

struct prevDirsNode {
  prevDirsNode *prevDirsNextNode;
  prevDirsNode *prevDirsPrevNode;
  char *prevDirsNodeDirectory;
};

struct FREEMEMORYNODE {
  void                    *memoryToFree;
  struct FREEMEMORYNODE   *nextMemNode;
  struct FREEMEMORYNODE   *prevMemNode;
};

#endif