#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SEND_BUFSIZE 1000	
#define RCEV_BUFSIZE 100000
void ErrorHandling(char *message);

int main()
{
  WSADATA wsaData;
  SOCKET hSocket;
  char send_message[SEND_BUFSIZE];
  char rcev_message[RCEV_BUFSIZE];
  int strLen;

  SOCKADDR_IN servAddr;
char *ip="211.224.130.165";
  int i;

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!"); 

  hSocket=socket(PF_INET, SOCK_STREAM, 0);   
  if(hSocket == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=inet_addr(ip);
  servAddr.sin_port=htons(8100);

  if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
    ErrorHandling("connect() error!");

  while(1) {
     fputs("������ �޽����� �Է� �ϼ��� (q to quit) : ", stdout);
     fgets(send_message, BUFSIZE, stdin); /* ���� �� ������ �ַܼκ��� �Է� */
       if(!strcmp(send_message,"q\n"))
		break;
     send(hSocket, send_message, strlen(send_message), 0); /* �޽��� ���� */
	 strLen=recv(hSocket, rcev_message, BUFSIZE-1, 0); /* �޽��� ���� */
     rcev_message[strLen]=0;
     printf("�����κ��� ���۵� �޽��� : %s \n", rcev_message);
	 
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
