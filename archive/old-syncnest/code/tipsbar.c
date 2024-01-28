#include <stdio.h>
#include <gtk/gtk.h>
#include "tipsbar.h"

void exampleTipsbarButton(GtkWidget *widget, gpointer data) {
    g_print("This is an example button\n");
}

GtkWidget* createTipsbar(GtkWidget *mainWindow, int tipsbarHeight) {
    // Creating the tipsbar (the bottom toolbar)
    GtkWidget *mainTipsbar = gtk_toolbar_new();

    // Create some tipsbar items
    GtkToolItem *cDriveRemainingStorage = gtk_tool_button_new(NULL, "settings");
    g_signal_connect(cDriveRemainingStorage, "clicked", G_CALLBACK(exampleTipsbarButton), "New");

    // Adding the button(s) to the tipsbar & changing their properties
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(cDriveRemainingStorage), "preferences-system");
    gtk_toolbar_insert(GTK_TOOLBAR(mainTipsbar), GTK_TOOL_ITEM(cDriveRemainingStorage), -1);
    gtk_toolbar_set_style(GTK_TOOLBAR(mainTipsbar), GTK_TOOLBAR_ICONS);

    // Adding the tipsbar to the main window
    gtk_widget_show_all(mainTipsbar);

    return mainTipsbar;
}