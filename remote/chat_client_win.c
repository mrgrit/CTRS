/*
 * chat_client_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define BUFSIZE 2000
#define NAMESIZE 20

DWORD WINAPI SendMSG(void *arg);
DWORD WINAPI RecvMSG(void *arg);
void ErrorHandling(char *message);

//char name[NAMESIZE]="[Default]";
char message[BUFSIZE];

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET sock;
  SOCKADDR_IN servAddr;
  
  HANDLE hThread1, hThread2;
  DWORD dwThreadID1, dwThreadID2;
/*
  if(argc!=4){
    printf("Usage : %s <IP> <port> <name>\n", argv[0]);
    exit(1);
  }*/
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");
  

  sock=socket(PF_INET, SOCK_STREAM, 0);
  if(sock == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=inet_addr("211.224.130.165");
  servAddr.sin_port=htons(8282);
  
  if(connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error");
  
  hThread1 = (HANDLE)_beginthreadex(NULL, 0, SendMSG, (void*)sock, 0, (unsigned *)&dwThreadID1); 
  hThread2 = (HANDLE)_beginthreadex(NULL, 0, RecvMSG, (void*)sock, 0, (unsigned *)&dwThreadID2);
  if(hThread1==0 || hThread2==0) {
	  ErrorHandling("������ ���� ����");
  }
  
  WaitForSingleObject(hThread1, INFINITE);
  WaitForSingleObject(hThread2, INFINITE);
 
  closesocket(sock);
  return 0;
}

DWORD WINAPI SendMSG(void *arg) // �޽��� ���� ������ ���� �Լ�
{
   SOCKET sock = (SOCKET)arg;
  // char nameMessage[NAMESIZE+BUFSIZE];
   while(1) {
      fgets(message, BUFSIZE, stdin);
     // sprintf(nameMessage,"%s %s", name, message);
      if(!strcmp(message,"q\n")) {  // 'q' �Է½� ����
         closesocket(sock);
         exit(0);		 
      }
      send(sock, message, strlen(message), 0);
   }
}

DWORD WINAPI RecvMSG(void *arg) /* �޽��� ���� ������ ���� �Լ� */
{
  SOCKET sock = (SOCKET)arg;
 // char nameMessage[NAMESIZE+BUFSIZE];
  int strLen;
  while(1){
    strLen = recv(sock, message, BUFSIZE, 0);
	if(strLen==-1) return 1;
	
   // nameMessage[strLen]=0;
    fputs(message, stdout);
  }
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
