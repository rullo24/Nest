#ifndef KEYBOARD_H
#define KEYBOARD_H

gboolean checkAndRunIfEscPressed();
gboolean onKeyPress(GtkWidget *widget, GdkEventKey *event, gpointer userData);

#endif // !KEYBOARD_H
