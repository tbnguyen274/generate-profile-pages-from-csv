#include "HTMLGeneration.h"

UChar* generateEmailFromName(const UChar* name) {
    UErrorCode status = U_ZERO_ERROR;

    if (U_FAILURE(status)) {
        printf("Error generating email: %s\n", u_errorName(status));
    }

    if (u_strlen(name) == 0) {
        printf("Error: name is empty\n");
        return NULL;
    }

    UChar* email = new UChar[MAX_EMAIL + 1];
    if (!email) {
        printf("Error allocating memory for email\n");
        return NULL;
    }

    UChar* nameCopy = new UChar[u_strlen(name) + 1];
    if (!nameCopy) {
        printf("Error allocating memory for nameCopy\n");
        delete[] email;
        return NULL;
    }

    u_strcpy(nameCopy, name);
    if (u_strlen(nameCopy) == 0) {
        printf("Error copying name\n");
        delete[] email;
        delete[] nameCopy;
        return NULL;
    }

    UChar* savePtr;
    // Split the name into words
    UChar* token = u_strtok_r(nameCopy, u" ", &savePtr);
    int index = 0;

    while (token) {
        // Take the first character of each word
        if (index < MAX_EMAIL) {
            email[index++] = u_tolower(*token);
        }
        token = u_strtok_r(NULL, u" ", &savePtr);
    }
    email[index] = u'\0';

    // Reallocate memory for email
    UChar* email1 = new UChar[index + 1];
    if (!email1) {
        printf("Error allocating memory for emailCopy\n");
        delete[] email;
        delete[] nameCopy;
        return NULL;
    }

    // Copy email to email1
    u_strcpy(email1, email);
    if (u_strlen(email1) == 0) {
        printf("Error copying email\n");
        delete[] email;
        delete[] nameCopy;
        delete[] email1;
        return NULL;
    }

    // Free memory
    delete[] nameCopy;
    delete[] email; // delete the old email

    return email1; // return the new email
}

void copyImageToDirectory(const UChar* sourceImage, const UChar* destImage) {
    UErrorCode status = U_ZERO_ERROR;

    UChar* sourceImageTemp = new UChar[u_strlen(sourceImage) + 1];
    UChar* destImageATemp = new UChar[u_strlen(destImage) + 1];

    // Copy not to modify the original string
    u_strcpy(sourceImageTemp, sourceImage);
    u_strcpy(destImageATemp, destImage);

    wchar_t* sourceImageW = new wchar_t[u_strlen(sourceImageTemp) + 1];
    wchar_t* destImageW = new wchar_t[u_strlen(destImageATemp) + 1];

    // Convert UChar* to wchar_t*
    u_strToWCS(sourceImageW, MAX_LEN, NULL, sourceImageTemp, u_strlen(sourceImageTemp), &status);
    u_strToWCS(destImageW, MAX_LEN, NULL, destImageATemp, u_strlen(destImageATemp), &status);

    if (U_FAILURE(status)) {
        printf("Error converting strings: %s\n", u_errorName(status));
        return;
    }

    // CopyFile function of Windows.h
    if (!CopyFileW(sourceImageW, destImageW, FALSE)) {
        printf("Error copying image: %d\n", GetLastError());
    }

    delete[] sourceImageTemp;
    delete[] destImageATemp;
    delete[] sourceImageW;
    delete[] destImageW;
}

