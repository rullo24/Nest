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
  // TODO: Designating a dark theme to the application

  // Check if powershell is downloaded on the targets computer --> Required for several commands
  uint8_t powershellInstalledResult = isPowershellInstalledOnTargetComp();
  if (powershellInstalledResult != 1) {
    logMessage("ERROR: Failed to locate powershell install [main.c]");
    return 1;
  }

  LLNode *ptr_headLL = NULL;
  LLNode *ptr_tailLL = NULL;

  getCurrDirFilesAddToLL("C:\\Coding\\Projects\\C\\Nest", &ptr_headLL, &ptr_tailLL);
  
  // Setting up main window
  GtkWidget* mainWindow = generateWindow(&argc, &argv);

  // Applying the Toolbar to the main window
  layoutBaseApp(mainWindow, &ptr_headLL, &ptr_tailLL);
	
  // Starting the GTK Loop
  startWindowLoop(mainWindow);


  ///////////////////////////////////////////////////////////////////
  //////// NEED TO DEALLOCATE ALL WIDGET MEMORY WHEN DONE??? ////////
  ///////////////////////////////////////////////////////////////////


  return 0;
}

