#include <gtk/gtk.h>
#include <windows.h>
#include <shlobj.h>
#include <gdk/gdkwin32.h>
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
#include "cStructs.h"
#include "keyboard.h"
#include <glib.h>
#include "filechoose.h"

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

// void _initialiseFiledataInWindowsPtr(char *directoryString, WINDOWSFILEDATA **ptrptr_filedata, WIN32_FIND_DATA *ptr_volatileFindFileData) {
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

    // Calculating the total size (in bytes) of the file from the two unsigned 32-bit portions
    (*ptrptr_filedata)->fileSizeInBytes = (uint64_t)(*ptr_volatileFindFileData).nFileSizeLow;
    (*ptrptr_filedata)->fileSizeInBytes |= ((uint64_t)(*ptr_volatileFindFileData).nFileSizeHigh) << 32; // Shift the high part to the left by 32 bits

    // Determining if the retrieved file is a file or folder
    if ((*ptr_volatileFindFileData).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        (*ptrptr_filedata)->isFolder = true;
    }
    else {
        (*ptrptr_filedata)->isFolder = false;
    }
}

// NOTE: Make sure to use \\ for strings when using a forwards slash
void getCurrDirFilesAddToLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Clearing any LL nodes if the LL is currently occupied --> Returns only curr dir files in the new LL
    checkAndClearLLFiles(&uniHeapMem); // LL head and tail pointer should be set to 0x0000000 after this is run

    // Adding the wildcard to the directory string to search for all files in the directory
    char wildcardDirString[MAX_PATH];
    int resultWildcardConcatSize = snprintf(wildcardDirString, sizeof(wildcardDirString), "%s\\*", uniHeapMem->nestAppDirectory); // Copying the wildcard char to the dir

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
    _initialiseFiledataInWindowsPtr(uniHeapMem->nestAppDirectory, &ptr_initialFileData, &volatileFindFileData);

    // Adding the fileData to the linkedlist
    insertInAlphabeticalFilenameLLFiles(&uniHeapMem, ptr_initialFileData); // ptrptr_xx can be parsed straight because it has already been dereferenced in parent func
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
        _initialiseFiledataInWindowsPtr(uniHeapMem->nestAppDirectory, &ptr_moreWindowFileData, &volatileFindFileData);

        // Adding node to the linkedlist
        insertInAlphabeticalFilenameLLFiles(&uniHeapMem, ptr_moreWindowFileData);

    } while (FindNextFile(fileSearchHandle, &volatileFindFileData) != 0);
    // === END OF ADDING ALL OTHER NODES === //
    ///////////////////////////////////////////

    // Func cleanup calls
    FindClose(fileSearchHandle); // Freeing the memory that is used by the windows.h fileSearchHandle
}

// Give a file location, this function returns the file's windows icon
HICON getIconFromFilepath(const char *filepath) {
    SHFILEINFOA shfi;
    ZeroMemory(&shfi, sizeof(shfi));

    // Retrieve the icon for the given file
    SHGetFileInfoA(filepath, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_SMALLICON);

    return shfi.hIcon;
}

// Function to convert HICON to GdkPixbuf
GdkPixbuf *hiconToPixbuf(HICON hIcon) {
    // Get icon information
    ICONINFO iconInfo;
    GetIconInfo(hIcon, &iconInfo);

    // Get the icon size
    BITMAP bmp; // Stores the bitmap data
    if (GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmp) != 0 && bmp.bmBits != NULL) {
        // Bitmap data is present
        printf("Success\n");
        int width = bmp.bmWidth;
        int height = bmp.bmHeight;

        // Create a GdkPixbuf from the bitmap data
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
            (guchar *)bmp.bmBits,      // Bitmap data
            GDK_COLORSPACE_RGB,        // Colorspace
            TRUE,                      // Has alpha
            8,                         // Bits per sample
            width,                     // Width
            height,                    // Height
            bmp.bmWidthBytes,          // Rowstride
            NULL,                      // Destroy function
            NULL                       // User data
        );

        return pixbuf;
        // Process the bitmap data or perform other operations
    } else {
        printf("[filechoose.c] bmp.bmBits == NULL\n");
        // No bitmap data is present
        // Handle the case where there is no valid bitmap data
    }

    // Clean up resources
    DeleteObject(iconInfo.hbmColor);
    DeleteObject(iconInfo.hbmMask);

    return NULL;
}

