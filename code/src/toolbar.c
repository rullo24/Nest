#include <gtk/gtk.h>
#include "toolbar.h"
#include "misc.h"
#include "log.h"
#include "colours.h"
#include "keyboard.h"

GtkToolItem* createToolbarButton(GtkCssProvider *mainCssProvider, char *buttonNameAsString, char *pathFromExecDir, int toolbarHeight) {
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
    colourWidgetFromStyles(mainCssProvider, mainToolbarStdButton, buttonNameAsString);
    GtkToolItem *mainToolbarButtonToolItem = gtk_tool_item_new();

    gtk_button_set_image(GTK_BUTTON(mainToolbarStdButton), pngImage);
    gtk_image_set_pixel_size(GTK_IMAGE(pngImage), 24);
    gtk_container_add(GTK_CONTAINER(mainToolbarButtonToolItem), mainToolbarStdButton);

    return mainToolbarButtonToolItem;   
  }
}

GtkWidget* createToolbar(GtkWidget *mainWindow, GtkCssProvider *mainCssProvider, int toolbarHeight, char **ptr_nestAppDirectory) {

  // Creating the toolbar object to hold all required buttons
  GtkWidget *mainToolbar = gtk_toolbar_new();

  // Create a separator and 2x expanders for the gap
  GtkToolItem *mainToolbarSeparatorObjLeft = gtk_separator_tool_item_new();
  GtkToolItem *mainToolbarSeparatorObjRight = gtk_separator_tool_item_new();

  // Creating all of the basic buttons that will be used in the main toolbar
  GtkToolItem *prevUsedDirToolItem = createToolbarButton(mainCssProvider, "prevUsedDirButton", "\\icons\\MainToolbar\\LeftArrow.png", toolbarHeight);
  GtkToolItem *nextUsedDirToolItem = createToolbarButton(mainCssProvider, "nextUsedDirToolItem", "\\icons\\MainToolbar\\RightArrow.png", toolbarHeight);
  GtkToolItem *parentDirToolItem = createToolbarButton(mainCssProvider, "parentDirToolItem", "\\icons\\MainToolbar\\UpArrow.png", toolbarHeight);
  GtkToolItem *refreshCurrentDirToolItem = createToolbarButton(mainCssProvider, "refreshCurrentDirToolIcon", "\\icons\\MainToolbar\\Refresh.png", toolbarHeight);
  GtkToolItem *ftpToolItem = createToolbarButton(mainCssProvider, "ftpToolItem", "\\icons\\MainToolbar\\FTP.png", toolbarHeight);
  GtkToolItem *settingsToolItem = createToolbarButton(mainCssProvider, "settingsToolItem", "\\icons\\MainToolbar\\Settings.png", toolbarHeight);

  // Creating the entry boxes that will be used for the address bar & file searching
  GtkWidget *addressBar = gtk_entry_new();
  GtkToolItem *addressBarToolItem = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(addressBarToolItem), addressBar);

  g_signal_connect(addressBar, "key-press-event", G_CALLBACK(checkForAddrBarEnter), NULL);

  colourWidgetFromStyles(mainCssProvider, addressBar, "addressBar"); // Used to change the font and size of text in the entry box

  GtkWidget *searchBar = gtk_entry_new();
  GtkToolItem *searchBarToolItem = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(searchBarToolItem), searchBar);
  colourWidgetFromStyles(mainCssProvider, searchBar, "searchBar");

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
