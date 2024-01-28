#include <stdio.h>
#include <gtk/gtk.h>
#include "keyboard.h"

gboolean checkAndRunIfEscPressed(GtkWidget *mainWindow, GdkEventKey *event) {
  if(event->keyval == GDK_KEY_Escape) {
    gtk_widget_destroy(mainWindow);
    return TRUE;
  }

  return FALSE;
}

// Callback function to be executed whenever a key is pressed
gboolean onKeyPress(GtkWidget *mainWindow, GdkEventKey *event, gpointer userData) {
  // Check for escape key pressed
  gboolean escPressedBool = checkAndRunIfEscPressed(mainWindow, event);
  if(escPressedBool == TRUE) {
    return TRUE;
  }

  // If the key press is not a specified key, let GTK handle it
  return FALSE;
}
