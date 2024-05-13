#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_WCSTOK

// Define max length for each field
#define MAX_LEN 1001
#define MAX_MSSV 11
#define MAX_HO_TEN 31
#define MAX_KHOA 31
#define MAX_KHOA_TUYEN 5
#define MAX_NGAY_SINH 11
#define MAX_HINH_ANH 1001
#define MAX_MO_TA 1001
#define MAX_EMAIL 31

// Define the indices of each field (used for promptUser function)
#define MSSV 0
#define HOTEN 1
#define KHOA 2
#define KHOATUYEN 3
#define NGAYSINH 4
#define HINHANH 5
#define MOTA 6
#define EMAIL 7
#define NUMBER_OF_FIELDS 8

#ifndef SINHVIEN_H
#define SINHVIEN_H

#include <stdio.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

#include <direct.h> // for _mkdir
#include <io.h> // for _access
#include <windows.h> // for CopyFile

struct SinhVien {
    UChar* mssv;
    UChar* hoTen;
    UChar* khoa;
    int* khoaTuyen;
    UChar* ngaySinh;
    UChar* hinhAnh;
    UChar* moTa;
    UChar* email;
    UChar** soThich;
    int demSoThich;
};

#endif // SINHVIEN_H