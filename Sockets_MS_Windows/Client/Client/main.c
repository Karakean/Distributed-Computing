#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define ST_PORT 2000
#define MAX_MESSAGE_LEN 80
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32")

DWORD WINAPI send_message(SOCKET* s) {
	int len;
	char buf[MAX_MESSAGE_LEN];
	for (;;)
	{
		fgets(buf, MAX_MESSAGE_LEN, stdin);
		len = strlen(buf);
		buf[len - 1] = '\0';
		send(*s, buf, len, 0);
		if (strcmp(buf, "end") == 0) break;
	}
	return 0;
}

DWORD WINAPI receive_message(SOCKET* s) {
	char buf[MAX_MESSAGE_LEN];
	for (;;)
	{
		recv(*s, buf, MAX_MESSAGE_LEN, 0);
		if (strcmp(buf, "end") == 0)
		{
			closesocket(*(s));
			WSACleanup();
			return;
		}
		printf("%s\n", buf);
	}
	printf("Disconnected from server.\n");
	return 0;
}

int main() {

	SOCKET s;
	struct sockaddr_in sa;
	WSADATA wsas;
	WORD version;
	version = MAKEWORD(2, 0);
	WSAStartup(version, &wsas);
	s = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(ST_PORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int result;
	result = connect(s, (struct sockaddr FAR*) & sa, sizeof(sa));
	if (result == SOCKET_ERROR)
	{
		printf("Connection error\n");
		return;
	}



	HANDLE threads[2];
	DWORD id[2];

	threads[0] = CreateThread(
		NULL,
		0,
		send_message,
		&s,
		0,
		id
	);

	threads[1] = CreateThread(
		NULL,
		0,
		receive_message,
		&s,
		0,
		&id[1]
	);

	WaitForMultipleObjects(2, threads, 1, INFINITE);

	closesocket(s);
	WSACleanup();

	return 0;
}