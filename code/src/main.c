#include <gtk/gtk.h>
#include "linkedlist.h"
#include <stdio.h>
#include "window.h"
#include "toolbar.h"
#include "widgets.h"
#include "misc.h"
#include "log.h"
#include "filechoose.h"
#include "colours.h"

//////////////////////////////////////////
////////// FORWARD DECLARATIONS //////////

uint8_t initHeapMemPointers(PROGRAMHEAPMEM *ptr_programWideHeapMem);
uint8_t initMainFuncs();
void freeHeapMemPointers(PROGRAMHEAPMEM *ptr_programWideHeapMem);

////// END OF FORWARD DECLARATIONS ///////
//////////////////////////////////////////

// ********************************************************************** //
// ************************* START OF MAIN FUNC ************************* //


int main(int argc, char** argv) {
  // Parsing all TOML settings into a usable struct
  // NESTSETTINGS nestSettings = grabNestSettings(); // NEEDS TO BE COMPLETED

  uint8_t mainFuncRunResult = initMainFuncs();
  if (mainFuncRunResult == -1) {
    logMessage("FATAL ERROR: One (or more) main.c functions failed to run");
    return -1;
  }

  PROGRAMHEAPMEM *ptr_programWideHeapMem = (PROGRAMHEAPMEM*)malloc(sizeof(PROGRAMHEAPMEM));
  if (ptr_programWideHeapMem == NULL) {
    logMessage("FATAL ERROR: Program-wide heap mem has not been allocated. Most functions will not have the required data [main.c]");
    return -1;
  }

  uint8_t heapMemAllocationResult = initHeapMemPointers(ptr_programWideHeapMem);
  if (heapMemAllocationResult == -1) {
    logMessage("FATAL ERROR: Program-wide heap mem unsuccessful set of data [main.c]");
    return -1;
  }

  // Creating node ptrs for linked list
  ptr_programWideHeapMem->ptr_headLL = NULL;
  ptr_programWideHeapMem->ptr_tailLL = NULL;
  strcpy(ptr_programWideHeapMem->nestAppDirectory, "C:\\Coding\\Projects\\C\\Nest"); // Starting point should be taken from settings in future

  // Applying the Toolbar to the main window
  layoutBaseApp(ptr_programWideHeapMem->mainWindow, &(ptr_programWideHeapMem->ptr_headLL), &(ptr_programWideHeapMem->ptr_tailLL), &(ptr_programWideHeapMem->nestAppDirectory));  
	
  // Starting the GTK Loop
  startWindowLoop(ptr_programWideHeapMem->mainWindow);

  //////// NEED TO DEALLOCATE ALL WIDGET MEMORY WHEN DONE??? ////////
  // NEED TO FINISH FREEING ALL MEMORY IN THIS FUNC //
  freeHeapMemPointers(ptr_programWideHeapMem);
  return 0;
}

// ************************* END OF MAIN FUNC *************************
// ******************************************************************** //

// Initialising memory that will be used for program-wide functionality w/o a 'gazzilion' arguments and difficulty freeing memory
uint8_t initHeapMemPointers(PROGRAMHEAPMEM *ptr_programWideHeapMem) {
  // Malloc controlled vars //
  ptr_programWideHeapMem->ptr_memToFreeHead_LL = (FREEMEMORYNODE*)malloc(sizeof(FREEMEMORYNODE)); // Allocating mem for free-tracking LL head
  if (ptr_programWideHeapMem->ptr_memToFreeHead_LL == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_memToFreeHead_LL [main.c]");
    return -1;
  }
  ptr_programWideHeapMem->ptr_memToFreeTail_LL = (FREEMEMORYNODE*)malloc(sizeof(FREEMEMORYNODE)); // Allocating mem for free-tracking LL tail
  if (ptr_programWideHeapMem->ptr_memToFreeTail_LL == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_memToFreeTail_LL [main.c]");
    return -1;
  }
  ptr_programWideHeapMem->ptr_nestSettings = (NESTSETTINGS*)malloc(sizeof(NESTSETTINGS)); // Allocating mem for setting struct
  if (ptr_programWideHeapMem->ptr_nestSettings == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_nestSettings [main.c]");
    return -1;
  }
  ptr_programWideHeapMem->ptr_headLL = (LLNode*)malloc(sizeof(LLNode)); // Allocating mem for file button head LL
  if (ptr_programWideHeapMem->ptr_headLL == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_headLL [main.c]");
    return -1;
  }
  ptr_programWideHeapMem->ptr_tailLL = (LLNode*)malloc(sizeof(LLNode)); // Allocating mem for file button tail LL
  if (ptr_programWideHeapMem->ptr_tailLL == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_tailLL [main.c]");
    return -1;
  }
  ptr_programWideHeapMem->nestAppDirectory = (char*)malloc(sizeof(char)); // Allocating mem for the Nest app current dir tracker
  if (ptr_programWideHeapMem->nestAppDirectory == NULL) {
    logMessage("ERROR: Failed to allocate memory for nestAppDirectory [main.c]");
    return -1;
  }

  // GTK controlled vars //
  ptr_programWideHeapMem->mainWindow = generateWindow(); // GtkWidget* created from gtk_window_new() func (also changes several GTK window settings)
  ptr_programWideHeapMem->mainCssProvider = loadCssProviderAndStyles(); // GtkCssProvider* created from gtk_css_provider_new() (local file used)

  return 1; // Return success flag
}

// Running all 'checking functions' at the beginning of the program
uint8_t initMainFuncs() {
  // Check if powershell is downloaded on the targets computer --> Required for several commands
  uint8_t powershellInstalledResult = isPowershellInstalledOnTargetComp();
  if (powershellInstalledResult != 1) {
    logMessage("ERROR: Failed to locate powershell install [main.c]");
    return -1; // Failure
  }

  return 1; // Indicates success
}

//!!!!!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!!!
// Freeing all required memory on the heap          
//!!!!!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!!!
void freeHeapMemPointers(PROGRAMHEAPMEM *ptr_programWideHeapMem) {
  // Freeing the directory address string
  free(ptr_programWideHeapMem->nestAppDirectory);

}

