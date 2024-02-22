#include <gtk/gtk.h>
#include "linkedlist.h"
#include <stdio.h>
#include "window.h"
#include "toolbar.h"
#include "widgets.h"
#include "misc.h"
#include "log.h"
#include "filechoose.h"

int main(int argc, char** argv) {
  // Parsing all TOML settings into a usable struct
  // NESTSETTINGS nestSettings = grabNestSettings(); // NEEDS TO BE COMPLETED

  // Check if powershell is downloaded on the targets computer --> Required for several commands
  uint8_t powershellInstalledResult = isPowershellInstalledOnTargetComp();
  if (powershellInstalledResult != 1) {
    logMessage("ERROR: Failed to locate powershell install [main.c]");
    return 1;
  }

  // Creating node ptrs for linked list
  LLNode *ptr_headLL = NULL;
  LLNode *ptr_tailLL = NULL;
  char *nestAppDirectory = (char*)malloc(MAX_PATH*sizeof(char));
  strcpy(nestAppDirectory, "C:\\Coding\\Projects\\C\\Nest"); // Starting point should be taken from settings in future
  
  // Setting up main window
  GtkWidget* mainWindow = generateWindow(&argc, &argv);

  // Applying the Toolbar to the main window
  layoutBaseApp(mainWindow, &ptr_headLL, &ptr_tailLL, &nestAppDirectory);
	
  // Starting the GTK Loop
  startWindowLoop(mainWindow);


  ///////////////////////////////////////////////////////////////////
  //////// NEED TO DEALLOCATE ALL WIDGET MEMORY WHEN DONE??? ////////
  ///////////////////////////////////////////////////////////////////
  free(nestAppDirectory);


  return 0;
}
