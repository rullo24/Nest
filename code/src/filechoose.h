#ifndef FILECHOOSE_H
#define FILECHOOSE_H
#include <stdbool.h>

bool removeAllListBoxRows(GtkWidget *fileListBox);
void getCurrDirFiles(char *directoryString);

#endif