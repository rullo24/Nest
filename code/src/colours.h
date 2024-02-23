#ifndef COLOURS_H
#define COLOURS_H

#include <gtk/gtk.h>
#include "cStructs.h"

GtkCssProvider* loadCssProviderAndStyles();
void colourWidgetFromStyles(PROGRAMHEAPMEM **ptr_uniHeapMem, GtkWidget *selectedWidget, char *selectedWidgetName);

#endif // !COLOURS_H


