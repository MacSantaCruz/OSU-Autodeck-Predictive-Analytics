#include <iostream>
#include <Windows.h>
#include <stdio.h>


#define BUFFERSIZE 1024

int main() {
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	DWORD dwWritten;

	printf("reads\n");

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\CPP_TO_PY_PIPE"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		BUFFERSIZE,
		BUFFERSIZE,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				/* do something with data in buffer */
				printf("%s", buffer);

				//WriteFile(hPipe, "MSGRECVD\n", 12, &dwWritten, NULL);
			}
		}

		DisconnectNamedPipe(hPipe);
	}
	return 0;
}