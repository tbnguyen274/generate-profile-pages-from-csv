#ifndef HTML_GENERATION_H
#define HTML_GENERATION_H

#include "Utils.h"

// generate email from name
UChar* generateEmailFromName(const UChar* name);

// copy Images from source to dest directory
void copyImageToDirectory(const UChar* sourceImage, const UChar* destImage);

// generate HTML files for students
void generateHTML(SinhVien* students, int count, const UChar* rootDir, const UChar* imagesDir);

void generateHeader(UFILE* file, SinhVien student);

void generateFooter(UFILE* file);

void generatePersonalInfo(UFILE* file, SinhVien student, bool* config);

void generateHobbies(UFILE* file, SinhVien student, bool* config);

void generateDescription(UFILE* file, SinhVien student, bool* config);

void generateBody(UFILE* file, SinhVien student, bool* config);

// prompt user for configuration
void promptUser(bool*& config, SinhVien student, int order);

#endif // HTML_GENERATION_H