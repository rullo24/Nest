#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "cStructs.h"

GtkWidget* createButtonForToolbar(PROGRAMHEAPMEM **ptr_uniHeapMem, char *buttonNameAsString, char *pathFromExecDir, int toolbarHeight);
GtkToolItem *createToolbarToolItemFromButton(GtkWidget *toolbarButton);
GtkWidget* createToolbar(PROGRAMHEAPMEM **ptr_uniHeapMem, int toolbarHeight);

#endif // !
