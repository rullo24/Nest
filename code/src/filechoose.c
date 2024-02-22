#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <tchar.h>
#include <stdio.h>
#include "filechoose.h"
#include <stdbool.h>
#include "log.h"
#include "linkedlist.h"
#include "log.h"
#include "colours.h"
#include "structs.h"
#include "keyboard.h"
#include <glib.h>

// Debugging list box
void _debug_printNumOfListBoxRows(GtkWidget *fileListBox) {
    // Get all of the children widgets within the list box   
    GList *listBoxChildren = gtk_container_get_children(GTK_CONTAINER(fileListBox));
    if (listBoxChildren == NULL) { // Returns NULL if the listbox is empty
        printf("None");
        return;
    }

    // Iterating through each child widget and removing it from the list box
    GList *childrenIterator;
    for(childrenIterator = listBoxChildren; childrenIterator != NULL; childrenIterator = g_list_next(childrenIterator)) {
        GtkWidget *childWidget = GTK_WIDGET(childrenIterator->data);
        printf("Widget Type: %s\n", g_type_name(G_OBJECT_TYPE(childWidget)));

        if (GTK_IS_BUTTON(childWidget)){
            const gchar *buttonLabel = gtk_button_get_label(GTK_BUTTON(childWidget));
            printf("Button Label: %s\n", buttonLabel);
        }
        printf("1x child found\n");
    }

    // Freeing the memory that was used within the list box children
    g_list_free(listBoxChildren);
}

// Refreshing each row in the listbox
void _refreshEachRowListBoxDuringRuntime(GtkWidget *fileListBox) {
    // Get all of the children widgets within the list box   
    GList *listBoxChildren = gtk_container_get_children(GTK_CONTAINER(fileListBox));
    if (listBoxChildren == NULL) { // Returns NULL if the listbox is empty
        printf("None");
        return;
    }

    // Iterating through each child widget and removing it from the list box
    GList *childrenIterator;
    for(childrenIterator = listBoxChildren; childrenIterator != NULL; childrenIterator = g_list_next(childrenIterator)) {
        GtkWidget *childWidget = GTK_WIDGET(childrenIterator->data);
        printf("Widget Type: %s\n", g_type_name(G_OBJECT_TYPE(childWidget)));
        gtk_widget_queue_draw(childWidget);
        printf("1x child found\n");
    }

    // Freeing the memory that was used within the list box children
    g_list_free(listBoxChildren);
}

// Just removes previous buttons. Does not deal with the underlying data LL
void removeAllGTKListBoxRows(GtkWidget *fileListBox) {
    // Get all of the children widgets within the list box   
    GList *listBoxChildren = gtk_container_get_children(GTK_CONTAINER(fileListBox));
    if (listBoxChildren == NULL) { // Returns NULL if the listbox is empty
        return;
    }

    // Iterating through each child widget and removing it from the list box
    GList *childrenIterator;
    for(childrenIterator = listBoxChildren; childrenIterator != NULL; childrenIterator = g_list_next(childrenIterator)) {
        GtkWidget *listBoxRow = GTK_WIDGET(childrenIterator->data); // Get each list row
        GtkWidget *listButton = gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(listBoxRow))), 0, 2); // Get the button widget from the row

        gtk_widget_destroy(listButton);
        gtk_widget_destroy(listBoxRow); // NOTE: Button data memory is freed by the LL
    }

    // Freeing the memory that was used within the list box children
    g_list_free(listBoxChildren);
}

