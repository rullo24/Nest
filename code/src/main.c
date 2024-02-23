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

uint8_t _initHeapMemPointers(PROGRAMHEAPMEM **ptr_uniHeapMem);
uint8_t _initMainFuncs();
void _freeHeapMemPointers(PROGRAMHEAPMEM **ptr_uniHeapMem);

////// END OF FORWARD DECLARATIONS ///////
//////////////////////////////////////////

// ********************************************************************** //
// ************************* START OF MAIN FUNC ************************* //


int main(int argc, char** argv) {
  // Parsing all TOML settings into a usable struct
  // NESTSETTINGS nestSettings = grabNestSettings(); // NEEDS TO BE COMPLETED

  uint8_t mainFuncRunResult = _initMainFuncs();
  if (mainFuncRunResult == -1) {
    logMessage("FATAL ERROR: One (or more) main.c functions failed to run");
    return -1;
  }

  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)malloc(sizeof(PROGRAMHEAPMEM));
  if (uniHeapMem == NULL) {
    logMessage("FATAL ERROR: Program-wide heap mem has not been allocated. Most functions will not have the required data [main.c]");
    return -1;
  }

  uint8_t heapMemAllocationResult = _initHeapMemPointers(&uniHeapMem);
  if (heapMemAllocationResult == -1) {
    logMessage("FATAL ERROR: Program-wide heap mem unsuccessful set of data [main.c]");
    return -1;
  }

  // Creating node ptrs for linked list
  strcpy(uniHeapMem->nestAppDirectory, "C:\\Coding\\Projects\\C\\Nest"); // Starting point should be taken from settings in future

  // Applying the Toolbar to the main window
  layoutBaseApp(&uniHeapMem);
	
  // Starting the GTK Loop
  startWindowLoop(&uniHeapMem);

  //////// NEED TO DEALLOCATE ALL WIDGET MEMORY WHEN DONE??? ////////
  // NEED TO FINISH FREEING ALL MEMORY IN THIS FUNC //
  _freeHeapMemPointers(&uniHeapMem);
  return 0;
}

// ************************* END OF MAIN FUNC *************************
// ******************************************************************** //

// Initialising memory that will be used for program-wide functionality w/o a 'gazzilion' arguments and difficulty freeing memory
uint8_t _initHeapMemPointers(PROGRAMHEAPMEM **ptr_uniHeapMem) {
  // Creating an alias for the double pointer
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

  // Malloc controlled vars //
  uniHeapMem->ptr_nestSettings = (NESTSETTINGS*)malloc(sizeof(NESTSETTINGS)); // Allocating mem for setting struct
  if (uniHeapMem->ptr_nestSettings == NULL) {
    logMessage("ERROR: Failed to allocate memory for ptr_nestSettings [main.c]");
    return -1;
  }

  uniHeapMem->nestAppDirectory = (char*)malloc(sizeof(char)); // Allocating mem for the Nest app current dir tracker
  if (uniHeapMem->nestAppDirectory == NULL) {
    logMessage("ERROR: Failed to allocate memory for nestAppDirectory [main.c]");
    return -1;
  }

  // Initially setting LL heads and tails to NULL (creating empty LLs)
  uniHeapMem->ptr_headLL = NULL; // LL head memory allocated on node addition
  uniHeapMem->ptr_tailLL = NULL; // LL tail memory allocated on node addition
  uniHeapMem->ptr_memToFreeHead_LL = NULL; // LL head for controlling program free() on exit - memory allocated on node addition
  uniHeapMem->ptr_memToFreeTail_LL = NULL; // LL tail for controlling program free() on exit - memory allocated on node addition

  // GTK controlled vars //
  uniHeapMem->mainWindow = generateWindow(); // GtkWidget* created from gtk_window_new() func (also changes several GTK window settings)
  uniHeapMem->mainCssProvider = loadCssProviderAndStyles(); // GtkCssProvider* created from gtk_css_provider_new() (local file used)

  return 1; // Return success flag
}

// Running all 'checking functions' at the beginning of the program
uint8_t _initMainFuncs() {
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
void _freeHeapMemPointers(PROGRAMHEAPMEM **ptr_uniHeapMem) {
  // Creating an alias for the double pointer
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

  // Freeing the directory address string
  free(uniHeapMem->nestAppDirectory);

  // Freeing the uniHeapMem memory (should be done last)
  free(uniHeapMem);
}

