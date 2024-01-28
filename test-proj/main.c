#include <gtk/gtk.h>

// Callback function for handling Enter key press in the address bar
static void addressBarActivated(GtkEntry *entry, gpointer user_data) {
    const gchar *newPath = gtk_entry_get_text(entry);
    g_print("Path entered: %s\n", newPath);

    // Perform actions based on the entered path, e.g., navigate to the directory
}

// Callback function for handling button clicks for each previous parent directory
static void navigateToParent(GtkButton *button, gpointer user_data) {
    const gchar *parentPath = (const gchar *)user_data;
    g_print("Navigate to parent: %s\n", parentPath);

    // Perform actions based on navigating to the parent directory
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Address Bar with Parent Buttons");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 80);

    // Create a vertical box container
    GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a horizontal box for the address bar and buttons
    GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    // Create address bar (GtkEntry)
    GtkWidget *addressEntry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), addressEntry, TRUE, TRUE, 0);

    // Connect the "activate" signal to the callback function
    g_signal_connect(G_OBJECT(addressEntry), "activate", G_CALLBACK(addressBarActivated), NULL);

    // Create buttons for each previous parent directory
    const gchar *parentDirectories[] = {
        "/",
        "/home",
        "/home/user",
    };

    for (int i = 0; i < G_N_ELEMENTS(parentDirectories); ++i) {
        GtkWidget *parentButton = gtk_button_new_with_label(parentDirectories[i]);
        g_signal_connect(G_OBJECT(parentButton), "clicked", G_CALLBACK(navigateToParent), (gpointer)parentDirectories[i]);
        gtk_box_pack_start(GTK_BOX(hbox), parentButton, FALSE, FALSE, 0);
    }

    // Connect the "destroy" signal to exit the GTK main loop
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
