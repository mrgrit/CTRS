#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 100000
void ErrorHandling(char *message);

int main()
{
	WSADATA  wsaData;
	SOCKET   hSocket;
	char message[30];
	int strLen;
	char *ip="211.224.130.165";
	//char *port="8100"

	SOCKADDR_IN servAddr;
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	
	hSocket=socket(PF_INET, SOCK_DGRAM, 0);
	if(hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(ip);
	servAddr.sin_port=htons(8100);
	
	if(connect(hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		ErrorHandling("connect() error!");
	
	while(1)
	{
		fputs("전송할 메시지를 입력 하세요 (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		
		if(!strcmp(message,"q\n"))	break;
		send(hSocket, message, strlen(message), 0);
		
		strLen=recv(hSocket, message, sizeof(message)-1, 0);
		message[strLen]=0;
		printf("서버로부터 전송된 메시지 : %s", message);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}