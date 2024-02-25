#include <stdio.h>
#include <gtk/gtk.h>
#include "tipsbar.h"
#include "cStructs.h"

// Creating the bottom toolbar for showing information about a directory
GtkWidget* createTipsbar(int tipsbarHeight) {
  // Creating the tipsbar (the bottom toolbar)
  GtkWidget *mainTipsbar = gtk_toolbar_new();
  gtk_container_set_border_width(GTK_CONTAINER(mainTipsbar), 5);

  // Create some tipsbar items
  GtkToolItem *toolbarSpacer1 = gtk_separator_tool_item_new();
  GtkToolItem *toolbarSpacer2 = gtk_separator_tool_item_new();

  GtkWidget *labelFolderSize = gtk_label_new("Folder Size (GB): ");
  GtkToolItem *toolItemFolderSize = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(toolItemFolderSize), labelFolderSize);

  GtkWidget *labelNoSelected = gtk_label_new("Num Selected: ");
  GtkToolItem *toolItemNoSelected = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(toolItemNoSelected), labelNoSelected);

  GtkWidget *labelNoDirFiles = gtk_label_new("Num Folder Files : ");
  GtkToolItem *toolItemNoDirFiles = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(toolItemNoDirFiles), labelNoDirFiles);

  // Adding the label(s) to the tipsbar & changing their properties
  gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), toolItemFolderSize, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), toolbarSpacer1, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), toolItemNoSelected, -1);

  gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), toolbarSpacer2, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), toolItemNoDirFiles, -1);
  gtk_toolbar_set_style(GTK_TOOLBAR(mainTipsbar), GTK_TOOLBAR_ICONS);

  // Adding the tipsbar to the main window
  gtk_widget_show_all(mainTipsbar);

  return mainTipsbar;
}
