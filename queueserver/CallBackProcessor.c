#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <sql.h>
#include <sqlext.h>
//#include <windows.h>



#define BUFSIZE 1000
void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
void ErrorHandling(char *message);
void init_queue(void);


struct _dnode_2
{
	// 두번째 큐 자료형
	int job_id;
	int msgid;
	char *tel; 
	int ret_code;
	int talktime;
	int pages;
	int fee;
	int dtmf;
	char s_time[15];
	char e_time[15];
	char replied_file[60];
	int totalReportCount;
	struct _dnode_2 *prev;
	struct _dnode_2 *next;
};
typedef struct _dnode_2 dnode_2;
dnode_2 *head, *tail;

void put2(char *);
DWORD WINAPI getThread_2(void *);

HANDLE	hThread_2;
DWORD	hID_2;
CRITICAL_SECTION  cs_2,cs_db;
int q_flag_2=1;

/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;
int q_count=0;
int q_count2=0;

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
  henv = hdbc = hstmt = 0;  

	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
	_ret=SQLConnect(hdbc,(UCHAR *)"KARMA",SQL_NTS,(UCHAR *)"karma",SQL_NTS,(UCHAR *)"karmahaja8282",SQL_NTS);
  init_queue();
  InitializeCriticalSection( &cs_2 );
  InitializeCriticalSection( &cs_db );
 // EnterCriticalSection( &cs_2 );
  hThread_2 = (HANDLE)CreateThread(NULL, 0, getThread_2,  (void *)NULL, 0, (unsigned long *)&hID_2);
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
				strLen=recv(hSockArray[index-WSA_WAIT_EVENT_0],message, sizeof(message), 0);
				if(strncmp(message,"submit",6)==0)
				{
				//	puts("쓰레드 락!");
					q_flag_2=1;
				//	EnterCriticalSection(&cs_2);
				}
				else if(strncmp(message,"leave",5)==0)
				{
			//		puts("쓰레드 동기화 해제!");
					q_flag_2=0;
				}else{
					printf("message...........=%s\n",message);
					printf("strlen(message)=%d\n",strlen(message));
					if(strlen(message)==1001) break;
					strtok(message," ");
					put2(message);
					memset(message,0,sizeof(message));
				}
			
			//	send(hSockArray[index-WSA_WAIT_EVENT_0],message, strLen, 0); // 에코 전송
			}// if(netEvents.lNetworkEvents & FD_READ) ENd
 
			if(netEvents.lNetworkEvents & FD_CLOSE) //연결 종료 요청의 경우.
			{
				if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)	{
					puts("Close Error");
					break;
				}
				WSACloseEvent(hEventArray[index]);
				closesocket(hSockArray[index]);
				printf("[%d] 소켓 강제 종료\n", hSockArray[index]);
				
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

void put2(char *message)
{
	char message_copy[BUFSIZE];
	int job_id;
//	char *tel="";
	int ret_code;
	int talktime;
	int pages;
	int fee;
	int dtmf;
	int i;
	char *p; //임시 포인터
	dnode_2 *t;
	if((t=(dnode_2 *)malloc(sizeof(dnode_2)))==NULL)
	{
		puts("Memory can not alloction");
		return -1;
	}
	
	strcpy(message_copy,message);

	
	p=strtok(message_copy,"$");
	job_id=atoi(p);
	p=strtok(NULL,"$");
//	strcpy(tel,p);
	p=strtok(NULL,"$");
	ret_code=atoi(p);
	p=strtok(NULL,"$");
	talktime=atoi(p);
	p=strtok(NULL,"$");
	pages=atoi(p);
	p=strtok(NULL,"$");
	fee=atoi(p);
	p=strtok(NULL,"$");
	dtmf=atoi(p);
	puts("data copy complete!!");


//	strcpy(t->tel,tel);	
//	t->tel = tel;
	t->job_id	=	job_id;
	t->ret_code	=	ret_code;	
	t->talktime	=	talktime;	
	t->pages	=	pages;		
	t->fee		=	fee;		
	t->dtmf		=	dtmf;
	
	tail->prev->next = t;
	t ->prev = tail ->prev;
	tail->prev=t;
	t->next=tail;

	q_count++;
	q_count2++;
	printf("[%d] 번째 자료가 큐에 들어갑니다.\n",q_count2);
	


	//데이터가 처음 들어왔을때 
/*		if (q_flag_2 == 1) {
			
		// 세마포어(1) unlock
	//	strcpy(t->tel,tel);
//			t->tel=tel;
		t->job_id	=	job_id;
		t->ret_code	=	ret_code;
		t->talktime	=	talktime;
		t->pages	=	pages;
		t->fee		=	fee;
		t->dtmf		=	dtmf;

		printf("[Q_2 : 자료가 다시 들어 갑니다.\n");
//		printf("%d, %d, %d, %d, %d, %d, %s\n",	Q_2[ R_2 ].job_id, Q_2[ R_2 ].ret_code,
//				Q_2[ R_2 ].talktime, Q_2[ R_2 ].pages, 	Q_2[ R_2 ].fee, Q_2[ R_2 ].dtmf, 
//				Q_2[ R_2 ].tel);
		q_count++;
		q_flag_2 = 0;
//		LeaveCriticalSection( &cs_2 );
		return;
		
	}
*/
	LeaveCriticalSection(&cs_2);
	return;
}