void _initialiseFiledataInWindowsPtr(char *directoryString, WINDOWSFILEDATA **ptrptr_filedata, WIN32_FIND_DATA *ptr_volatileFindFileData) {
    /////////////////////////////////
    // === ADDING INITIAL NODE === //
    // Initialize the struct variables
    memset((*ptrptr_filedata)->fullPathName, 0, sizeof((*ptrptr_filedata)->fullPathName));
    memset((*ptrptr_filedata)->cAlternateFileName, 0, sizeof((*ptrptr_filedata)->cAlternateFileName));
    memset((*ptrptr_filedata)->cFileName, 0, sizeof((*ptrptr_filedata)->cFileName));
    (*ptrptr_filedata)->dwFileAttributes = (*ptr_volatileFindFileData).dwFileAttributes;
    (*ptrptr_filedata)->ftCreationTime = (*ptr_volatileFindFileData).ftCreationTime;
    (*ptrptr_filedata)->ftLastAccessTime = (*ptr_volatileFindFileData).ftLastAccessTime;
    (*ptrptr_filedata)->ftLastWriteTime = (*ptr_volatileFindFileData).ftLastWriteTime;
    (*ptrptr_filedata)->nFileSizeHigh = (*ptr_volatileFindFileData).nFileSizeHigh;
    (*ptrptr_filedata)->nFileSizeLow = (*ptr_volatileFindFileData).nFileSizeLow;

    // Copy the content of cAlternateFileName and cFileName to struct
    strcpy((*ptrptr_filedata)->cAlternateFileName, (*ptr_volatileFindFileData).cAlternateFileName);
    strcpy((*ptrptr_filedata)->cFileName, (*ptr_volatileFindFileData).cFileName);

    // Copying the full path of the file into the LL object
    char fullPathNameBuffer[MAX_PATH];
    snprintf(fullPathNameBuffer, sizeof(fullPathNameBuffer), "%s\\%s", directoryString, (*ptrptr_filedata)->cFileName);
    strcpy((*ptrptr_filedata)->fullPathName, fullPathNameBuffer); // Need to copy as fullPathNameBuffer memory is removed once the func goes out of scope
}

// NOTE: Make sure to use \\ for strings when using a forwards slash
void getCurrDirFilesAddToLL(char *directoryString, LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL) {
    // Clearing any LL nodes if the LL is currently occupied --> Returns only curr dir files in the new LL
    checkAndClearLL(ptrptr_headLL, ptrptr_tailLL);

    // Adding the wildcard to the directory string to search for all files in the directory
    char wildcardDirString[MAX_PATH];
    int resultWildcardConcatSize = snprintf(wildcardDirString, sizeof(wildcardDirString), "%s\\*", directoryString); // Copying the wildcard char to the dir

    if (resultWildcardConcatSize >= sizeof(wildcardDirString)) {
        logMessage("ERROR: Cannot concat a wildcard (*) to the given directory as the total path is > MAX_PATH (260 chars)");
        return;
    }

    // Creating file finding objects [windows.h]
    WIN32_FIND_DATA volatileFindFileData;
    HANDLE fileSearchHandle = FindFirstFile(_T(wildcardDirString), &volatileFindFileData);
    if (fileSearchHandle == INVALID_HANDLE_VALUE) {
        logMessage("Error: Couldn't open listed directory");
        return;
    }

    //////////////////////////////////
    // === START OF ADDING NODE === //
    // Acting on the first selected file --> Save to ll based in alphabetical order (head = A | tail = Z)
    WINDOWSFILEDATA *ptr_initialFileData = (WINDOWSFILEDATA*)malloc((sizeof(WINDOWSFILEDATA))); // Memory to be deallocated after use
    if (ptr_initialFileData == NULL) {
        logMessage("ERROR: Failed to assign memory for WINDOWSFILEDATA struct");
        return;
    }

    // Creating a WINDOWSFILEDATA struct for the initial ptr
    _initialiseFiledataInWindowsPtr(directoryString, &ptr_initialFileData, &volatileFindFileData);

    // Adding the fileData to the linkedlist
    insertInAlphabeticalFilenameLL(ptrptr_headLL, ptrptr_tailLL, ptr_initialFileData); // ptrptr_xx can be parsed straight because it has already been dereferenced in parent func
    // === END OF ADDING NODE === //
    ////////////////////////////////

    /////////////////////////////////////
    // === ADDING SUBSEQUENT NODES === //
    // Iterating through all other files in the directory
    FindNextFile(fileSearchHandle, &volatileFindFileData);
    do {
        WINDOWSFILEDATA *ptr_moreWindowFileData = (WINDOWSFILEDATA*)malloc((sizeof(WINDOWSFILEDATA))); // Memory to be deallocated after use
        if (ptr_moreWindowFileData == NULL) {
            logMessage("ERROR: Failed to assign memory for WINDOWSFILEDATA struct");
            return;
        }
        // Creating a WINDOWSFILEDATA struct for the other file ptrs
        _initialiseFiledataInWindowsPtr(directoryString, &ptr_moreWindowFileData, &volatileFindFileData);

        // Adding node to the linkedlist
        insertInAlphabeticalFilenameLL(ptrptr_headLL, ptrptr_tailLL, ptr_moreWindowFileData);

    } while (FindNextFile(fileSearchHandle, &volatileFindFileData) != 0);
    // === END OF ADDING ALL OTHER NODES === //
    ///////////////////////////////////////////

    // Func cleanup calls
    FindClose(fileSearchHandle); // Freeing the memory that is used by the windows.h fileSearchHandle
}

