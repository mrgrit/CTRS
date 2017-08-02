#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 100
void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
void ErrorHandling(char *message);

int main()
{
  WSADATA wsaData;
  SOCKET hServSock;
  SOCKADDR_IN servAddr;

  SOCKET hSockArray[WSA_MAXIMUM_WAIT_EVENTS]; 
  SOCKET hClntSock;
  int clntLen;
  SOCKADDR_IN clntAddr;

  WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];
  WSAEVENT newEvent;
  WSANETWORKEVENTS netEvents;

  int sockTotal=0;
  int index, i;  
  char message[BUFSIZE];
  int strLen;
  memset(message,0,BUFSIZE);
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hServSock = socket(PF_INET, SOCK_STREAM, 0);
  if(hServSock==INVALID_SOCKET)
	  ErrorHandling("socket() error");

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(8100);

  if(bind(hServSock, (struct sockaddr *) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("bind() error");

  newEvent = WSACreateEvent();
  if(WSAEventSelect(hServSock, newEvent, FD_ACCEPT)==SOCKET_ERROR)
	  ErrorHandling("WSAEventSelect() error");

  if(listen(hServSock, 5)==SOCKET_ERROR)
	  ErrorHandling("listen() error");
  
  hSockArray[sockTotal]=hServSock;
  hEventArray[sockTotal]=newEvent;
  sockTotal++;


  while(1)
  {
	index = WSAWaitForMultipleEvents(sockTotal, hEventArray, FALSE, WSA_INFINITE, FALSE);
	index = index-WSA_WAIT_EVENT_0;

	for(i=index; i<sockTotal; i++)
	{
		index=WSAWaitForMultipleEvents(1, &hEventArray[i], TRUE, 0, FALSE);
		if((index==WSA_WAIT_FAILED || index==WSA_WAIT_TIMEOUT))	continue;
		else
		{
			index=i;
			WSAEnumNetworkEvents(hSockArray[index], hEventArray[index], &netEvents);
			if(netEvents.lNetworkEvents & FD_ACCEPT) //연결 요청의 경우.
			{
				if(netEvents.iErrorCode[FD_ACCEPT_BIT] != 0){
					puts("Accept Error");
					break;
				}

				clntLen = sizeof(clntAddr);
				hClntSock = accept(hSockArray[index], (SOCKADDR*)&clntAddr, &clntLen);
				newEvent=WSACreateEvent();
				WSAEventSelect(hClntSock, newEvent, FD_READ|FD_CLOSE);

				hEventArray[sockTotal]=newEvent;
				hSockArray[sockTotal]=hClntSock;
				sockTotal++;
				printf("새로 연결된 소켓의 핸들 %d \n", hClntSock);
			} //if(NetworkEvents.lNetworkEvents & FD_ACCEPT) ENd

			if(netEvents.lNetworkEvents & FD_READ) //데이터 전송의 경우.
			{
				if(netEvents.iErrorCode[FD_READ_BIT] != 0){
					puts("Read Error");
					break;
				}

				strLen=recv(hSockArray[index-WSA_WAIT_EVENT_0],
					        message, sizeof(message), 0);
				for (i=1000; i >= 0; --i) {
					if (message[i] != ' ') {
					message[i+1] = '\0';
					break;
					}
				}
				printf("message=%s\n",message);
				send(hSockArray[index-WSA_WAIT_EVENT_0],
					 message, strLen, 0); // 에코 전송
			}// if(netEvents.lNetworkEvents & FD_READ) ENd
 
			if(netEvents.lNetworkEvents & FD_CLOSE) //연결 종료 요청의 경우.
			{
				if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)	{
					puts("Close Error");
					break;
				}
				WSACloseEvent(hEventArray[index]);
				closesocket(hSockArray[index]);
				printf("종료 된 소켓의 핸들 %d \n", hSockArray[index]);
				
				sockTotal--;
				CompressSockets(hSockArray, index, sockTotal); //배열 정리.
				CompressEvents(hEventArray, index, sockTotal);
			}// if(netEvents.lNetworkEvents & FD_CLOSE) ENd
		} //else ENd
	} //for(i=index; i<sockTotal; i++) ENd
  } //while(1) ENd

  WSACleanup();
  return 0;
}


void CompressSockets(SOCKET* hSockArray, int omitIndex, int total)
{
	int i;
	for(i=omitIndex; i<total; i++)
		hSockArray[i]=hSockArray[i+1];
}

void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total)
{
	int i;
	for(i=omitIndex; i<total; i++)
		hEventArray[i]=hEventArray[i+1];
}

void ErrorHandling(char *message)
{	
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
