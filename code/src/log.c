#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "misc.h"
#include "log.h"

// Getting the current date and time using time.h C library
char* getFormatDateTime() {
  // Getting the current time
  time_t rawTime;
  time(&rawTime);

  // Moving the time object to a structure containing date and time individually
  struct tm *timeInfo = localtime(&rawTime);

  // Formatting the date and time
  char *datetimeBuffer = (char*)malloc(100 * sizeof(char));
  if(datetimeBuffer != NULL) {
    strftime(datetimeBuffer, 100*sizeof(char), "%Y-%m-%d %H:%M:%S", timeInfo);
  }

  return datetimeBuffer;
}

// Function to print a message (and the time) to the logfile
void logMessage(const char *message) {

  // Finding the path of the logfile (relative to the .exe's location)
  char logfileLocation[260];
  appendFileToExecDirWindowsWDoubleBackslash("\\logfile.log", logfileLocation, sizeof(logfileLocation));

  // Opening the file object in append mode --> File path found from .exe location
  FILE *logFile = fopen(logfileLocation, "a"); 
  
  // Checking if a file was returned
  if(logFile != NULL) {
    const char *currentTime = getFormatDateTime();
    fprintf(logFile, "\n[%s]\n%s\n", currentTime, message);
    
    free((void*)currentTime);
  }

  fclose(logFile);
}

