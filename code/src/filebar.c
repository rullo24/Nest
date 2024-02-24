#include <gtk/gtk.h>
#include "filebar.h"
#include "colours.h"
#include "misc.h"
#include "toolbar.h"

// GtkWidget *createFilebar(GtkWidget *mainWindow, GtkCssProvider *mainCssProvider, int fileBarHeight) {
GtkWidget *createFilebar(PROGRAMHEAPMEM **ptr_uniHeapMem, int fileBarHeight) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Creating the filebar
    GtkWidget *filebar = gtk_toolbar_new();
    gtk_container_set_border_width(GTK_CONTAINER(filebar), 5);

    // Adding all filebar items
    GtkToolItem *filebarSpacer1 = gtk_separator_tool_item_new();

    GtkWidget *newFileFolderButton = createButtonForToolbar(&uniHeapMem, "newFileFolderButton", "\\icons\\FileViewTaskbar\\CirclePlus.png", fileBarHeight); 
    GtkToolItem *newFileFolderToolItem = createToolbarToolItemFromButton(newFileFolderButton);
    GtkWidget *cutButton = createButtonForToolbar(&uniHeapMem, "cutButton", "\\icons\\FileViewTaskbar\\CutFile.png", fileBarHeight);
    GtkToolItem *cutToolItem = createToolbarToolItemFromButton(cutButton);
    GtkWidget *copyButton = createButtonForToolbar(&uniHeapMem, "copyButton", "\\icons\\FileViewTaskbar\\CopyFile.png", fileBarHeight);
    GtkToolItem *copyToolItem = createToolbarToolItemFromButton(copyButton);
    GtkWidget *pasteButton = createButtonForToolbar(&uniHeapMem, "pasteButton", "\\icons\\FileViewTaskbar\\PasteFile.png", fileBarHeight);
    GtkToolItem *pasteToolItem = createToolbarToolItemFromButton(pasteButton);
    GtkWidget *renameButton = createButtonForToolbar(&uniHeapMem, "renameButton", "\\icons\\FileViewTaskbar\\RenameFile.png", fileBarHeight);
    GtkToolItem *renameToolItem = createToolbarToolItemFromButton(renameButton);
    GtkWidget *shareButton = createButtonForToolbar(&uniHeapMem, "shareButton", "\\icons\\FileViewTaskbar\\ShareFile.png", fileBarHeight);
    GtkToolItem *shareToolItem = createToolbarToolItemFromButton(shareButton);

    GtkWidget *hideFolderButton = createButtonForToolbar(&uniHeapMem, "hideFolderButton", "\\icons\\FileViewTaskbar\\File.png", fileBarHeight);
    GtkToolItem *hideFolderToolItem = createToolbarToolItemFromButton(hideFolderButton);

    GtkWidget *moreOptionsButton = createButtonForToolbar(&uniHeapMem, "moreOptionsButton", "\\icons\\FileViewTaskbar\\Menu.png", fileBarHeight);
    GtkToolItem *moreOptionsToolItem = createToolbarToolItemFromButton(moreOptionsButton);

    GtkWidget *detailsButton = createButtonForToolbar(&uniHeapMem, "detailsButton", "\\icons\\FileViewTaskbar\\Menu.png", fileBarHeight);
    GtkToolItem *detailsToolItem = createToolbarToolItemFromButton(detailsButton);


    // Adding the buttons & widgets to the filebar & changing their properties
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(newFileFolderToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(cutToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(copyToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(pasteToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(renameToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(shareToolItem), -1);

    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(filebarSpacer1), -1);
    gtk_tool_item_set_expand(filebarSpacer1, TRUE);

    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(hideFolderToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(moreOptionsToolItem), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(detailsToolItem), -1);

    // Adding the filebar to the window
    gtk_toolbar_set_style(GTK_TOOLBAR(filebar), GTK_TOOLBAR_ICONS);
    gtk_widget_show_all(filebar);

    return filebar;
}