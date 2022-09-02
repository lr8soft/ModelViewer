#include <Windows.h>
#include <commdlg.h>

#include "FileOpenUtil.h"
#include "LogUtil.h"

wchar_t* FileOpenUtil::ConvertCharToLPWSTR(const char * szString)
{
    int dwLen = strlen(szString) + 1;
    int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);
    wchar_t* lpszPath = new wchar_t[dwLen];
    MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
    return lpszPath;
}

bool FileOpenUtil::openNewFile(const char* filePath, const char* title, const char * fileFilter)
{
    OPENFILENAMEA openFileName;
    char szFile[260];
    ZeroMemory(&openFileName, sizeof(openFileName));
    openFileName.lStructSize = sizeof(openFileName);
    openFileName.hwndOwner = NULL;
    openFileName.lpstrFile = szFile;
    openFileName.lpstrFile[0] = '\0';
    openFileName.nMaxFile = sizeof(szFile);
    openFileName.lpstrFilter = fileFilter;
    openFileName.nFilterIndex = 1;
    openFileName.lpstrFileTitle = NULL;
    openFileName.lpstrTitle = title;
    openFileName.nMaxFileTitle = 0;
    openFileName.lpstrInitialDir = NULL;
    openFileName.Flags = 0;
    if (GetOpenFileNameA(&openFileName) == TRUE)
    {
        memcpy_s((void*)filePath, 260, openFileName.lpstrFile, 260);
        return true;
    }
#if _DEBUG
    LogUtil::printInfo("Open file operation canceled.");
#endif
    return false;
}
