
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>


#define BUFSIZE 30
void ErrorHandling(char *message);
SOCKET hSocket2;
WSADATA wsaData;
char message[10];
SOCKADDR_IN servAddr;
int main()
{


	WinExec("CallBackProcessor.exe",SW_SHOWNORMAL);
	Sleep(2000);
	WinExec("MessageProcessor.exe",SW_SHOWNORMAL);

	  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
			ErrorHandling("WSAStartup() error!"); 

	  hSocket2=socket(PF_INET, SOCK_STREAM, 0);   
	  if(hSocket2 == INVALID_SOCKET)
		ErrorHandling("socket() error");

	  memset(&servAddr, 0, sizeof(servAddr));
	  servAddr.sin_family=AF_INET;
	  servAddr.sin_addr.s_addr=inet_addr("211.224.130.165");
	  servAddr.sin_port=htons(8100);
	  if(connect(hSocket2, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error!");
	
	while(1)
	{
		puts("1 : ¿Ë³àÀÇ Å¥ ¾÷µ¥ÀÌÆ®ÇÏ±â.");
		puts("2 : ¿Ë³à ¾÷µ¥ÀÌÆ® ±×¸¸ÇÏ±â.");
		fputs("¿Ë³à¿¡°Ô ³»¸± ¸í·É (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strncmp(message,"1",1))	send(hSocket2,"leave",5,0);
		if(!strncmp(message,"2",1))	send(hSocket2,"submit",6,0);
//		if(!strncmp(message,"1",1)) WinExec("º¯°­¼è.exe",SW_SHOWNORMAL);
//		if(!strncmp(message,"1",1)) WinExec("¿Ë³à.exe",SW_SHOWNORMAL);
		if(!strcmp(message,"q\n"))	break;
	//	Sleep(1000);
	}

	closesocket(hSocket2);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}