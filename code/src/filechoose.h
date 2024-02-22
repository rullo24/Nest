#ifndef FILECHOOSE_H
#define FILECHOOSE_H
#include <stdbool.h>
#include "linkedlist.h"
#include <windows.h>
#include "structs.h"
#include <gtk/gtk.h>

void removeAllGTKListBoxRows(GtkWidget *fileListBox);
void getCurrDirFilesAddToLL(char *directoryString, LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL);
void addFileButtonsToScreen(LLNode **ptrptr_headLL, LLNode **ptrptr_tailLL, GtkWidget *fileListBox, GtkCssProvider *mainCssProvider);

#endif