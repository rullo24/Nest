#ifndef KEYBOARD_H
#define KEYBOARD_H

gboolean checkForEscKeyEnter(GtkWidget *widget, GdkEventKey *event, gpointer userData);
gboolean checkForAddrBarEnter(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean callbackHandleDoubleClickedFileOrFolder(GtkWidget *listButton, GdkEventButton *event, gpointer parsedData);

#endif // !KEYBOARD_H
