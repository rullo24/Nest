#ifndef FILECHOOSE_H
#define FILECHOOSE_H
#include <stdbool.h>
#include "linkedlist.h"
#include <windows.h>
#include "cStructs.h"
#include <gtk/gtk.h>

void removeAllGTKListBoxRows(GtkWidget *fileListBox);
void getCurrDirFilesAddToLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
uint8_t addFileButtonsToScreen(PROGRAMHEAPMEM **ptr_uniHeapMem);
uint8_t refreshNewFileDisplayFromLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
WINDOWSFILEDATA *getFileDataFromButton(GtkWidget *parsedButton);

#endif