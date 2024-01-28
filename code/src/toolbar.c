#include <gtk/gtk.h>
#include "toolbar.h"
#include "misc.h"
#include "log.h"

GtkToolItem* createMainToolbarButton(char *pathFromExecDir) {
  // Create some toolbar items
  char pathPrevUsedDirImageLocation[260];
  getExecDirPathWindows(pathPrevUsedDirImageLocation, sizeof(pathPrevUsedDirImageLocation));
  strcat(pathPrevUsedDirImageLocation, pathFromExecDir);
  g_print("%s\n", pathPrevUsedDirImageLocation);

  GtkWidget *pngImage = gtk_image_new_from_file(pathPrevUsedDirImageLocation);

  // For some reason this error checking does not work
  if (pngImage == NULL) {
    logMessage("ERROR: Couldn't find the specified .PNG file.");  
    return NULL;
  }
  else {

    gtk_image_set_pixel_size(GTK_IMAGE(pngImage), 24);
    // Create a GtkToolItem and set its properties
    GtkToolItem *mainToolbarButtonToolItem = gtk_tool_item_new();
    GtkWidget *mainToolbarStdButton = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(mainToolbarStdButton), pngImage);
    gtk_container_add(GTK_CONTAINER(mainToolbarButtonToolItem), mainToolbarStdButton);

    return mainToolbarButtonToolItem;   
  }
}

GtkWidget* createToolbar(GtkWidget *mainWindow, int toolbarHeight) {

  // Creating the toolbar object to hold all required buttons
  GtkWidget *mainToolbar = gtk_toolbar_new();

  GtkToolItem *prevUsedDirToolItem = createMainToolbarButton("\\icons\\MainTaskbar\\LeftArrow.png");

  // Adding the button to the toolbar & changing it's properties
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(prevUsedDirToolItem), -1);
  gtk_toolbar_set_style(GTK_TOOLBAR(mainToolbar), GTK_TOOLBAR_ICONS);

  // Adding the toolbar to the window
  gtk_widget_show_all(mainToolbar);

  return mainToolbar;
}
