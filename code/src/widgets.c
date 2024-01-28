#include <stdio.h>
#include <gtk/gtk.h>
#include <windows.h>
#include <tchar.h>
#include <strings.h>
#include "window.h"
#include "toolbar.h"
#include "tipsbar.h"
#include "colours.h"
#include "misc.h"
#include "log.h"

// A structure that is used to hold the window's width and height
struct WINDOWSIZESTRUCT {
  int windowWidth;
  int windowHeight;
};

// Initialising the WINDOWSIZESTRUCT
void initWINDOWSIZESTRUCT (GtkWidget *mainWindow, struct WINDOWSIZESTRUCT *windowSize) {
  int width, height;
  gtk_window_get_size(GTK_WINDOW(mainWindow), &width, &height);
  windowSize->windowWidth = width;
  windowSize->windowHeight = height;
}

// Structure to hold the data of all .png icons in a hashmap
typedef struct {
  GtkWidget *extIconImage;
} IconImageData;

// Functions to make syntax easier on the eye when reading larger functions
GtkWidget* createSizedVertBox(int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_VERTICAL, edgeSpacing);
}
GtkWidget* createSizedHorzBox(int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_HORIZONTAL, edgeSpacing);
}
GtkWidget* createSizedHorzPane() {
  return gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
}

void layoutBaseApp(GtkWidget* mainWindow) {
  // Generaating the window's css style provider object
  GtkCssProvider *mainCssProvider = loadCssProviderAndStyles();  // Gathering the Working Area for use in widget scaling

  // Holding the current window's sizing
  int workingAreaHeight = getWindowWorkAreaHeight();
  struct WINDOWSIZESTRUCT windowSize; 
  initWINDOWSIZESTRUCT(mainWindow, &windowSize);

  // Create a boxs to hold the toolbar & the other widgets
  GtkWidget *vertBoxLargeMain = createSizedVertBox(0); // Creating a vertical box to house all of the smaller components
  GtkWidget *vertBoxRightMain = createSizedVertBox(0); // Creating a vertical box to hold the directories, file selector & tipsbar
  GtkWidget *vertBoxLeftMain = createSizedVertBox(0); // Creating a vertical box to hold file tree and favourites buttons
  GtkWidget *horzPaneLargeMid = createSizedHorzPane(); // Creating a draggable pane between the tree and directory lister
  gtk_container_add(GTK_CONTAINER(mainWindow), vertBoxLargeMain); 

  // Creating all widgets and defining their properties
  int toolbarHeight = workingAreaHeight/25; // 1:25
  GtkWidget *mainToolbar = createToolbar(mainWindow, toolbarHeight);
  colourWidgetFromStyles(mainCssProvider, mainToolbar, "mainToolbar");

  int tipsbarHeight = workingAreaHeight/60; // 1:60
  GtkWidget *mainTipsbar = createTipsbar(mainWindow, tipsbarHeight);
  colourWidgetFromStyles(mainCssProvider, mainTipsbar, "mainTipsbar");

    // TEST buttons
    GtkWidget *testButtonR = gtk_button_new_with_label("RTEST");
    gtk_box_pack_start(GTK_BOX(vertBoxRightMain), testButtonR, TRUE, TRUE, 0);
    colourWidgetFromStyles(mainCssProvider, testButtonR, "testButtonR"); // colour.h

  // Adding the bottom tipsbar
  gtk_box_pack_start(GTK_BOX(vertBoxRightMain), mainTipsbar, FALSE, FALSE, 0); 

  // Adding widgets to the left vertical box
  // TESTING buttons
    GtkWidget *testButton = gtk_button_new_with_label("Test");
    gtk_box_pack_start(GTK_BOX(vertBoxLeftMain), testButton, TRUE, TRUE, 0); // Adding the test button the left vert box

  // Adding the left and right vertical boxes to the larger horz boxes
  gtk_paned_add1(GTK_PANED(horzPaneLargeMid), vertBoxLeftMain);
  gtk_paned_add2(GTK_PANED(horzPaneLargeMid), vertBoxRightMain);

  // Defining the scaling of widgets (and boxes) inside of other boxes
  gtk_widget_set_size_request(mainToolbar, -1, toolbarHeight);

  gtk_widget_set_size_request(mainTipsbar, -1, tipsbarHeight);
  gtk_widget_set_size_request(testButtonR, -1, windowSize.windowHeight-tipsbarHeight);

  gtk_widget_set_size_request(vertBoxLeftMain, (windowSize.windowWidth/10)*3, -1);
  gtk_widget_set_size_request(vertBoxRightMain, (windowSize.windowWidth/10)*7, -1);

  // Adding the widgets to the large vertical box
  gtk_box_pack_start(GTK_BOX(vertBoxLargeMain), mainToolbar, FALSE, FALSE, 0); // Adding top toolbar 
  gtk_box_pack_start(GTK_BOX(vertBoxLargeMain), GTK_WIDGET(horzPaneLargeMid), TRUE, TRUE, 0);

}