void generateHeader(UFILE* file, SinhVien student) {
    u_fprintf_u(file, u"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n");
    u_fprintf_u(file, u"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
    u_fprintf_u(file, u"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
    u_fprintf_u(file, u"  <head>\n");
    u_fprintf_u(file, u"    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
    u_fprintf_u(file, u"    <link rel=\"stylesheet\" type=\"text/css\" href=\"personal.css\" />\n");
    u_fprintf_u(file, u"    <title>HCMUS - %S</title>\n", student.hoTen);
    u_fprintf_u(file, u"  </head>\n");
}

void generatePersonalMainInfo(UFILE* file, SinhVien student, bool* config) {
    UErrorCode status = U_ZERO_ERROR;
    u_fprintf_u(file, u"        <div class=\"Personal_Main_Information\">\n");
    u_fprintf_u(file, u"          <div class=\"Personal_Location\">\n");

    if (config[HOTEN]) {
        UChar* hoTenInHoa = new UChar[u_strlen(student.hoTen) + 1];
        u_strToUpper(hoTenInHoa, 31, student.hoTen, -1, NULL, &status);

        if (config[MSSV]) {
            u_fprintf_u(file, u"            <div class=\"Personal_FullName\">%S - %S</div>\n", hoTenInHoa, student.mssv);
        }
        else {
            u_fprintf_u(file, u"            <div class=\"Personal_FullName\">%S</div>\n", hoTenInHoa);
        }

        delete[] hoTenInHoa;
    }
    else if (config[MSSV]) {
        u_fprintf_u(file, u"            <div class=\"Personal_FullName\">%S</div>\n", student.mssv);
    }

    if (config[KHOA]) {
        UChar* khoaInHoa = new UChar[u_strlen(student.khoa) + 1];
        u_strToUpper(khoaInHoa, 31, student.khoa, -1, NULL, &status);
        u_fprintf_u(file, u"            <div class=\"Personal_Department\">KHOA %S</div>\n", khoaInHoa);
        delete[] khoaInHoa;
    }

    if (config[EMAIL]) {
        u_fprintf_u(file, u"            <br />\n");
        u_fprintf_u(file, u"            <div class=\"Personal_Phone\">\n");
        u_fprintf_u(file, u"              Email: %S@gmail.com\n", student.email);

        u_fprintf_u(file, u"            </div>\n");
    }

    u_fprintf_u(file, u"            <br />\n");
    u_fprintf_u(file, u"            <br />\n");
    u_fprintf_u(file, u"          </div>\n");

    if (config[HINHANH]) {
        u_fprintf_u(file, u"          <div class=\"Personal_HinhcanhanKhung\">\n");

        /*
        In case the image path is absolute, we need to extract the file name
        by the use of u_strrchr to find the last backslash character
        */
        u_fprintf_u(file, u"            <img src=\"Images/%S\" class=\"Personal_Hinhcanhan\" />\n", u_strrchr(student.hinhAnh, u'\\') + 1);

        u_fprintf_u(file, u"          </div>\n");
    }
    u_fprintf_u(file, u"        </div>\n");
    u_fprintf_u(file, u"        <!-- End Below Banner Region -->\n");
}

void generatePersonalInfo(UFILE* file, SinhVien student, bool* config) {
    u_fprintf_u(file, u"            <div class=\"InfoGroup\">Thông tin cá nhân</div>\n");
    u_fprintf_u(file, u"            <div>\n");
    u_fprintf_u(file, u"              <ul class=\"TextInList\">\n");

    if (config[HOTEN]) {
        u_fprintf_u(file, u"                <li>Họ và tên: %S</li>\n", student.hoTen);
    }
    if (config[MSSV]) {
        u_fprintf_u(file, u"                <li>MSSV: %S</li>\n", student.mssv);
    }
    if (config[KHOA]) {
        u_fprintf_u(file, u"                <li>Khoa: %S</li>\n", student.khoa);
    }
    if (config[KHOATUYEN]) {
        u_fprintf_u(file, u"                <li>Khoá tuyển: %d</li>\n", *student.khoaTuyen);
    }
    if (config[NGAYSINH]) {
        u_fprintf_u(file, u"                <li>Ngày sinh: %S</li>\n", student.ngaySinh);
    }
    if (config[EMAIL]) {
        u_fprintf_u(file, u"                <li>Email: %S@gmail.com</li>\n", student.email);
    }
    u_fprintf_u(file, u"              </ul>\n");
    u_fprintf_u(file, u"            </div>\n");
}

void generateHobbies(UFILE* file, SinhVien student, bool* config) {
    // Check if the hobbies are empty
    if (student.demSoThich == 0) {
        return;
    }

    u_fprintf_u(file, u"            <div class=\"InfoGroup\">Sở thích</div>\n");
    u_fprintf_u(file, u"            <div>\n");
    u_fprintf_u(file, u"              <ul class=\"TextInList\">\n");

    for (int i = 0; i < student.demSoThich; i++) {
        if ((u_strcmp(student.soThich[i], u"") != 0) && config[NUMBER_OF_FIELDS + i]) {
            u_fprintf_u(file, u"                <li>%S</li>\n", student.soThich[i]);
        }
    }

    u_fprintf_u(file, u"              </ul>\n");
    u_fprintf_u(file, u"            </div>\n");
}

void generateDescription(UFILE* file, SinhVien student, bool* config) {
    if (u_strcmp(student.moTa, u"") == 0) {
        return;
    }
    u_fprintf_u(file, u"            <div class=\"InfoGroup\">Mô tả</div>\n");
    u_fprintf_u(file, u"            <div class=\"Description\">\n");
    u_fprintf_u(file, u"              %S\n", student.moTa);
    u_fprintf_u(file, u"            </div>\n");
}

void generateBody(UFILE* file, SinhVien student, bool* config) {
    u_fprintf_u(file, u"  <body>\n");
    u_fprintf_u(file, u"    <!-- Begin Layout MainContents Region -->\n");
    u_fprintf_u(file, u"    <div class=\"Layout_container\">\n");
    u_fprintf_u(file, u"      <div class=\"Layout_MainContents\">\n");
    u_fprintf_u(file, u"        <!-- Begin Below Banner Region -->\n");

    if (config[HOTEN] || config[MSSV] || config[HINHANH]) {
        generatePersonalMainInfo(file, student, config);
    }

    u_fprintf_u(file, u"        <!-- Begin MainContents Region -->\n");
    u_fprintf_u(file, u"        <div class=\"MainContain\">\n");
    u_fprintf_u(file, u"          <!-- Begin Top Region -->\n");
    u_fprintf_u(file, u"          <div class=\"MainContain_Top\">\n");

    if (config[HOTEN] || config[MSSV] || config[KHOA] || config[KHOATUYEN] || config[NGAYSINH] || config[EMAIL]) {
        generatePersonalInfo(file, student, config);
    }
    if (student.demSoThich > 0) {
        bool exist = false;
        for (int i = 0; i < student.demSoThich; i++) {
            // check if hobbies config is true
            if (config[NUMBER_OF_FIELDS + i]) {
                exist = true;
                break;
            }
        }
        if (exist) {
            generateHobbies(file, student, config);
        }
    }
    if (config[MOTA]) {
        generateDescription(file, student, config);
    }

    u_fprintf_u(file, u"          </div>\n");
    u_fprintf_u(file, u"        </div>\n");
    u_fprintf_u(file, u"      </div>\n");
}


void generateFooter(UFILE* file) {
    u_fprintf_u(file, u"      <!-- Begin Layout Footer -->\n");
    u_fprintf_u(file, u"      <div class=\"Layout_Footer\">\n");
    u_fprintf_u(file, u"        <div class=\"divCopyright\">\n");
    u_fprintf_u(file, u"          <br />\n");
    u_fprintf_u(file, u"          <br />\n");
    u_fprintf_u(file, u"          @2024</br>\n");
    u_fprintf_u(file, u"          Đồ án giữa kỳ</br>\n");
    u_fprintf_u(file, u"        Kỹ thuật lập trình</br>\n");
    u_fprintf_u(file, u"        TH2024</br>\n");
    u_fprintf_u(file, u"        23120023 - Nguyễn Thái Bảo</br>\n");
    u_fprintf_u(file, u"        </div>\n");
    u_fprintf_u(file, u"      </div>\n");
    u_fprintf_u(file, u"      <!-- End Layout Footer -->\n");
    u_fprintf_u(file, u"    </div>\n");
    u_fprintf_u(file, u"  </body>\n");
    u_fprintf_u(file, u"</html>\n");
}

void generateHTML(SinhVien* students, int count, const UChar* rootDir, const UChar* imagesDir) {
    // generate HTML for each student
    for (int i = 0; i < count; i++) {
        UChar fileName[MAX_LEN];
        u_sprintf_u(fileName, u"%S\\%S.html", rootDir, students[i].mssv);

        UFILE* file = u_fopen_u(fileName, "w", NULL, "UTF-8");
        if (!file) {
            printf("Error opening file");
            continue;
        }

        // Copy image to Images directory
        UChar destImage[MAX_LEN];
        u_sprintf_u(destImage, u"%S\\%S", imagesDir, u_strrchr(students[i].hinhAnh, u'\\') + 1);
        copyImageToDirectory(students[i].hinhAnh, destImage);

        // Assign email to student
        students[i].email = generateEmailFromName(students[i].hoTen);

        // Define an array to hold the configuration
        bool* config;

        // Prompt the user to enter their preferences
        promptUser(config, students[i], i);

        // Generate HTML content
        generateHeader(file, students[i]);
        generateBody(file, students[i], config);
        generateFooter(file);

        // Free memory
        delete[] config;

        u_fclose(file);
    }

    printf("\nProfile pages generated successfully.\n");
}

void promptUser(bool*& config, SinhVien student, int order) {
    printf("\nEnter your preferences for student %d\n", order + 1);
    u_printf_u(u"Student ID: %S\n", student.mssv);

    // Count the number of fields
    int totalFields = NUMBER_OF_FIELDS + student.demSoThich;
    printf("Total fields: %d\n", totalFields);

    // Allocate memory for the configuration array
    config = new bool[totalFields];

    // Prompt the user for their preferences
    printf("0: None\n");
    printf("1: MSSV\n");
    printf("2: Ho va ten\n");
    printf("3: Khoa\n");
    printf("4: Khoa tuyen\n");
    printf("5: Ngay sinh\n");
    printf("6: Hinh anh\n");
    printf("7: Mo ta\n");
    printf("8: Email\n");

    for (int i = 9; i <= totalFields; i++) {
        printf("%d: So thich %d\n", i, i - 8);
    }

    printf("The number of fields you would NOT like to show in this profile page: ");

    int numInvisible = 0;
    int res1 = scanf("%d", &numInvisible);
    while (res1 != 1) {
        printf("Invalid input\n");
        printf("The number of fields you would NOT like to show in this profile page: ");
        // Clear the input buffer
        while (getchar() != '\n');
        res1 = scanf("%d", &numInvisible);
    }

    // Initialize the configuration
    for (int i = 0; i < totalFields; i++) {
        config[i] = true;
    }

    // Prompt the user to enter the fields they want to hide
    for (int i = 0; i < numInvisible; i++) {
        int field;
        printf("Enter field %d: ", i + 1);
        int res2 = scanf("%d", &field);
        if (res2 != 1) {
            printf("Invalid input\n");
            // Clear the input buffer
            while (getchar() != '\n');
            i--;
            continue;
        }

        if (field == 0) {
            break;
        }
        if (field < 0 || field > totalFields) {
            printf("Invalid field\n");
            i--;
            continue;
        }
        config[field - 1] = false;
    }

    // Print the configuration
    printf("Configuration: ");
    for (int i = 0; i < totalFields; i++) {
        printf("%d ", config[i]);
    }
    printf("\n");
}