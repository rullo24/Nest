#include <gtk/gtk.h>
#include "toolbar.h"

void onButtonClickExample(GtkWidget *widget, gpointer data) {
  g_print("Button %s clicked!\n", (char*)data);
}

GtkWidget* createToolbar(GtkWidget *mainWindow, int toolbarHeight) {

  // Creating the toolbar object to hold all required buttons
  GtkWidget *mainToolbar = gtk_toolbar_new();

  // Create some toolbar items
  GtkToolItem *settingsToolbarButton = gtk_tool_button_new(NULL, "settings"); 
  g_signal_connect(settingsToolbarButton, "clicked", G_CALLBACK(onButtonClickExample), "New");

  // Adding the button to the toolbar & changing it's properties
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(settingsToolbarButton), "preferences-system");
  gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), GTK_TOOL_ITEM(settingsToolbarButton), -1);
  gtk_toolbar_set_style(GTK_TOOLBAR(mainToolbar), GTK_TOOLBAR_ICONS);

  // Adding the toolbar to the window
  gtk_widget_show_all(mainToolbar);

  return mainToolbar;
}
