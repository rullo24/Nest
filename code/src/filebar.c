#include <gtk/gtk.h>
#include "filebar.h"
#include "colours.h"
#include "misc.h"
#include "toolbar.h"

GtkWidget *createFilebar(GtkWidget *mainWindow, GtkCssProvider *mainCssProvider, int fileBarHeight) {
    // Creating the filebar
    GtkWidget *filebar = gtk_toolbar_new();
    gtk_container_set_border_width(GTK_CONTAINER(filebar), 5);

    // Adding all filebar items
    GtkToolItem *filebarSpacer1 = gtk_separator_tool_item_new();

    GtkToolItem *newFileFolderButton = createToolbarButton(mainCssProvider, "newFileFolderButton", "\\icons\\FileViewTaskbar\\CirclePlus.png", fileBarHeight);
    GtkToolItem *cutButton = createToolbarButton(mainCssProvider, "cutButton", "\\icons\\FileViewTaskbar\\CutFile.png", fileBarHeight);
    GtkToolItem *copyButton = createToolbarButton(mainCssProvider, "copyButton", "\\icons\\FileViewTaskbar\\CopyFile.png", fileBarHeight);
    GtkToolItem *pasteButton = createToolbarButton(mainCssProvider, "pasteButton", "\\icons\\FileViewTaskbar\\PasteFile.png", fileBarHeight);
    GtkToolItem *renameButton = createToolbarButton(mainCssProvider, "renameButton", "\\icons\\FileViewTaskbar\\RenameFile.png", fileBarHeight);
    GtkToolItem *shareButton = createToolbarButton(mainCssProvider, "shareButton", "\\icons\\FileViewTaskbar\\ShareFile.png", fileBarHeight);

    GtkToolItem *hideFolderButton = createToolbarButton(mainCssProvider, "hideFolderButton", "\\icons\\FileViewTaskbar\\File.png", fileBarHeight);
    GtkToolItem *moreOptionsButton = createToolbarButton(mainCssProvider, "moreOptionsButton", "\\icons\\FileViewTaskbar\\Menu.png", fileBarHeight);
    GtkToolItem *detailsButton = createToolbarButton(mainCssProvider, "detailsButton", "\\icons\\FileViewTaskbar\\Menu.png", fileBarHeight);

    // Adding the buttons & widgets to the filebar & changing their properties
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(newFileFolderButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(cutButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(copyButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(pasteButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(renameButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(shareButton), -1);

    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(filebarSpacer1), -1);
    gtk_tool_item_set_expand(filebarSpacer1, TRUE);

    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(hideFolderButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(moreOptionsButton), -1);
    gtk_toolbar_insert(GTK_TOOLBAR(filebar), GTK_TOOL_ITEM(detailsButton), -1);

    // Adding the filebar to the window
    gtk_toolbar_set_style(GTK_TOOLBAR(filebar), GTK_TOOLBAR_ICONS);
    gtk_widget_show_all(filebar);

    return filebar;
}