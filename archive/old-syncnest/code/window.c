#include <gtk/gtk.h>
#include "window.h"
#include "toolbar.h"

GtkWidget* generateWindow(int *ref_argc, char** *ref_argv) {
  // Initialize GTK
  gtk_init(ref_argc, ref_argv);

  // Create the main window
  GtkWidget* mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // Defining variables for the base width and height of the computer's Resolution
  int screenResWorkingAreaWidth;
  int screenResWorkingAreaHeight;

  // Reassigning the area width and height
  getWindowWorkArea(&screenResWorkingAreaWidth, &screenResWorkingAreaHeight);

  // Setting Optional Window Stats
  gtk_window_set_title(GTK_WINDOW(mainWindow), "SyncNest");
  gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(mainWindow), screenResWorkingAreaWidth/2, screenResWorkingAreaHeight/2);

  // Connect the "destroy" signal to the on_window_destroy callback
  g_signal_connect(mainWindow, "destroy", G_CALLBACK(on_window_destroy), NULL);

  return mainWindow;
}

int setWindowSizeFromRes(GtkWidget *mainWindow) {
  // Defining variables to store the width and height in
  int screenResWorkingAreaWidth;
  int screenResWorkingAreaHeight;

  // Getting the window's working area via a user function
  getWindowWorkArea(&screenResWorkingAreaWidth, &screenResWorkingAreaHeight);

  // Setting the Window Size to half of the Computer's Resolution
  gtk_window_set_default_size(GTK_WINDOW(mainWindow), screenResWorkingAreaWidth/2, screenResWorkingAreaHeight/2);

  return screenResWorkingAreaHeight;
}

void getWindowWorkArea(int *ref_ResWidth, int *ref_ResHeight) {
  // Create a GDK screen object (used for gathering info abotu the computer's screen)
  GdkRectangle workarea = {0};

  // Gathering the Resolution of the Computer
  gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);

  *ref_ResWidth = workarea.width;
  *ref_ResHeight = workarea.height;
}

// Getting only the height of the work area (for the toolbar sizing)
int getWindowWorkAreaHeight() {
  // Create a GDK screen object (used for gathering info abotu the computer's screen)
  GdkRectangle workarea = {0};

  // Gathering the Resolution of the Computer
  gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);

  int screenResWorkingAreaHeight = workarea.height;

  return screenResWorkingAreaHeight;
}

void startWindowLoop(GtkWidget* mainWindow) {
  // Displaying the window (using the GTK loop)
  gtk_widget_show_all(mainWindow);

  // Start the GTK main loop
  gtk_main();
}

// Callback function for closing "destroying" the window
void on_window_destroy(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}
