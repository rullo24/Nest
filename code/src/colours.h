#include <gtk/gtk.h>

#ifndef COLOURS_H
#define COLOURS_H

GtkCssProvider* loadCssProviderAndStyles();
void colourWidgetFromStyles(GtkCssProvider *cssProvider, GtkWidget *selectedWidget, char *selectedWidgetName);

#endif // !COLOURS_H