/////Queue
void init_queue(void)
{

	head=(dnode_2 *)malloc(sizeof(dnode_2));
	tail=(dnode_2 *)malloc(sizeof(dnode_2));
	head -> prev = head;
	head -> next = tail;
	tail -> prev = head;
	tail -> next = tail;
}

//////get_thread2
DWORD WINAPI getThread_2(void *args)
{
	
	int s[6]={0};
//	char *tel="";	
	SQLRETURN res;  
	UCHAR report_bak[1000]="";
	int cnt=10;
	fprintf(stdout, "Thread BLOCK[2]!!\n");
//	EnterCriticalSection( &cs_2 );	// 최초 대기 상태
	fprintf(stdout, "Thread First[2] - Start!! \n");
	
	while (1) {
		dnode_2 *t;
		t=head->next;
		if(t==tail || q_flag_2==1){
				Sleep(1000);
				continue;
				}
			
/*		if (t == tail) {			// 데이타가 없다 
			q_flag_2 = 1;			// put() 함수에게 lock 상태임을 설정 한다.
			// 세미포어(1) lock
			EnterCriticalSection( &cs_2 );
		//	fprintf(stdout, "\nnThread[2] Locking.... \n");
			// 세마포어(1) lock , 두번 lock 연산을 해서 대기 상태로 된다.
			EnterCriticalSection( &cs_2 );
			Sleep(200);
			continue;
            //fprintf(stderr, "get() : Q is Empty \n");
		}/* else {
			F_2 = (F_2 + 1) % MAX_QUE;
			t = &Q_2[F_2];
		}
		*/
			
			

		s[0] = t->job_id;
		s[1] = t->ret_code;
		s[2] = t->talktime;
		s[3] = t->pages;
		s[4] = t->fee;
		s[5] = t->dtmf;
			
//		tel  = t->tel;

		printf("[report CB] 질의 대기중...\n");
//		EnterCriticalSection( &cs_db );
		printf("[report CB] 질의 시작\n");
		th_1=0;
		res=SQLAllocStmt(hdbc,&th_1);
		printf("res1=%d\n",res);
		sprintf((char *)report_bak, "update message_bak2 set ret_code=%d,talktime=%d,page=%d,fee=%d where jobid=%d",s[1],s[2],s[3],s[4],s[0]);
		printf("report_bak=%s\n",report_bak);
		res=SQLPrepare(th_1, report_bak, SQL_NTS);
		printf("res=%d\n",res);
		
//		printf("totalReportCount=%d\n",s[6]);
		while ( (_ret=SQLExecute(th_1) != 0)) {	// Error  이면 다시 데이타 베이스에 저장.
			printf("report_ret=%d\n",_ret);
			fprintf(stderr, "SQL 질의 재실행[2][%d] !!\n", cnt);
			if( (--cnt) < 2 ) break;
			Sleep(40);
		}
		cnt=10;

		SQLFreeStmt(th_1, SQL_DROP); 
		head->next = t->next;
		t ->next ->prev = head;
		free(t);
		q_count--;
		printf("현재 큐에 있는 데이터 : [%d] 개\n",q_count);
	//	LeaveCriticalSection( &cs_db);
	LeaveCriticalSection(&cs_2);
	printf("동기화 해제 \n");
	}//while(1)
//	free(t);
	return 1;
}