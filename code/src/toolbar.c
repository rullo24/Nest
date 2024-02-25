#include <gtk/gtk.h>
#include "toolbar.h"
#include "misc.h"
#include "log.h"
#include "colours.h"
#include "keyboard.h"
#include "filechoose.h"

// Creating a button for use in a GTK toolbar
GtkWidget* createButtonForToolbar(PROGRAMHEAPMEM **ptr_uniHeapMem, char *buttonNameAsString, char *pathFromExecDir, int toolbarHeight) {
  // Creating an alias for the double pointer
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

  // Create some toolbar items
  char pathImageLocation[260];
  getExecDirPathWindows(pathImageLocation, sizeof(pathImageLocation));
  strcat(pathImageLocation, pathFromExecDir);

  // Creating an image object
  GdkPixbuf *pngPixbuf = gdk_pixbuf_new_from_file_at_scale(pathImageLocation, toolbarHeight/2, toolbarHeight/2, TRUE, NULL); // Storing an image in memory
  GtkWidget *pngImage = gtk_image_new_from_pixbuf(pngPixbuf);
  g_object_unref(pngPixbuf); // Freeing the memory used in the pixbuf

  // For some reason this error checking does not work
  if (pngImage == NULL) {
    logMessage("ERROR: Couldn't find the specified .PNG file.");  
    return NULL;
  }
  else {

    // Create a GtkToolItem and set its properties
    GtkWidget *mainToolbarStdButton = gtk_button_new();
    colourWidgetFromStyles(&uniHeapMem, mainToolbarStdButton, buttonNameAsString);

    gtk_button_set_image(GTK_BUTTON(mainToolbarStdButton), pngImage);
    gtk_image_set_pixel_size(GTK_IMAGE(pngImage), 24);

    return mainToolbarStdButton;
  }
}

// Creating a toolbar-compatible item from the GTK kit
GtkToolItem *createToolbarToolItemFromButton(GtkWidget *toolbarButton) {
    GtkToolItem *mainToolbarButtonToolItem = gtk_tool_item_new();
    gtk_container_add(GTK_CONTAINER(mainToolbarButtonToolItem), toolbarButton);

    return mainToolbarButtonToolItem;   
}

// Creating a prevUsedDirButtonCallback
gboolean _parentDirButtonClickCallback(GtkWidget *parentDirButton, gpointer parsedData) {
  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)parsedData;
  char *currentDirBuffer = (char*)malloc(MAX_PATH * sizeof(char));
  strcpy(currentDirBuffer, uniHeapMem->nestAppDirectory); // Copying to the current dir to the buffer (avoiding issues caused by an error)
  uint8_t grabParentResult = grabParentDirFromDirectory(&currentDirBuffer);

  if (grabParentResult == 0) {
    return TRUE; // 0 returned if you cannot move any higher up the tree
  }
  else if (grabParentResult == 1) {
    // Success func run
    strcpy(uniHeapMem->nestAppDirectory, currentDirBuffer); // Copying the string to the nestAppDir file-wide var
    uint8_t refreshResult = refreshNewFileDisplayFromLL(&uniHeapMem);
    if (refreshResult != 1) {
      logMessage("ERROR: Failed to refresh file display from back button press [toolbar.c]");
      return FALSE;
    }
  }
  else {
    logMessage("ERROR: An error occurred in the grabParentDirFromDirectory() func [toolbar.c]");
    return FALSE;
  }

  free(currentDirBuffer); // Refreshing the buffer once the func has run
  return TRUE;
}

// Refreshing the current dir
gboolean _refreshCurrentDirClickCallback(GtkWidget *refreshButton, gpointer parsedData) {
  PROGRAMHEAPMEM *uniHeapMem = (PROGRAMHEAPMEM*)parsedData;

  // Refreshing the display from the LL
  uint8_t refreshResult = refreshNewFileDisplayFromLL(&uniHeapMem);
  if (refreshResult != 1) {
    logMessage("ERROR: Failed to refresh file display from back button press [toolbar.c]");
    return FALSE;
  }
  return TRUE;
}

