#ifndef CSV_HANDLING_H
#define CSV_HANDLING_H

#include "Utils.h"

// get the number of lines in a file
int getNumberOfLines(const UChar* fileName);

// get the maximum length of lines in a file
int getMaxLenOfLines(const UChar* fileName);

// extract a field from a line
void extractField(UChar* token, UChar*& dest, int maxLen);

// count the number of fields in a line
int countFields(UChar* line);

// extract the KhoaTuyen field from a line
void extractKhoaTuyen(UChar* token, SinhVien& student);

// extract all student's information from a line
void extractStudentInfoFromLine(UChar* line, SinhVien*& students, int& count);

// read students from a CSV file
int readCSV(const UChar* fileName, SinhVien* students, int maxStudents);

#endif // CSV_HANDLING_H