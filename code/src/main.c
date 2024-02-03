#include <gtk/gtk.h>
#include <stdio.h>
#include "window.h"
#include "toolbar.h"
#include "widgets.h"
#include "misc.h"
#include "log.h"

int main(int argc, char** argv) {
  // TODO: Designating a dark theme to the application

  // Setting up main window
  GtkWidget* mainWindow = generateWindow(&argc, &argv);

  // Applying the Toolbar to the main window
  layoutBaseApp(mainWindow);
	
  // Starting the GTK Loop
  startWindowLoop(mainWindow);

  return 0;
}

