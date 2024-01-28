#include <gtk/gtk.h>
#include <stdio.h>
#include "window.h"
#include "toolbar.h"
#include "widgets.h"

int main(int argc, char** argv) {
  // Setting up main window
  GtkWidget* mainWindow = generateWindow(&argc, &argv);

  // Applying the Toolbar to the main window
  layoutBaseApp(mainWindow);
	
  // Starting the GTK Loop
  startWindowLoop(mainWindow);

  return 0;
}

