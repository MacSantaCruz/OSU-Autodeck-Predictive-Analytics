#include <iostream>
#include <Windows.h>
#include <stdio.h>

int main(void)
{
	HANDLE hPipe;
	DWORD dwWritten;


	hPipe = CreateFile(TEXT("\\\\.\\pipe\\CPP_TO_PY_PIPE"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		WriteFile(hPipe,
			"Hello Pipe\n",
			12,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL);

		CloseHandle(hPipe);
	}
	printf("End\n");
	return (0);
}