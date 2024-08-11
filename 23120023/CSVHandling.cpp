#include "CSVHandling.h"

int getNumberOfLines(const UChar* fileName) {
    UFILE* file = u_fopen_u(fileName, "r", NULL, "UTF-8");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    int count = 0;
    UChar current;
    UChar previous = 0;

    while ((current = u_fgetc(file)) != U_EOF) {
        // when encountering a new line character, count++
        if (current == '\n') {
            count++;
        }
        previous = current;
    }

    // Check the last line if it doesn't end with \n
    if (previous != '\n' && previous != U_EOF) {
        count++;
    }

    u_fclose(file);
    return count;
}


int getMaxLenOfLines(const UChar* fileName) {
    UFILE* file = u_fopen_u(fileName, "r", NULL, "UTF-8");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    int maxLen = 0;
    int len = 0;
    UChar buffer;

    while ((buffer = u_fgetc(file)) != U_EOF) {
        len++;
        if (buffer == '\n') {
            if (len > maxLen) {
                maxLen = len;
            }
            len = 0;
        }
    }

    // Check the last line if it doesn't end with \n
    if (len > maxLen) {
        maxLen = len;
    }

    u_fclose(file);
    return maxLen;
}

void extractField(UChar* token, UChar*& dest, int maxLen) {
    int len = u_strlen(token);
    if (len == 0) {
        dest = NULL;
        return;
    }
    if (len > maxLen) {
        printf("The length of the field exceeds the maximum length.\n");
        len = maxLen;
    }

    dest = new UChar[len + 1];

    // Copy the token to the destination
    UChar* src = token;
    UChar* dst = dest;
    while (*src != u'\0') {
        if (*src == u'"' && *(src + 1) == u'"') {
            *dst++ = *src++;
            src++;  // Skip the second quote
        }
        else {
            *dst++ = *src++;
        }
    }
    *dst = u'\0';
}

void extractKhoaTuyen(UChar* token, SinhVien& student) {
    char buffer[MAX_KHOA_TUYEN];
    u_austrcpy(buffer, token);
    student.khoaTuyen = new int;
    *student.khoaTuyen = _atoi_l(buffer, NULL);
}

int countFields(UChar* line) {
    int count = 0;
    UChar* current = line;
    bool inQuotes = false;

    // Count the number of fields
    while (*current != u'\0') {
        if (*current == u'"') {
            inQuotes = !inQuotes;   // Change the state of inQuotes: true -> false, false -> true
        }
        // If the current char is a comma and not in quotes, count++
        else if (*current == u',' && !inQuotes) {
            count++;
        }
        current++;
    }

    // Add one more to count because the number of fields is one more than the number of commas
    if (*current == u'\0' && *(current - 1) != u',') {
        count++;
    }

    return count;
}


void extractStudentInfoFromLine(UChar* line, SinhVien*& students, int& count) {
    // Count the total number of fields
    int totalFields = countFields(line);
    if (-1 == totalFields) {
        return;
    }

    if (totalFields < 7) {
	printf("The number of fields is not enough: %d\n", totalFields);
        printf("From line number: %d\n", count + 1);
		return;
	}

    students[count].demSoThich = totalFields - 7;
    if (students[count].demSoThich > 0) {
        students[count].soThich = new UChar * [students[count].demSoThich];
    }
    else {
        students[count].soThich = NULL;
    }

    UChar* start = line;
    UChar* end = line;
    bool inQuotes = false;
    int fieldCount = 0;

    while (*end != u'\0') {
        if (*end == u'"') {
            inQuotes = !inQuotes;
        }
        else if (*end == u',' && !inQuotes) {
            *end = u'\0';
            UChar* token = trimSpacesAndQuotes(start);

            switch (fieldCount) {
            case 0: extractField(token, students[count].mssv, MAX_MSSV); break;
            case 1: extractField(token, students[count].hoTen, MAX_HO_TEN); break;
            case 2: extractField(token, students[count].khoa, MAX_KHOA); break;
            case 3: extractKhoaTuyen(token, students[count]); break;
            case 4: extractField(token, students[count].ngaySinh, MAX_NGAY_SINH); break;
            case 5: extractField(token, students[count].hinhAnh, MAX_HINH_ANH); break;
            case 6: extractField(token, students[count].moTa, MAX_MO_TA); break;
            default:
                if (fieldCount - 7 < students[count].demSoThich) {
                    extractField(token, students[count].soThich[fieldCount - 7], MAX_LEN);
                }
                break;
            }

            start = end + 1;
            fieldCount++;
        }
        end++;
    }

    // Extract the last field
    UChar* token = trimSpacesAndQuotes(start);

    // SoThich is optional, so the last field can onl be moTa or soThich
    if (fieldCount == 6) {
        extractField(token, students[count].moTa, MAX_MO_TA);
    }
    else if (students[count].soThich != NULL) {
        extractField(token, students[count].soThich[fieldCount - 7], MAX_LEN);
    }

    count++;
}

int readCSV(const UChar* fileName, SinhVien* students, int maxStudents) {
    UErrorCode status = U_ZERO_ERROR;

    // Set locale to en_US_POSIX so that numbers are formatted correctly
    uloc_setDefault("en_US_POSIX", &status);
    if (U_FAILURE(status)) {
        printf("Error setting locale: %s\n", u_errorName(status));
        return -1;
    }

    UFILE* file = u_fopen_u(fileName, "r", NULL, "UTF-8");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    int count = 0;
    int lenMaxLine = getMaxLenOfLines(fileName);
    if (lenMaxLine == -1) {
        u_fclose(file);
        return -1;
    }
    UChar* line = new UChar[lenMaxLine + 1];

    while (u_fgets(line, lenMaxLine + 1, file)) {
        if (count >= maxStudents) {
            printf("Exceed maximum number of students.\n");
            break;
        }

        if (line == NULL || *line == u'\n' || *line == u'\0') {
            continue;
        }

        // Extract information from line
        extractStudentInfoFromLine(line, students, count);
    }

    delete[] line;
    u_fclose(file);

    return count;
}
