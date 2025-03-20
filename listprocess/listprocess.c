#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include "info.h"
#define MAX_SIZE 1024

DWORD ListProcess(CHAR *cProcessNeeded) {
	DWORD dwPids[MAX_SIZE];
	DWORD dwNumberOfPids = 0;
	DWORD dwPidsSize = sizeof(dwPids);
	HANDLE hProcess = NULL;
	HMODULE lphModule = NULL;
	DWORD dwCb = sizeof(lphModule);
	DWORD dwCbNeeded = 0;
	CHAR cProcessName[MAX_SIZE];
	SIZE_T nSize = sizeof(cProcessName);

	printf("You choosed process: %s\n", cProcessNeeded);

	if (!EnumProcesses(&dwPids, dwPidsSize, &dwNumberOfPids)) {
		printf("%s EnumProcesses failed with error: 0x%x\n", ERROR, GetLastError());
		return 1;
	}

	for (int i = 0; i < dwNumberOfPids; i++) {
		if (dwPids[i] != 0) {
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, dwPids[i]);
			if (hProcess == NULL) {
				if (GetLastError() == 0x5) {
					printf("%s Access Denied\n", ERROR);
				}
				else {
					printf("%s OpenProcess failed with error: 0x%x\n", ERROR, GetLastError());
				}
				continue;
				//return 1;
			}
			//printf("%s Get handle for process %d at address: 0x%p\n", SUCCESS, dwPids[i], hProcess);

			if (!EnumProcessModules(hProcess, &lphModule, dwCb, &dwCbNeeded)) {
				printf("%s EnumProcessModules failed with error: 0x%x\n", ERROR, GetLastError());
				continue;
				//return 1;
			}

			GetModuleBaseNameA(hProcess, lphModule, &cProcessName, nSize);
			printf("%s Get name of process %d: %s\n", SUCCESS, dwPids[i], cProcessName);

			if (strstr(cProcessName, cProcessNeeded)) {
				printf("%s Process %s active with pid %d\n", SUCCESS, cProcessName, dwPids[i]);
				return dwPids[i];
			}
		}
	}
}
