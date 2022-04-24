#define _CRT_SECURE_NO_WARNINGS
#define ST_PORT 2000
#define MAX_MESSAGE_LEN 80
#define MAX_THREADS_NUMBER 9
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32")

SOCKET sockets[MAX_THREADS_NUMBER];
int client_counter = 0;

struct thread_data {
	int client_ID;
};

DWORD WINAPI thread_function(void* args) {
	struct thread_data* data = (struct thread_data*)args;
	printf("Client_%d connected.\n", data->client_ID);
	char* greeting;
	char* beg = "SERVER: Welcome, Client_";
	greeting = malloc(strlen(beg) + 2);
	strcpy(greeting, beg);
	greeting[strlen(beg)] = data->client_ID + 0x30;
	greeting[strlen(beg) + 1] = '\0';
	send(sockets[data->client_ID - 1], greeting, strlen(greeting) + 1, 0);
	free(greeting);
	char buf[MAX_MESSAGE_LEN];
	while (recv(sockets[data->client_ID - 1], buf, MAX_MESSAGE_LEN, 0) > 0)
	{
		if (strcmp(buf, "end") == 0)
		{
			closesocket(sockets[data->client_ID - 1]);
			WSACleanup();
			return;
		}

		int private_msg = 0;
		char* message;
		char client_name[] = "Client_x";
		client_name[sizeof(client_name) - 2] = data->client_ID + 0x30;
		const char* decorations = ": ";
		message = (char*)malloc(10 + MAX_MESSAGE_LEN);

		strcpy(message, client_name);
		strcat(message, decorations);
		char* bufptr = buf;
		if (buf[0] == '@') {
			private_msg = buf[1] - 0x30 - 1;
			bufptr += 3;
		}
		strcat(message, bufptr);

		int len;
		if (private_msg != 0) {
			len = strlen(message);
			message[len] = '\0';
			send(sockets[private_msg], message, len + 1, 0);
		}
		else {
			for (int i = 0; i < client_counter; i++) {
				len = strlen(message);
				message[len] = '\0';
				send(sockets[i], message, len + 1, 0);
			}
		}

		printf("%s\n", message);
		free(message);
	}
	printf("Client_%d disconnected.\n", data->client_ID);
}

int main() {
	WSADATA wsas;
	int result;
	WORD version;
	version = MAKEWORD(1, 1);
	result = WSAStartup(version, &wsas);
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sa;
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(ST_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(s, (struct sockaddr FAR*) & sa, sizeof(sa));
	result = listen(s, 5);

	HANDLE threads[MAX_THREADS_NUMBER];
	struct sockaddr_in sc;
	int lenc;
	for (;;)
	{
		lenc = sizeof(sc);
		sockets[client_counter] = accept(s, (struct sockaddr FAR*) & sc, &lenc);
		client_counter++;
		DWORD id;
		struct thread_data* data = (struct thread_data*)malloc(sizeof(struct thread_data));
		data->client_ID = client_counter;
		threads[client_counter - 1] = CreateThread(
			NULL,
			0,
			thread_function,
			(void*)data,
			0,
			&id
		);
	}
	WaitForMultipleObjects(client_counter, threads, 1, INFINITE);
	return 0;
}