// Creating file size string from file size
char *_createFileSizeStringFromSize(LLNode *currentNode) {
    char *concatStrings = (char*)malloc(sizeof(char) * 20); // Max size of string w/o 64-bit overflow and \0 char is 17 chars (provide 20 incase of failed calc)

    if (currentNode->fileData->fileSizeInBytes >= 1073741824) { // Bigger than 1GB
        uint64_t calculatedGB = currentNode->fileData->fileSizeInBytes/1073741824;
        sprintf(concatStrings, "%llu GB", calculatedGB);
    }
    else if (currentNode->fileData->fileSizeInBytes >= 1048576) { // Bigger than 1MB
        uint64_t calculatedMB = currentNode->fileData->fileSizeInBytes/1048576;
        sprintf(concatStrings, "%llu MB", calculatedMB);
    }
    else if (currentNode->fileData->fileSizeInBytes >= 1024) { // Bigger than 1KB
        uint64_t calculatedKB = currentNode->fileData->fileSizeInBytes/1024;
        sprintf(concatStrings, "%llu KB", calculatedKB);
    }
    else {
        sprintf(concatStrings, "%llu Bytes", currentNode->fileData->fileSizeInBytes);
    }

    return concatStrings; // To be free()'d after use
}

// Adding all of the LL files to the listbox
uint8_t addFileButtonsToScreen(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    if (uniHeapMem->fileListBox == NULL) {
        logMessage("ERROR: NULL value for fileListBox that is trying to be accessed [filechoose.c]");
        return -1;
    }

    // Removing all previous file buttons
    removeAllGTKListBoxRows(uniHeapMem->fileListBox);

    if (uniHeapMem->ptr_headLL == NULL) {
        // logMessage("ERROR: Tried to add an empty LL to the screen [filechoose.c]"); --> Don't need this as an error message (e.g empty folders)
        return 1; // Success for an empty folder location (or a broken head ptr) --> fixed elsewhere if so
    }

    LLNode *currentNode = uniHeapMem->ptr_headLL;
    while (currentNode != NULL) {
        GtkWidget *listButton = gtk_button_new_with_label(currentNode->fileData->cFileName);
        g_object_set_data(G_OBJECT(listButton), "WINDOWSFILEDATA", currentNode->fileData); // Setting the button data
        g_signal_connect(listButton, "button-press-event", G_CALLBACK(callbackHandleDoubleClickedFileOrFolder), uniHeapMem); // Attaching callback to double-click action
        colourWidgetFromStyles(&uniHeapMem, listButton, "fileButtons");
        
        GtkWidget *listBoxRow = gtk_list_box_row_new();
        colourWidgetFromStyles(&uniHeapMem, listBoxRow, "fileRow");
        GtkWidget *rowGrid = gtk_grid_new();
        
        // Adding the button to the row
        gtk_grid_attach(GTK_GRID(rowGrid), listButton, 0, 2, 1, 1);

        // Adding the file's icon into the row
        HICON windowsFileIcon = getIconFromFilepath(currentNode->fileData->fullPathName);
        GdkPixbuf *currentFilePixbuf = hiconToPixbuf(windowsFileIcon);

        GtkWidget *currentFileIconFromPixbuf = gtk_image_new_from_pixbuf(currentFilePixbuf);
        gtk_grid_attach_next_to(GTK_GRID(rowGrid), currentFileIconFromPixbuf, listButton, GTK_POS_LEFT, 1, 1);

        // Adding the size of the file if it is not a folder
        if (currentNode->fileData->isFolder == false) { // Only adding the file size if it is not a folder
            char *concatStrings = _createFileSizeStringFromSize(currentNode); // char * memory to be free()'d after use

            GtkWidget *fileSizeLabel = gtk_label_new(concatStrings);
            colourWidgetFromStyles(&uniHeapMem, fileSizeLabel, "fileSizeLabel");
            gtk_grid_attach_next_to(GTK_GRID(rowGrid), fileSizeLabel, listButton, GTK_POS_RIGHT, 1, 1);

            free(concatStrings);
        }
        
        gtk_container_add(GTK_CONTAINER(listBoxRow), rowGrid);
        gtk_list_box_insert(GTK_LIST_BOX(uniHeapMem->fileListBox), listBoxRow, -1);
        gtk_widget_show_all(listBoxRow); // This is require to reshow the widget during runtime --> show_all() recursively shows all children
    
        currentNode = currentNode->nextNode; // Moving to the next node in the LL
    }

    return 1; // Success
}

// Reposting the LL buttons onto the right side of the screen -> return 1 = SUCCESS
uint8_t refreshNewFileDisplayFromLL(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Add files from the new directory to the (now empty) LL | also, removing all data in the current file LL (calls clearAndCheckLL internally)
    getCurrDirFilesAddToLL(&uniHeapMem);

    // Remove all old buttons and create new ones from the new LL data
    addFileButtonsToScreen(&uniHeapMem); // Screen refresh done within this function

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