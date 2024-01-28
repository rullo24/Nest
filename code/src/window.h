#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>

GtkWidget* generateWindow(int *ref_argc, char** *ref_argv);
int setWindowSizeFromRes(GtkWidget *mainWindow);
void getWindowWorkArea(int *ref_ResWidth, int *ref_ResHeight);
int getWindowWorkAreaHeight();
void on_window_destroy(GtkWidget *widget, gpointer data);
void startWindowLoop(GtkWidget *window);

#endif // WINDOW_H