// Adding all of the LL files to the listbox
void addFileButtonsToScreen(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, GtkWidget *fileListBox, GtkCssProvider *mainCssProvider, char **ptr_nestAppDirectory) {
    // Removing all previous file buttons
    removeAllGTKListBoxRows(fileListBox);

    if (*ptrptr_headLL == NULL) {
        // logMessage("ERROR: Tried to add an empty LL to the screen [filechoose.c]"); --> Don't need this as an error message (e.g empty folders)
        return;
    }

    LLNode *currentNode = *ptrptr_headLL;
    while (currentNode != NULL) {
        GtkWidget *listButton = gtk_button_new_with_label(currentNode->fileData->cFileName);
        g_object_set_data(G_OBJECT(listButton), "WINDOWSFILEDATA", currentNode->fileData); // Setting the button data

        PTRS_NESTDIRCHANGEDATA *nestNecessaryChangeDirData = (PTRS_NESTDIRCHANGEDATA*)malloc(sizeof(PTRS_NESTDIRCHANGEDATA));
        if (nestNecessaryChangeDirData == NULL) {
            logMessage("ERROR: Failed to malloc memory for the PTRS_NESTDIRCHANGEDATA struct [filechoose.c]");
            return;
        }

        // Storing data to pass to the callback function
        nestNecessaryChangeDirData->ptr_nestAppDirectory = ptr_nestAppDirectory;
        nestNecessaryChangeDirData->ptrptr_headLL = ptrptr_headLL;
        nestNecessaryChangeDirData->ptrptr_tailLL = ptrptr_tailLL;
        nestNecessaryChangeDirData->fileListBox = fileListBox;
        nestNecessaryChangeDirData->mainCssProvider = mainCssProvider;
  
        g_signal_connect(listButton, "button-press-event", G_CALLBACK(callbackHandleDoubleClickedFileOrFolder), nestNecessaryChangeDirData); // Attaching callback to double-click action
        colourWidgetFromStyles(mainCssProvider, listButton, "fileButtons");
        
        GtkWidget *listBoxRow = gtk_list_box_row_new();
        colourWidgetFromStyles(mainCssProvider, listBoxRow, "fileRow");
        GtkWidget *rowGrid = gtk_grid_new();
        
        gtk_grid_attach(GTK_GRID(rowGrid), listButton, 0, 2, 1, 1);
        gtk_container_add(GTK_CONTAINER(listBoxRow), rowGrid);
        gtk_list_box_insert(GTK_LIST_BOX(fileListBox), listBoxRow, -1);
        gtk_widget_show_all(listBoxRow); // This is require to reshow the widget during runtime --> show_all() recursively shows all children
    
        currentNode = currentNode->nextNode; // Moving to the next node in the LL
    }
}

// Reposting the LL buttons onto the right side of the screen -> return 1 = SUCCESS
uint8_t refreshNewFileDisplayFromLL(char **ptr_newDirectory, LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, GtkWidget *fileListBox, GtkCssProvider *mainCssProvider) {
    // Add files from the new directory to the (now empty) LL | also, removing all data in the current file LL (calls clearAndCheckLL internally)
    getCurrDirFilesAddToLL(*ptr_newDirectory, ptrptr_headLL, ptrptr_tailLL);

    // Remove all old buttons and create new ones from the new LL data
    addFileButtonsToScreen(ptrptr_headLL, ptrptr_tailLL, fileListBox, mainCssProvider, ptr_newDirectory); // Screen refresh done within this function

    return 1;
}

// Getting the data from a button
WINDOWSFILEDATA *getFileDataFromButton(GtkWidget *parsedButton) {
    WINDOWSFILEDATA *buttonData = g_object_get_data(G_OBJECT(parsedButton), "WINDOWSFILEDATA");
    if (buttonData == NULL) {
        logMessage("ERROR: Failed to get the button's data [keyboard.c]");
        return NULL;
    }
    return buttonData;
}