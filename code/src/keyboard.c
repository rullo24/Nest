#include <windows.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "keyboard.h"
#include <gtk/gtk.h>
#include "misc.h"
#include "log.h"
#include "structs.h"
#include "filechoose.h"

/////////////////////////////////////////////////////////
///////////////// START OF CUSTOM FUNCS /////////////////

// Exiting the mainWindow on esc key press
gboolean _checkMainWindowKeystrokeForExitKey(GtkWidget *mainWindow, GdkEventKey *event) {
  if(event->keyval == GDK_KEY_Escape) {
    gtk_widget_destroy(mainWindow); // NEEDS TO DEALLOCATE ALL MEMORY FIRST
    return TRUE;
  }

  return FALSE;
}

// Changing directory if addr bar holds a dir
void _changeDirFromAddrBarSpecified(GtkWidget *addrBar, gpointer data) {
  const gchar *addrBarCurrentTextGchar = gtk_entry_get_text(GTK_ENTRY(addrBar));
  char *addrBarCurrentText = (char*)addrBarCurrentTextGchar;
  uint8_t dirExistsAsDir = isDirLocationValidDir(addrBarCurrentText);
  if (dirExistsAsDir == 1) { // Acting on a found dir
    // Move to the new directory
  







  }
  else if (dirExistsAsDir == 2) { // Opening the found file
    HINSTANCE fileOpenResult = ShellExecute(NULL, "open", addrBarCurrentText, NULL, NULL, SW_SHOWNORMAL);
    if (fileOpenResult <= (HINSTANCE)32) {
      logMessage("ERROR: Failed to open the address bar file [keyboard.c]");
      return;
    }
  }
}

// Opening the file of a button
void _openFolderOrFileFromButton(GtkWidget *listButton) {

}

////////////////// END OF CUSTOM FUNCS //////////////////
/////////////////////////////////////////////////////////
// --------------------------------------------------- //
/////////////////////////////////////////////////////////
//////////////// START OF CALLBACK FUNCS ////////////////

// Callback function to be executed whenever a key is pressed
gboolean checkForEscKeyEnter(GtkWidget *mainWindow, GdkEventKey *event, gpointer userData) {
  // Check for escape key pressed
  gboolean escPressedBool = _checkMainWindowKeystrokeForExitKey(mainWindow, event);
  if(escPressedBool == TRUE) {
    return TRUE;
  }

  // If the key press is not a specified key, let GTK handle it
  return FALSE;
}

// Address bar callback function for changing the current dir on enter key press
gboolean checkForAddrBarEnter(GtkWidget *addrBar, GdkEventKey *event, gpointer data) {
  // Requires the key to be pressed down (not on up) & main or keypad enter key
  if (event->type == GDK_KEY_PRESS && (event->keyval == GDK_KEY_Return || event -> keyval == GDK_KEY_KP_Enter)) {
    // Executing directory change func
    _changeDirFromAddrBarSpecified(addrBar, data);

    return TRUE; // Mark the event as used
  }
  return FALSE; // Ignore the results of the event if it does not meet the if requirements
}

// Callback that handles a double clicked file or folder
gboolean callbackHandleDoubleClickedFileOrFolder(GtkWidget *listButton, GdkEventButton *event, gpointer parsedData)
{
    if (event->type == GDK_DOUBLE_BUTTON_PRESS) {
      WINDOWSFILEDATA *tempFileDataPointer = getFileDataFromButton(listButton);     
      if (tempFileDataPointer == NULL) {
        logMessage("ERROR: Empty temp file data pointer [keyboard.c]");
        return FALSE;
      }

    uint8_t dirExistsAsDir = isDirLocationValidDir(tempFileDataPointer->fullPathName);
      if (dirExistsAsDir == 1) { // Moving to this directory

        PTRS_NESTDIRCHANGEDATA *nestNecessaryChangeDirData = (PTRS_NESTDIRCHANGEDATA*)parsedData;
        strcpy(*(nestNecessaryChangeDirData->ptr_nestAppDirectory), (tempFileDataPointer->fullPathName)); // Copying the full path string to the nest app directory var located in main.c
        uint8_t refreshResult = refreshNewFileDisplayFromLL(nestNecessaryChangeDirData->ptr_nestAppDirectory, nestNecessaryChangeDirData->ptrptr_headLL, nestNecessaryChangeDirData->ptrptr_tailLL, nestNecessaryChangeDirData->fileListBox, nestNecessaryChangeDirData->mainCssProvider);

      }
      else if (dirExistsAsDir == 2) { // Opening the found file
        HINSTANCE fileOpenResult = ShellExecute(NULL, "open", tempFileDataPointer->fullPathName, NULL, NULL, SW_SHOWNORMAL);
        if (fileOpenResult <= (HINSTANCE)32) {
          logMessage("ERROR: Failed to open the button's file [keyboard.c]");
          return FALSE;
        }
      }
    }
    return TRUE;
}

///////////////// END OF CALLBACK FUNCS /////////////////
/////////////////////////////////////////////////////////
