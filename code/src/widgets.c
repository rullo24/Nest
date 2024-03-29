#include <stdio.h>
#include <gtk/gtk.h>
#include <windows.h>
#include "cStructs.h"
#include <tchar.h>
#include <strings.h>
#include "window.h"
#include "toolbar.h"
#include "tipsbar.h"
#include "colours.h"
#include "misc.h"
#include "log.h"
#include "fileBar.h"
#include "filechoose.h"
#include <stdbool.h>
#include "locations.h"

// Initialising the WINDOWSIZESTRUCT
void initWINDOWSIZESTRUCT (GtkWidget *mainWindow, struct WINDOWSIZESTRUCT *windowSize) {
  int width, height;
  gtk_window_get_size(GTK_WINDOW(mainWindow), &width, &height);
  windowSize->windowWidth = width;
  windowSize->windowHeight = height;
}

// Functions to make syntax easier on the eye when reading larger functions
GtkWidget* createSizedVertBox(int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_VERTICAL, edgeSpacing);
}
GtkWidget* createSizedHorzBox(int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_HORIZONTAL, edgeSpacing);
}
GtkWidget* createSizedHorzPane() {
  return gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
}

// void layoutBaseApp(GtkWidget* mainWindow, LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, char **ptr_nestStartingDir) {
void layoutBaseApp(PROGRAMHEAPMEM **ptr_uniHeapMem) {
  // Creating an alias from the double pointer pass
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem; 

  // Generaating the window's css style provider object
  colourWidgetFromStyles(&uniHeapMem, uniHeapMem->mainWindow, "mainWindow");
  getCurrDirFilesAddToLL(&uniHeapMem); // Adding the initial files to the explorer

  // Holding the current window's sizing
  int workingAreaHeight = getWindowWorkAreaHeight();
  struct WINDOWSIZESTRUCT windowSize; 
  initWINDOWSIZESTRUCT(uniHeapMem->mainWindow, &windowSize);

  // Create a boxs to hold the toolbar & the other widgets
  GtkWidget *vertBoxLargeMain = createSizedVertBox(0); // Creating a vertical box to house all of the smaller components
  GtkWidget *vertBoxRightMain = createSizedVertBox(0); // Creating a vertical box to hold the directories, file selector & tipsbar
  colourWidgetFromStyles(&uniHeapMem, vertBoxRightMain, "backgroundBorder");
  GtkWidget *vertBoxLeftMain = createSizedVertBox(0); // Creating a vertical box to hold file tree and favourites buttons
  GtkWidget *horzPaneLargeMid = createSizedHorzPane(); // Creating a draggable pane between the tree and directory lister
  gtk_container_add(GTK_CONTAINER(uniHeapMem->mainWindow), vertBoxLargeMain); 

  // Creating all widgets and defining their properties
  int toolbarHeight = workingAreaHeight/25; // 1:25
  GtkWidget *mainToolbar = createToolbar(&uniHeapMem, toolbarHeight);
  colourWidgetFromStyles(&uniHeapMem, mainToolbar, "mainToolbar");

  //////////////////////////////////////////////////////
  // Creating and adding widget to the right vert box //

  int tipsbarHeight = workingAreaHeight/60; // 1:60
  GtkWidget *mainTipsbar = createTipsbar(tipsbarHeight);
  colourWidgetFromStyles(&uniHeapMem, mainTipsbar, "mainTipsbar");

  int filebarHeight = workingAreaHeight/40; // 1:40
  GtkWidget *filebar = createFilebar(&uniHeapMem, filebarHeight);
  colourWidgetFromStyles(&uniHeapMem, filebar, "filebar");
  
  GtkWidget *fileBrowserScrolledWindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(fileBrowserScrolledWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  // Allocating memory for the fileListBox (so that it is not freed after the scope of this function finishes) --> Need to free at the end of the program run
  gtk_container_add(GTK_CONTAINER(fileBrowserScrolledWindow), uniHeapMem->fileListBox);
  addFileButtonsToScreen(&uniHeapMem); // Moving files to the listbox
  colourWidgetFromStyles(&uniHeapMem, uniHeapMem->fileListBox, "fileListBox");

  gtk_box_pack_start(GTK_BOX(vertBoxRightMain), filebar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vertBoxRightMain), fileBrowserScrolledWindow, TRUE, TRUE, 0); // The filebox window fills the empty space depending on the window size
  gtk_box_pack_start(GTK_BOX(vertBoxRightMain), mainTipsbar, FALSE, FALSE, 0); 

  // Finished adding widgets to the right vertical box //
  ///////////////////////////////////////////////////////

  //////////////////////////////////////////////////////
  // Creating and adding widgets to the left vert box //
  
  saveLibraryLocationsToUniPtr(&uniHeapMem);

  GtkWidget *leftBoxDriveAndLibLocationScrolledBox = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(leftBoxDriveAndLibLocationScrolledBox), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  // Finished creating and adding widgets to the left vert box
  ////////////////////////////////////////////////////////////












  // Adding the left and right vertical boxes to the larger horz boxes
  gtk_paned_add1(GTK_PANED(horzPaneLargeMid), vertBoxLeftMain);
  gtk_paned_add2(GTK_PANED(horzPaneLargeMid), vertBoxRightMain);

  // Defining the scaling of widgets (and boxes) inside of other boxes
  gtk_widget_set_size_request(mainToolbar, -1, toolbarHeight);
  gtk_widget_set_size_request(mainTipsbar, -1, tipsbarHeight);

  gtk_widget_set_size_request(vertBoxLeftMain, (windowSize.windowWidth/10)*3, -1);
  gtk_widget_set_size_request(vertBoxRightMain, (windowSize.windowWidth/10)*7, -1);

  // Adding the widgets to the large vertical box
  gtk_box_pack_start(GTK_BOX(vertBoxLargeMain), mainToolbar, FALSE, FALSE, 0); // Adding top toolbar 
  gtk_box_pack_start(GTK_BOX(vertBoxLargeMain), GTK_WIDGET(horzPaneLargeMid), TRUE, TRUE, 0);

}