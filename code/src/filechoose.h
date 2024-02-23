#ifndef FILECHOOSE_H
#define FILECHOOSE_H
#include <stdbool.h>
#include "linkedlist.h"
#include <windows.h>
#include "cStructs.h"
#include <gtk/gtk.h>

void removeAllGTKListBoxRows(GtkWidget *fileListBox);
void getCurrDirFilesAddToLL(PROGRAMHEAPMEM **ptr_uniHeapMem);
void addFileButtonsToScreen(PROGRAMHEAPMEM **ptr_uniHeapMem, GtkWidget *fileListBox);
uint8_t refreshNewFileDisplayFromLL(PROGRAMHEAPMEM **ptr_uniHeapMem, GtkWidget *fileListBox);
WINDOWSFILEDATA *getFileDataFromButton(GtkWidget *parsedButton);

#endif