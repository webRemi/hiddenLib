#include <Windows.h>
#include <stdio.h>
#include "info.h"
#include "fetchfile.h"

FileData fetchFile(LPCSTR lpFileName) {
	HANDLE hFile = NULL;
	FileData fileData = { NULL, 0 };

	hFile = CreateFileA(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("%s CreateFile failed with error: 0x%x\n", ERROR, GetLastError());
		exit(1);
	}
	printf("%s Opened file successfully\n", SUCCESS);

	fileData.sContentSize = GetFileSize(hFile, NULL);
	if (fileData.sContentSize == INVALID_FILE_SIZE) {
		printf("%s GetFileSize failed with error: 0x%x\n", ERROR, GetLastError());
		exit(1);
	}
	printf("%s FileSize is: %zu\n", INFO, fileData.sContentSize);

	fileData.cContent = (char*)malloc(fileData.sContentSize + 1);
	if (fileData.cContent == NULL) {
		printf("%s Malloc failed with error: 0x%x\n", ERROR, GetLastError());
		exit(1);
	}
	printf("%s Successfully allocated %zu bytes of memory\n", INFO, fileData.sContentSize + 1);

	DWORD dwBytesRead;
	if (!ReadFile(hFile, fileData.cContent, fileData.sContentSize, &dwBytesRead, NULL)) {
		printf("%s ReadFile failed with error: 0x%x\n", ERROR, GetLastError());
		exit(1);
	}

	if (!CloseHandle(hFile)) {
		printf("%s CloseHandle failed with error: 0x%x\n", ERROR, GetLastError());
		exit(1);
	}
	fileData.cContent[fileData.sContentSize] = '\0';
	return fileData;
}