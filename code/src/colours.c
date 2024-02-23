#include <stdio.h>
#include "colours.h"
#include <gtk/gtk.h>
#include "log.h"
#include "misc.h"
#include "cStructs.h"

// Should be run last after all buttons have been created
GtkCssProvider* loadCssProviderAndStyles() {
  GtkCssProvider *cssProvider = gtk_css_provider_new(); // Creating a new css object
  GError *CssLoadErr = NULL;
  
  // Finding the path of the colourStyles.css file (relative to the .exe's location)
  char colourStylesLocation[260];
  appendFileToExecDirWindowsWDoubleBackslash("\\code\\styles\\colourStyles.css", colourStylesLocation, sizeof(colourStylesLocation));

  // Matching the .css file to their appropriate css styles
  gtk_css_provider_load_from_file(cssProvider, g_file_new_for_path(colourStylesLocation), &CssLoadErr);

  // Checking if the .css styles could be loaded from the file
  if(CssLoadErr != NULL) {
    // Send an error message to the logfile
    logMessage("ERROR: Unable to load .css styles");
    g_error_free(CssLoadErr); // Freeing the memory that is used in a GError
  }

  return cssProvider;
}

void colourWidgetFromStyles(PROGRAMHEAPMEM **ptr_uniHeapMem, GtkWidget *selectedWidget, char *selectedWidgetName) {
  // Creating an alias for the double pointer pass
  PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

  // Setting the name of the widget in the css file to adhere to style standards
  gtk_widget_set_name(selectedWidget, selectedWidgetName); 

  // Getting the widget's current style object for alteration
  GtkStyleContext *selectedWidgetContext = gtk_widget_get_style_context(selectedWidget);
  gtk_style_context_add_provider(selectedWidgetContext, GTK_STYLE_PROVIDER(uniHeapMem->mainCssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

