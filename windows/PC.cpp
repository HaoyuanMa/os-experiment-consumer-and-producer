// PC.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	HANDLE SEM_FULL;
	HANDLE SEM_EMPTY;
	HANDLE SEM_MUTEX;
	HANDLE Map;

	SEM_FULL = CreateSemaphore(NULL, 0, 3, (LPCWSTR)"FULL");
	SEM_EMPTY = CreateSemaphore(NULL, 3, 3, (LPCWSTR)"EMPTY");
	SEM_MUTEX = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"MUTEX");

	HANDLE CurrentProcess = GetCurrentProcess();
	Map = CreateFileMapping(CurrentProcess, NULL, PAGE_READWRITE, 0, sizeof(int) * 4, (LPCWSTR)"buffer");

	int *buf = (int*)MapViewOfFile(Map, FILE_MAP_WRITE, 0, 0, sizeof(int) * 4);

	for (int i = 0; i < 4; i++)
		buf[i] = 0;

	WCHAR program[100];
	memset(program, 0, sizeof(program));
	char str1[20] = "producer.exe";
	char str2[20] = "consumer.exe";
	
	PROCESS_INFORMATION  ip[6];
	STARTUPINFO is[6];
	for (int i = 0; i < 6; i++)
	{
		memset(&is[i], 0, sizeof(is[i]));
		is[i].cb = sizeof(is[i]);
		memset(&ip[i], 0, sizeof(ip[i]));
	}
	int t;

	MultiByteToWideChar(CP_ACP, 0, str1, strlen(str1) + 1, program, sizeof(program) / sizeof(program[0]));
	

	t = CreateProcess(program, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &is[1], &ip[1]);
	t = CreateProcess(program, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &is[2], &ip[2]);

	MultiByteToWideChar(CP_ACP, 0, str2, strlen(str2) + 1, program, sizeof(program) / sizeof(program[0]));
	t = CreateProcess(program, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &is[3], &ip[3]);
	t = CreateProcess(program, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &is[4], &ip[4]);
	t = CreateProcess(program, NULL, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &is[5], &ip[5]);

	for (int i = 1; i<=5; i++)
		WaitForSingleObject(ip[i].hProcess, INFINITE);

	for (int i = 1; i <= 5; i++)
		CloseHandle(ip[i].hProcess);

	CloseHandle(SEM_MUTEX);
	CloseHandle(SEM_EMPTY);
	CloseHandle(SEM_FULL);
	CloseHandle(Map);

	getchar();

}

