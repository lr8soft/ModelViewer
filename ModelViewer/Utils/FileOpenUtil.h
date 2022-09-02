#pragma once
#ifndef _FILE_OPEN_UTIL_H_
#define _FILE_OPEN_UTIL_H_
#include <string>

class FileOpenUtil {
public:
    static wchar_t* ConvertCharToLPWSTR(const char * szString);
    static bool openNewFile(const char* filePath, const char* title = NULL, const char* fileFilter = "All Files(*.*)\0*.*\0\0");
};

#endif