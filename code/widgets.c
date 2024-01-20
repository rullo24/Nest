#include <stdio.h>
#include <gtk/gtk.h>
#include "toolbar.h"
#include "window.h"
#include "tipsbar.h"

GtkWidget* createSizedVertBox(GtkWidget *mainWindow, int numOfPositions) {
  return gtk_box_new(GTK_ORIENTATION_VERTICAL, numOfPositions);
}

void vertBoxToolbarAndRest(GtkWidget* mainWindow) {
  // Gathering the Working Area for use in widget scaling
  int workingAreaHeight = getWindowWorkAreaHeight();

  // Create a vertical box to hold the toolbar & the other widgets separately (file-tree, directory-bar, file-locater and tipsbar)
  GtkWidget *vertBoxMain = createSizedVertBox(mainWindow, 5);
  gtk_container_add(GTK_CONTAINER(mainWindow), vertBoxMain); // Adding the Vertical Box to the Main mainWindow 

  // Creating toolbar widget and adjusting it's properties
  int toolbarHeight = workingAreaHeight/25; // 1:25
  GtkWidget* mainToolbar = createToolbar(mainWindow, toolbarHeight);

  // Adding the toolbar to the vertical box
  gtk_box_pack_start(GTK_BOX(vertBoxMain), mainToolbar, FALSE, TRUE, 0);
  gtk_widget_set_size_request(mainToolbar, -1, toolbarHeight);

  // Creating the tipsbar widget and adjusting it's properties
  int tipsbarHeight = workingAreaHeight/40; // 1:40
  GtkWidget *mainTipsbar = createTipsbar(mainWindow, tipsbarHeight);

  // Adding the tipsbar to the vertical box
  gtk_box_pack_end(GTK_BOX(vertBoxMain), mainTipsbar, FALSE, TRUE, 0);
  gtk_widget_set_size_request(mainTipsbar, -1, tipsbarHeight);

}

void layoutBaseApp(GtkWidget *mainWindow) {
  // Creating vertical box to hold the toolbar
  vertBoxToolbarAndRest(mainWindow);
}

