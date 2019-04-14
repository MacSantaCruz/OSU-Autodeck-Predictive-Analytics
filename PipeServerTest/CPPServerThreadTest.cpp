#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <string>


#define BUFFERSIZE 1024

void Read();
void Write();

HANDLE hPipeR, hPipeW;


bool quit = false;

int main() {
	std::vector<std::thread> threads;
	threads.push_back(std::thread(Read));
	threads.push_back(std::thread(Write));

	for (auto& th : threads) th.join();

	return 0;
}


void Read() {

	char buffer[1024];
	DWORD dwRead;
	DWORD dwWritten;
	hPipeR = CreateNamedPipe(TEXT("\\\\.\\pipe\\Py_to_CPP"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		BUFFERSIZE,
		BUFFERSIZE,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (hPipeR != INVALID_HANDLE_VALUE && !quit)
	{
		if (ConnectNamedPipe(hPipeR, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipeR, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				/* do something with data in buffer */
				printf("%s", buffer);

				//WriteFile(hPipe, "MSGRECVD\n", 12, &dwWritten, NULL);
			}
		}

		DisconnectNamedPipe(hPipeR);
	}
	printf("CPP - Read Stream Closed\n");
}

void Write() {
	DWORD dwWritten;

	hPipeW = CreateFile(TEXT("\\\\.\\pipe\\Py_to_CPP"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	while (true && !quit) {
		if (hPipeW != INVALID_HANDLE_VALUE)
		{
			//printf("Write Message\n");
			std::string msg;
			std::getline(std::cin, msg);
			if (msg == "q") {
				quit = true;
				msg = "";
			}

			msg += '\n';

			WriteFile(hPipeW,
				msg.c_str(),//"Hello pipe\n",
				msg.length()+1,//12,   // = length of string + terminating '\0' !!!
				&dwWritten,
				NULL);
		}
	}
	CloseHandle(hPipeW);
	printf("CPP - Write Stream Closed\n");
}