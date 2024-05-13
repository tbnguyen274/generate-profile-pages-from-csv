#include "CSVHandling.h"
#include "HTMLGeneration.h"
#include "Utils.h"


int main() {
    // Get CSV directory
    UChar* csvDir = getCSVDirectory();
    if (csvDir == NULL) {
        printf("Error reading CSV directory\n");
        return 1;
    }

    // Get root directory
    UChar* rootDir = getRootDirectory();
    if (rootDir == NULL) {
        printf("Error reading root directory\n");
        return 1;
    }

    // Create Images directory
    UChar* imagesDir = new UChar[MAX_LEN];
    if (!createImagesDirectory(rootDir, imagesDir)) {
        return 1;
    }

    // Read students from CSV file
    int numLines = getNumberOfLines(csvDir);
    printf("The number of lines read from file: %d\n", numLines);

    SinhVien* students = new SinhVien[numLines];
    int numStudents = readCSV(csvDir, students, MAX_LEN);
    if (numStudents == numLines) {
        printf("The total number of students successfully read: %d\n", numStudents);
    }
    else {
        printf("Failed to read all students. Only %d were read\n", numStudents);
        return 1;
    }

    // Generate HTML files
    generateHTML(students, numStudents, rootDir, imagesDir);

    // Copy personal.css file
    copyCSS(rootDir);

    // Free memory
    delete[] rootDir;
    delete[] csvDir;
    delete[] imagesDir;
    freeStudents(students, numStudents);

    return 0;
}