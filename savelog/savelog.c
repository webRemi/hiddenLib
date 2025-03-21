#include <Windows.h>
#include <stdio.h>
#include "info.h"

BOOL SaveLog(LPCSTR lpFileName, CHAR* cBuffer, SIZE_T sBufferSize) {
    HANDLE hFile = NULL;
    DWORD dwBytesWritten;
    CHAR* cSeparatorStart = "=====START=====\n";
    CHAR* cSeparatorEnd = "\n======END======\n";
    SIZE_T sSeparatorStartSize = strlen(cSeparatorStart);
    SIZE_T sSeparatorEndSize = strlen(cSeparatorEnd);


    printf("%s Writing to a file\n", INFO);
    hFile = CreateFileA(lpFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("%s CreateFile failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }

    SetFilePointer(hFile, 0, NULL, FILE_END);

    if (!WriteFile(hFile, cSeparatorStart, sSeparatorStartSize, &dwBytesWritten, NULL)) {
        printf("%s WriteFile failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }
    if (!WriteFile(hFile, cBuffer, sBufferSize, &dwBytesWritten, NULL)) {
        printf("%s WriteFile failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }
    if (!WriteFile(hFile, cSeparatorEnd, sSeparatorEndSize, &dwBytesWritten, NULL)) {
        printf("%s WriteFile failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }

    CloseHandle(hFile);

    return TRUE;
}
