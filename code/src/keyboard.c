#include <windows.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "keyboard.h"
#include <gtk/gtk.h>
#include "misc.h"
#include "log.h"
#include "cStructs.h"
#include "filechoose.h"

/////////////////////////////////////////////////////////
///////////////// START OF CUSTOM FUNCS /////////////////

// Exiting the mainWindow on esc key press
gboolean _checkMainWindowKeystrokeForExitKey(PROGRAMHEAPMEM *uniHeapMem, GdkEventKey *event) {
  if(event->keyval == GDK_KEY_Escape) {
    gtk_widget_destroy(uniHeapMem->mainWindow); // NEEDS TO DEALLOCATE ALL MEMORY FIRST
    return TRUE;
  }

  return FALSE;
}

// Changing directory if addr bar holds a dir
gboolean _changeDirFromAddrBarSpecified(GtkWidget *addrBar, gpointer parsedData) {
  // Creating a stack pointer for the universal vars
  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)parsedData; 

  const gchar *addrBarCurrentTextGchar = gtk_entry_get_text(GTK_ENTRY(addrBar));
  char *addrBarCurrentText = (char*)addrBarCurrentTextGchar;
  uint8_t dirExistsAsDir = isDirLocationValidDir(addrBarCurrentText);

  if (dirExistsAsDir == 1) { // Move to the new directory

    // Check that data has been passed to the function
    if (uniHeapMem == NULL) {
      logMessage("ERROR: Data not parsed to the changeDir func correctly [keyboard.c]");
      return FALSE;
    }

    // Change dir string in main.c
    strcpy(uniHeapMem->nestAppDirectory, addrBarCurrentText); // Copying the full path string to the nest app directory var located in main.c
    // Remove all fileListBox buttons
    // Remove all nodes in LL 
    // Add new nodes from new dir  
    uint8_t refreshResult = refreshNewFileDisplayFromLL(&uniHeapMem);
    if (refreshResult != 1) {
      logMessage("Failed to refresh file display from LL [keyboard.c]");
      return FALSE;
    }
  }

  else if (dirExistsAsDir == 2) { // Opening the found file
    HINSTANCE fileOpenResult = ShellExecute(NULL, "open", addrBarCurrentText, NULL, NULL, SW_SHOWNORMAL);
    if (fileOpenResult <= (HINSTANCE)32) {
      logMessage("ERROR: Failed to open the address bar file [keyboard.c]");
      return FALSE;
    }
  }

  else {
    logMessage("ERROR: Invalid file or folder location has been entered into the address bar");
    return FALSE;
  }

  return TRUE; // Returned if the function is successful
}

////////////////// END OF CUSTOM FUNCS //////////////////
/////////////////////////////////////////////////////////
// --------------------------------------------------- //
/////////////////////////////////////////////////////////
//////////////// START OF CALLBACK FUNCS ////////////////

// Callback function to be executed whenever a key is pressed
gboolean checkForEscKeyEnter(GtkWidget *mainWindow, GdkEventKey *event, gpointer userData) {
  // Grab userdata from the gpointer parse
  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)userData; 

  // Check for escape key pressed
  gboolean escPressedBool = _checkMainWindowKeystrokeForExitKey(uniHeapMem, event);
  if(escPressedBool == TRUE) {
    return TRUE;
  }

  // If the key press is not a specified key, let GTK handle it
  return FALSE;
}

// Address bar callback function for changing the current dir on enter key press
gboolean checkForAddrBarEnter(GtkWidget *addrBar, GdkEventKey *event, gpointer data) {
  // Grabbing necessary pointers
  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)data; // Type cast from gpointer to NEST PTR data
  
  // Requires the key to be pressed down (not on up) & main or keypad enter key
  if (event->type == GDK_KEY_PRESS && (event->keyval == GDK_KEY_Return || event -> keyval == GDK_KEY_KP_Enter)) {
    // Executing directory change func
    _changeDirFromAddrBarSpecified(addrBar, uniHeapMem);

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

      // ADD THIS TO BUTTON DATA SO THAT IT CAN BE FREED WITH THE DATA??? ADD TO STRUCT???
      PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)parsedData;

      // Check if the data has been parsed correctly
      if (uniHeapMem == NULL) {
        logMessage("ERROR: Data not parsed correctly to file button callback func");
        return FALSE;
      }

      // Creating an alias for the double pointer
      strcpy(uniHeapMem->nestAppDirectory, tempFileDataPointer->fullPathName); // Copying the full path string to the nest app directory var located in main.c
      uint8_t refreshResult = refreshNewFileDisplayFromLL(&uniHeapMem);
      if (refreshResult != 1) {
        logMessage("ERROR: Failed to refresh file display from LL [keyboard.c]");
        return FALSE;
      }
    }
    else if (dirExistsAsDir == 2) { // Opening the found file
      HINSTANCE fileOpenResult = ShellExecute(NULL, "open", tempFileDataPointer->fullPathName, NULL, NULL, SW_SHOWNORMAL);
      if (fileOpenResult <= (HINSTANCE)32) {
        logMessage("ERROR: Failed to open the button's file [keyboard.c]");
        return FALSE;
      }
    }
    else {
      logMessage("ERROR: Invalid file or folder location button has been clicked");
      return FALSE;
    }
  }
  return TRUE;
}

///////////////// END OF CALLBACK FUNCS /////////////////
/////////////////////////////////////////////////////////
