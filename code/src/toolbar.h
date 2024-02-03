#ifndef TOOLBAR_H
#define TOOLBAR_H

GtkToolItem* createToolbarButton(GtkCssProvider *mainCssProvider, char *buttonNameAsString, char *pathFromExecDir, int toolbarHeight);
GtkWidget* createToolbar(GtkWidget *mainWindow, GtkCssProvider *mainCssProvider, int toolbarHeight);

#endif // !
