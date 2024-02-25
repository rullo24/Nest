#include "locations.h"
#include <windows.h>
#include <shlobj.h>
#include <knownfolders.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include "cStructs.h"

// Creating a function that saves the known locations of user libraries to the uniHeapMem ptr --> Doesn't need error checking as sometimes these folders don't exist on a machine
void saveLibraryLocationsToUniPtr(PROGRAMHEAPMEM **ptr_uniHeapMem) {
    // Creating an alias for the double pointer
    PROGRAMHEAPMEM *uniHeapMem = *ptr_uniHeapMem;

    // Creating ptr variables to store the paths of all common user libraries
    // NOTE: PWSTR type represents a 16-bit, "wide-char" pointer > used for non-ASCII chars
    PWSTR desktopPath = NULL;
    PWSTR documentsPath = NULL;
    PWSTR downloadsPath = NULL;
    PWSTR musicPath = NULL;
    PWSTR picturesPath = NULL;
    PWSTR videosPath = NULL;

    // Retrieving the paths using the windows API
    HRESULT resultDesktop = SHGetKnownFolderPath(&FOLDERID_Desktop, 0, NULL, &desktopPath);
    HRESULT resultDocuments = SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &documentsPath);
    HRESULT resultDownloads = SHGetKnownFolderPath(&FOLDERID_Downloads, 0, NULL, &downloadsPath);
    HRESULT resultMusic = SHGetKnownFolderPath(&FOLDERID_Music, 0, NULL, &musicPath);
    HRESULT resultPictures = SHGetKnownFolderPath(&FOLDERID_Pictures, 0, NULL, &picturesPath);
    HRESULT resultVideos = SHGetKnownFolderPath(&FOLDERID_Videos, 0, NULL, &videosPath);

    // Check if all paths were successfully retrieved -> Convert wide char strings to narrow char strings
    if (SUCCEEDED(resultDesktop)) {
        wcstombs(uniHeapMem->defaultLocations->desktopLoc, desktopPath, MAX_PATH);
    } 
    if (SUCCEEDED(resultDocuments)) {
        wcstombs(uniHeapMem->defaultLocations->documentsLoc, documentsPath, MAX_PATH);
    } 
    if (SUCCEEDED(resultDownloads)) {
        wcstombs(uniHeapMem->defaultLocations->downloadsLoc, downloadsPath, MAX_PATH);
    } 
    if (SUCCEEDED(resultMusic)) {
        wcstombs(uniHeapMem->defaultLocations->musicLoc, musicPath, MAX_PATH);
    } 
    if (SUCCEEDED(resultPictures)) {
        wcstombs(uniHeapMem->defaultLocations->picturesLoc, picturesPath, MAX_PATH);
    } 
    if (SUCCEEDED(resultVideos)) {
        wcstombs(uniHeapMem->defaultLocations->videosLoc, videosPath, MAX_PATH);
    } 

    // Now you have the narrow char strings, you can use them as needed

    // Free the allocated memory for the wide char strings
    CoTaskMemFree(desktopPath);
    CoTaskMemFree(documentsPath);
    CoTaskMemFree(downloadsPath);
    CoTaskMemFree(musicPath);
    CoTaskMemFree(picturesPath);
    CoTaskMemFree(videosPath);
}