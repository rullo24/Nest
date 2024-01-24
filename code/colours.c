#include <stdio.h>
#include "colours.h"
#include <gtk/gtk.h>
#include "log.h"

// Should be run last after all buttons have been created
void loadColourStyles() {
  GtkCssProvider *cssProvider = gtk_css_provider_new(); // Creating a new css object
  const char *pathToCssStyles = "./colourStyles.css";
  GError *CssLoadErr = NULL;

  // Matching the .css file to their appropriate css styles
  gtk_css_provider_load_from_file(cssProvider, g_file_new_for_path(pathToCssStyles), &CssLoadErr);

  // Checking if the .css styles could be loaded from the file
  if(CssLoadErr != NULL) {
    // Send an error message to the logfile
    logMessage("ERROR: Unable to load .css styles");
    g_error_free(CssLoadErr); // Freeing the memory that is used in a GError
  }
}

void exampleColourLoad() {
  /////// TEST
      // Attempting to colour the test button
      // gtk_widget_set_name(testButtonR, "testButtonR");
      // GtkCssProvider *cssProvider = gtk_css_provider_new();
      // const gchar *cssData =
      //     "#testButtonR {"
      //     "   background-color: #ff0000;" // Set your desired background color
      //     "   color: #ffffff;"            // Set your desired text color
      //     "}";
      // gtk_css_provider_load_from_data(cssProvider, cssData, -1, NULL);
      // GtkStyleContext *styleContext = gtk_widget_get_style_context(testButtonR);
      // gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  /////// END TEST
}
