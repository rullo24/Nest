#include <gtk/gtk.h>
#include <glib.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "filechoose.h"
#include <stdbool.h>
#include "log.h"

typedef struct {
    TCHAR       cAlternateFileName[14];
    TCHAR       cFileName[MAX_PATH];
    DWORD       dwFileAttributes;
    FILETIME    ftCreationTime;
    FILETIME    ftLastAccessTime;
    FILETIME    ftLastWriteTime;
    DWORD       nFileSizeHigh;
    DWORD       nFileSizeLow;
} WINDOWSFILEDATA;

bool removeAllListBoxRows(GtkWidget *fileListBox) {
    // Get all of the children widgets within the list box   
    GList *listBoxChildren = gtk_container_get_children(GTK_CONTAINER(fileListBox));
    if (listBoxChildren == NULL) {
        logMessage("Error: Failed to get listbox children [filechoose.h].");
        return false;
    }

    // Iterating through each child widget and removing it from the list box
    GList *childrenIterator;
    for(childrenIterator = listBoxChildren; childrenIterator != NULL; childrenIterator = g_list_next(childrenIterator)) {
        gtk_widget_destroy(GTK_WIDGET(childrenIterator->data));
    }

    // Freeing the memory that was used within the list box children
    g_list_free(listBoxChildren);
    return true;
}

// Function to free the windows object when the hashmap is destroyed
static void _freeWindowsHFileObj(gpointer data) {
    WINDOWSFILEDATA *objToFree = (WINDOWSFILEDATA*)data;
    g_free(objToFree);
}

// Higher level wrapper for hashmap functionality start ////////////
void _insertWin32Hashmap(GHashTable *hashmap, char *searchKey, WINDOWSFILEDATA *refFileDataObj) {
    g_hash_table_insert(hashmap, g_strdup(searchKey), (gpointer)refFileDataObj);
}

WINDOWSFILEDATA *_getRefWin32Hashmap(GHashTable *hashmap, char *searchKey) {
    WINDOWSFILEDATA *returnedWin32Obj = (WINDOWSFILEDATA*)g_hash_table_lookup(hashmap, searchKey);
    if(returnedWin32Obj == NULL) {
        logMessage("Error: Could not get the hashmap value [filechoose.h]");
        return NULL;
    }
    return returnedWin32Obj;
}
// End of hashmap functionality wrapper ///////////////////////////

// Remember: Make sure to use \\ for strings when using a forwards slash
void getCurrDirFiles(char *directoryString) {
    // Creating a hashmap to store the files in {stringName:fileObjAddress}
    GHashTable *currDirHashmap = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, _freeWindowsHFileObj);

    // Creating file finding objects [windows.h]
    WIN32_FIND_DATA volatileFindFileData;
    HANDLE fileSearchHandle = FindFirstFile(_T(directoryString), &volatileFindFileData);
    if (fileSearchHandle == INVALID_HANDLE_VALUE) {
        logMessage("Error: Couldn't opening listed directory");
    }

    // _tprintf(_T("RAW: %s\n"), volatileFindFileData.cFileName);
    // Acting on the first selected file --> Save to ll based in alphabetical order (head = A | tail = Z)
    WINDOWSFILEDATA initialFileData = {
        .cAlternateFileName = {0},
        .cFileName = {0},
        .dwFileAttributes = volatileFindFileData.dwFileAttributes,
        .ftCreationTime = volatileFindFileData.ftCreationTime,
        .ftLastAccessTime = volatileFindFileData.ftLastAccessTime,
        .ftLastWriteTime = volatileFindFileData.ftLastWriteTime,
        .nFileSizeHigh = volatileFindFileData.nFileSizeHigh,
        .nFileSizeLow = volatileFindFileData.nFileSizeLow,
    };
    // Copy the content of cAlternateFileName and cFileName
    strcpy(initialFileData.cAlternateFileName, volatileFindFileData.cAlternateFileName);
    strcpy(initialFileData.cFileName, volatileFindFileData.cFileName);
    _tprintf(_T("TESTER: %s\n"), initialFileData.cFileName);

    // TODO:
    // Assign memory for the furtherWindowsFileData variable so that the memory 
    // isn't freed after being used. The values should be freed when removed from the hash.















    // Iterating through all other files in the directory
    FindNextFile(fileSearchHandle, &volatileFindFileData);
    do {
        WINDOWSFILEDATA furtherWindowsFileData = {
            .cAlternateFileName = {0},
            .cFileName = {0},
            .dwFileAttributes = volatileFindFileData.dwFileAttributes,
            .ftCreationTime = volatileFindFileData.ftCreationTime,
            .ftLastAccessTime = volatileFindFileData.ftLastAccessTime,
            .ftLastWriteTime = volatileFindFileData.ftLastWriteTime,
            .nFileSizeHigh = volatileFindFileData.nFileSizeHigh,
            .nFileSizeLow = volatileFindFileData.nFileSizeLow,
        };
        // Copy the content of cAlternateFileName and cFileName
        strcpy(furtherWindowsFileData.cAlternateFileName, volatileFindFileData.cAlternateFileName);
        strcpy(furtherWindowsFileData.cFileName, volatileFindFileData.cFileName);

        _tprintf(_T("TESTER: %s\n"), furtherWindowsFileData.cFileName);

    } while (FindNextFile(fileSearchHandle, &volatileFindFileData) != 0);
    // _tprintf(_T("TESTER: %s | %s\n"), initialFileData.cFileName, secondData.cFileName);
    // _tprintf(_T("TESTER: %p | %p\n"), initialFileData.cFileName, secondData.cFileName);

    FindClose(fileSearchHandle);
}

void displayCurrDirFiles(GtkWidget *mainWindow, GtkWidget *fileListBox) {


}