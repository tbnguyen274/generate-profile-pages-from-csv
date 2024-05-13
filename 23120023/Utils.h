#ifndef UTILS_H
#define UTILS_H

#include "SinhVien.h"

// trim spaces and quotes from a string
UChar* trimSpacesAndQuotes(UChar* str);

// get the root directory from the user
UChar* getRootDirectory();

// get the directory of the CSV file from the user
UChar* getCSVDirectory();

// create the images directory
bool createImagesDirectory(UChar* rootDir, UChar* imagesDir);

// copy the personal.css file to the root directory
void copyCSS(const UChar* rootDir);

// free memory
void freeStudents(SinhVien* students, int numStudents);

#endif // UTILS_H