#include <stdio.h>
#include <gtk/gtk.h>
#include "toolbar.h"
#include "window.h"
#include "tipsbar.h"

struct WINDOWSIZESTRUCT {
  int windowWidth;
  int windowHeight;
};

void initWINDOWSIZESTRUCT (GtkWidget *mainWindow, struct WINDOWSIZESTRUCT *windowSize) {
  int width, height;
  gtk_window_get_size(GTK_WINDOW(mainWindow), &width, &height);
  windowSize->windowWidth = width;
  windowSize->windowHeight = height;
}

GtkWidget* createSizedVertBox(GtkWidget *mainWindow, int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_VERTICAL, edgeSpacing);
}

GtkWidget* createSizedHorzBox(GtkWidget *mainWindow, int edgeSpacing) {
  return gtk_box_new(GTK_ORIENTATION_HORIZONTAL, edgeSpacing);
}

GtkWidget* createSizedHorzPane(GtkWidget *mainWindow) {
  return gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
}

void vertBoxToolbarAndRest(GtkWidget* mainWindow) {
  // Gathering the Working Area for use in widget scaling
  int workingAreaHeight = getWindowWorkAreaHeight();

  // Holding the current window's sizing
  struct WINDOWSIZESTRUCT windowSize; 
  initWINDOWSIZESTRUCT(mainWindow, &windowSize);
  //
  // Create a boxs to hold the toolbar & the other widgets
  GtkWidget *vertBoxLargeMain = createSizedVertBox(mainWindow, 0); // Creating a vertical box to house all of the smaller components
  GtkWidget *vertBoxRightMain = createSizedVertBox(mainWindow, 0); // Creating a vertical box to hold the directories, file selector & tipsbar
  GtkWidget *vertBoxLeftMain = createSizedVertBox(mainWindow, 0); // Creating a vertical box to hold file tree and favourites buttons
  GtkWidget *horzPaneLargeMid = createSizedHorzPane(mainWindow); // Creating a draggable pane between the tree and directory lister
  gtk_container_add(GTK_CONTAINER(mainWindow), vertBoxLargeMain); 

  // Creating all widgets and defining their properties
  int toolbarHeight = workingAreaHeight/25; // 1:25
  GtkWidget *mainToolbar = createToolbar(mainWindow, toolbarHeight);
  int tipsbarHeight = workingAreaHeight/60; // 1:60
  GtkWidget *mainTipsbar = createTipsbar(mainWindow, tipsbarHeight);
 
    // TEST buttons
    GtkWidget *testButtonR = gtk_button_new_with_label("RTEST");
    gtk_box_pack_start(GTK_BOX(vertBoxRightMain), testButtonR, TRUE, TRUE, 0);
    // Attempting to colour the test button
    gtk_widget_set_name(testButtonR, "testButtonR");
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    const gchar *cssData =
        "#testButtonR {"
        "   background-color: #ff0000;" // Set your desired background color
        "   color: #ffffff;"            // Set your desired text color
        "}";
    gtk_css_provider_load_from_data(cssProvider, cssData, -1, NULL);
    GtkStyleContext *styleContext = gtk_widget_get_style_context(testButtonR);
    gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
  gtk_box_pack_start(GTK_BOX(vertBoxRightMain), mainTipsbar, FALSE, FALSE, 0); // Adding the bottom tipsbar

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

void layoutBaseApp(GtkWidget *mainWindow) {
  // Creating vertical box to hold the toolbar
  vertBoxToolbarAndRest(mainWindow);
}

