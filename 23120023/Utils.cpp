#include "Utils.h"

UChar* trimSpacesAndQuotes(UChar* str) {
    UChar* start = str;
    UChar* end = str + u_strlen(str) - 1;

    // Skip BOM if present at the start of the string
    if (*start == 0xFEFF) {
        // use memmove to shift the content of the string to the left by 1
        // therefore avoid changing the address of the string
        memmove(start, start + 1, (end - start + 1) * sizeof(UChar));
        end--;
    }

    // Skip leading spaces
    while (*start == u' ') {
        memmove(start, start + 1, (end - start + 1) * sizeof(UChar));
        end--;
    }

    // Trim trailing spaces and newline characters
    while (*end == u' ' || *end == u'\n') {
        *end-- = u'\0';
    }

    // Trim trailing quotes
    if (*start == u'"' && *end == u'"') {
        memmove(start, start + 1, (end - start) * sizeof(UChar));
        *(end - 1) = u'\0';
    }

    return str;
}

UChar* getRootDirectory() {
    printf("Enter the root directory: ");
    UChar* rootDir = new UChar[MAX_LEN];
    UFILE* uStdin = u_finit(stdin, NULL, NULL);
    if (u_fgets(rootDir, MAX_LEN, uStdin) != NULL) {
        int len = u_strlen(rootDir);
        if (len > 0 && rootDir[len - 1] == '\n') {
            rootDir[len - 1] = 0; // remove newline character
        }
        rootDir = trimSpacesAndQuotes(rootDir);
        if (u_strlen(rootDir) == 0) {
            printf("Error: Empty input\n");
            delete[] rootDir;
            return NULL;
        }
        return rootDir;
    }
    else {
        printf("Error reading input\n");
        delete[] rootDir;
        return NULL;
    }
}

UChar* getCSVDirectory() {
    printf("Enter the directory of the CSV file: ");
    UChar* csvDir = new UChar[MAX_LEN];
    UFILE* uStdin = u_finit(stdin, NULL, NULL);
    if (u_fgets(csvDir, MAX_LEN, uStdin) != NULL) {
        int len = u_strlen(csvDir);
        if (len > 0 && csvDir[len - 1] == '\n') {
            csvDir[len - 1] = 0; // remove newline character
        }
        csvDir = trimSpacesAndQuotes(csvDir);
        if (u_strlen(csvDir) == 0) {
            printf("Error: Empty input\n");
            delete[] csvDir;
            return NULL;
        }
        return csvDir;
    }
    else {
        printf("Error reading input\n");
        delete[] csvDir;
        return NULL;
    }
}

bool createImagesDirectory(UChar* rootDir, UChar* imagesDir) {
    UErrorCode status = U_ZERO_ERROR;

    u_sprintf_u(imagesDir, u"%S\\Images", rootDir);

    wchar_t* imagesDirW = new wchar_t[MAX_LEN];
    u_strToWCS(imagesDirW, MAX_LEN, NULL, imagesDir, u_strlen(imagesDir), &status);

    if (_waccess(imagesDirW, 0) == -1) {
        int result = _wmkdir(imagesDirW);
        if (result == 0) {
            printf("Created Images directory.\n");
            return true;
        }
        else {
            printf("Failed to create Images directory.\n");
            return false;
        }
    }
    else {
        printf("Images directory already exists.\n");
        return true;
    }

    delete[] imagesDirW;
}

void copyCSS(const UChar* rootDir) {
    UFILE* source, * dest;
    UChar buffer;

    // Open source file
    source = u_fopen("personal.css", "r", NULL, "UTF-8");
    if (source == NULL) {
        printf("Unable to open source file.\n");
        return;
    }

    // Prepare destination file path
    UChar* destPath = new UChar[MAX_LEN];
    u_sprintf_u(destPath, u"%S\\personal.css", rootDir);

    // Open destination file
    dest = u_fopen_u(destPath, "w", NULL, "UTF-8");
    if (dest == NULL) {
        printf("Unable to open destination file.\n");
        u_fclose(source);
        return;
    }

    // Copy file content
    while ((buffer = u_fgetc(source)) != U_EOF) {
        u_fputc(buffer, dest);
    }

    // Close files
    u_fclose(source);
    u_fclose(dest);

    delete[] destPath;
}

void freeStudents(SinhVien* students, int numStudents) {
    for (int i = 0; i < numStudents; i++) {
        delete[] students[i].mssv;
        delete[] students[i].hoTen;
        delete[] students[i].khoa;
        delete students[i].khoaTuyen;
        delete[] students[i].ngaySinh;
        delete[] students[i].hinhAnh;
        delete[] students[i].moTa;
        delete[] students[i].email;
        for (int j = 0; j < students[i].demSoThich; j++) {
            delete[] students[i].soThich[j];
        }
        delete[] students[i].soThich;
    }
    delete[] students;
}