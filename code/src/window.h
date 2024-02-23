#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>
#include "cStructs.h"

GtkWidget* generateWindow();
int setWindowSizeFromRes(GtkWidget *mainWindow);
void getWindowWorkArea(int *ref_ResWidth, int *ref_ResHeight);
int getWindowWorkAreaHeight();
void on_window_destroy(GtkWidget *widget, gpointer data);
void startWindowLoop(PROGRAMHEAPMEM **ptr_uniHeapMem);

#endif // WINDOW_H
