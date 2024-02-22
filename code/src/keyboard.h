#ifndef KEYBOARD_H
#define KEYBOARD_H

gboolean checkForEscKeyEnter(GtkWidget *widget, GdkEventKey *event, gpointer userData);
gboolean checkForAddrBarEnter(GtkWidget *widget, GdkEventKey *event, gpointer data);

#endif // !KEYBOARD_H
