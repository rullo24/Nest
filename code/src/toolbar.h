#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "cStructs.h"

GtkToolItem* createToolbarButton(PROGRAMHEAPMEM **ptr_uniHeapMem, char *buttonNameAsString, char *pathFromExecDir, int toolbarHeight);
GtkWidget* createToolbar(PROGRAMHEAPMEM **ptr_uniHeapMem, int toolbarHeight);

#endif // !