// GtkWidget* createToolbar(GtkWidget *mainWindow, GtkCssProvider *mainCssProvider, int toolbarHeight, char **ptr_nestAppDirectory) {
GtkWidget* createToolbar(PROGRAMHEAPMEM **ptr_uniHeapMem, int toolbarHeight) {
  // Creating an alias for the double pointer
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

  // Creating the toolbar object to hold all required buttons
  GtkWidget *mainToolbar = gtk_toolbar_new();

  // Create a separator and 2x expanders for the gap
  GtkToolItem *mainToolbarSeparatorObjLeft = gtk_separator_tool_item_new();
  GtkToolItem *mainToolbarSeparatorObjRight = gtk_separator_tool_item_new();

  // Creating all of the basic buttons that will be used in the main toolbar
  GtkWidget *prevUsedDirButton = createButtonForToolbar(&uniHeapMem, "prevUsedDirButton", "\\icons\\MainToolbar\\LeftArrow.png", toolbarHeight);
  GtkToolItem *prevUsedDirToolItem = createToolbarToolItemFromButton(prevUsedDirButton);
  GtkWidget *nextUsedDirButton = createButtonForToolbar(&uniHeapMem, "nextUsedDirToolItem", "\\icons\\MainToolbar\\RightArrow.png", toolbarHeight);
  GtkToolItem *nextUsedDirToolItem = createToolbarToolItemFromButton(nextUsedDirButton);
  GtkWidget *parentDirButton = createButtonForToolbar(&uniHeapMem, "parentDirToolItem", "\\icons\\MainToolbar\\UpArrow.png", toolbarHeight);
  g_signal_connect(parentDirButton, "clicked", G_CALLBACK(_parentDirButtonClickCallback), uniHeapMem);
  GtkToolItem *parentDirToolItem = createToolbarToolItemFromButton(parentDirButton);
  GtkWidget *refreshCurrentDirButton = createButtonForToolbar(&uniHeapMem, "refreshCurrentDirToolIcon", "\\icons\\MainToolbar\\Refresh.png", toolbarHeight);
  g_signal_connect(refreshCurrentDirButton, "clicked", G_CALLBACK(_refreshCurrentDirClickCallback), uniHeapMem);
  GtkToolItem *refreshCurrentDirToolItem = createToolbarToolItemFromButton(refreshCurrentDirButton);
  GtkWidget *ftpButton = createButtonForToolbar(&uniHeapMem, "ftpToolItem", "\\icons\\MainToolbar\\FTP.png", toolbarHeight);
  GtkToolItem *ftpToolItem = createToolbarToolItemFromButton(ftpButton);
  GtkWidget *settingsButton = createButtonForToolbar(&uniHeapMem, "settingsToolItem", "\\icons\\MainToolbar\\Settings.png", toolbarHeight);
  GtkToolItem *settingsToolItem = createToolbarToolItemFromButton(settingsButton);

  // Creating the entry boxes that will be used for the address bar & file searching
  GtkWidget *addressBar = gtk_entry_new();
  GtkToolItem *addressBarToolItem = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(addressBarToolItem), addressBar);
  g_signal_connect(addressBar, "key-press-event", G_CALLBACK(checkForAddrBarEnter), uniHeapMem);
  colourWidgetFromStyles(&uniHeapMem, addressBar, "addressBar"); // Used to change the font and size of text in the entry box

  GtkWidget *searchBar = gtk_entry_new();
  GtkToolItem *searchBarToolItem = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(searchBarToolItem), searchBar);
  colourWidgetFromStyles(&uniHeapMem, searchBar, "searchBar");

  // Adding the buttons and widgets to the toolbar & changing their properties
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(prevUsedDirToolItem), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(nextUsedDirToolItem), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(parentDirToolItem), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(refreshCurrentDirToolItem), -1);

  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(mainToolbarSeparatorObjLeft), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(addressBarToolItem), -1);
  gtk_tool_item_set_expand(addressBarToolItem, TRUE);
  
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(searchBarToolItem), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(mainToolbarSeparatorObjRight), -1);

  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(ftpToolItem), -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(settingsToolItem), -1);

  // Adding the toolbar to the window
  gtk_toolbar_set_style(GTK_TOOLBAR(mainToolbar), GTK_TOOLBAR_ICONS);
  gtk_widget_show_all(mainToolbar);

  return mainToolbar;
}
