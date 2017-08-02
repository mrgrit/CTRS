#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
#include <winsock2.h>
#include <sql.h>
#include <sqlext.h>

#define BUFSIZE 1024
#define RESULT_BUFFER 100000

void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
void ErrorHandling(char *message);
int query_exe(char *);
char *select_query(char *);
int field_count(char *);
int row_count(char *);
char *db_error(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);

/***********************************/
/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;
/***********************************/


int type_int[20];
char type_char[20][1000];
int count_int;
int count_char;
SDWORD type_int_len=SQL_NTS;
SDWORD type_char_len=SQL_NTS;


int main(int argc, char **argv)
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
  memset(message,0,BUFSIZE);
  char err_msg[30]="올바른 쿼리문이 아닙니다.";
  char *err_msg_ptr=err_msg;
  
  int strLen;
  char *select_return="";//select 쿼리 결과
  char return_temp[2]=""; //insert,update쿼리 결과 
  char *return_temp_ptr="";

  int query_return;
  
/********************************************************************************************************/
// 초기화 - MS SQL
henv = hdbc = hstmt = 0;  

// MS SQL 에 접속을 시도 한다
SQLAllocEnv(&henv);
SQLAllocConnect(henv,&hdbc);
_ret=SQLConnect(hdbc,(UCHAR *)"KARMA",SQL_NTS,(UCHAR *)"karma",SQL_NTS,(UCHAR *)"karmahaja8282",SQL_NTS);

UCHAR use[20];
hstmt=0;
_ret=SQLAllocStmt(hdbc,&hstmt);
{
sprintf((char *)use,"use external");
_ret = SQLPrepare(hstmt,use,SQL_NTS);
_ret = SQLExecute(hstmt);
printf("use=%d",_ret);
_ret = SQLFreeStmt(hstmt,SQL_DROP);
}
/********************************************************************************************************/
  puts("connect ok");
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hServSock = socket(PF_INET, SOCK_STREAM, 0);
  if(hServSock==INVALID_SOCKET)
	  ErrorHandling("socket() error");

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(atoi("8100"));

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
		if((index==WSA_WAIT_FAILED || index==WSA_WAIT_TIMEOUT))
		{
			puts("if((index==WSA_WAIT_FAILED || index==WSA_WAIT_TIMEOUT))");
			continue;
		}else{
			index=i;
			WSAEnumNetworkEvents(hSockArray[index], hEventArray[index], &netEvents);
			if(netEvents.lNetworkEvents & FD_ACCEPT) //연결 요청의 경우.
			{
				puts("connect request");
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
								
				/*****************************************************************************************/
				//데이터 주고받는 부분
				strLen=recv(hSockArray[index-WSA_WAIT_EVENT_0],message, sizeof(message), 0);
				printf("message=%s\n",message);
				printf("strLen=%d\n",strLen);
				if(strncmp(message,"select",6)==0)
				{
					message[strLen+1]='\0';
					select_return=select_query(message);
					send(hSockArray[index-WSA_WAIT_EVENT_0],select_return,strlen(select_return), 0);
					memset(select_return,0,sizeof(select_return));
				}else if(strncmp(message,"insert",6)==0) 
				{
					message[strLen+1]='\0';
					query_return=query_exe(message);
					itoa(query_return,return_temp,10);
					return_temp_ptr=return_temp;
					printf(return_temp_ptr);
					send(hSockArray[index-WSA_WAIT_EVENT_0],return_temp_ptr,1,0);
				}else if(strncmp(message,"update",6)==0) 
				{
					message[strLen+1]='\0';
					query_return=query_exe(message);
					itoa(query_return,return_temp,10);
					return_temp_ptr=return_temp;
					printf(return_temp_ptr);
					send(hSockArray[index-WSA_WAIT_EVENT_0],return_temp_ptr,1,0);
				}else if(strncmp(message,"delete",6)==0) 
				{
					message[strLen+1]='\0';
					query_return=query_exe(message);
					itoa(query_return,return_temp,10);
					return_temp_ptr=return_temp;
					printf(return_temp_ptr);
					send(hSockArray[index-WSA_WAIT_EVENT_0],return_temp_ptr,1,0);
				}
				else if(strncmp(message,"use",3)==0) 
				{
					message[strLen+1]='\0';
					query_return=query_exe(message);
					itoa(query_return,return_temp,10);
					return_temp_ptr=return_temp;
					printf(return_temp_ptr);
					send(hSockArray[index-WSA_WAIT_EVENT_0],return_temp_ptr,1,0);
				}
				/*else if(strncmp(message,"file",4)==0)
				{

				*/
				else{
					send(hSockArray[index-WSA_WAIT_EVENT_0],err_msg_ptr,strlen(err_msg_ptr),0);
				}
				/*****************************************************************************************/
				memset(message,0,sizeof(message));
				printf("after message=%s\n",message);
			}// if(netEvents.lNetworkEvents & FD_READ) ENd
 
			if(netEvents.lNetworkEvents & FD_CLOSE) //연결 종료 요청의 경우.
			{
				if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)	{
					printf("%d 번 소켓 강제 종료\n",hSockArray[index]);
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
int query_exe(char *query)
{
	int exe_ret=-1;
	hstmt=0;
	char *db_err_msg;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	_ret=SQLPrepare(hstmt,(UCHAR *)query,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	if(_ret==SQL_ERROR)	
	{
		db_err_msg=db_error(henv, hdbc, hstmt);
		return exe_ret;
	}
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
	return exe_ret;
}

char *select_query(char *query)
{
	char result[RESULT_BUFFER];//쿼리 질의결과 
	memset(result,0,1000);
	char result_set[40];//int와 char의 순서를 저장하는 변수
	int i;
	long cbValue;
	short num_row=0;
	int n=0;//임시 변수
	char *db_err_msg;
	char not_found[15]="data not found";
	char *not_found_ptr=not_found;
	
	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
	SQLPrepare(hstmt,(UCHAR *)query,SQL_NTS);
	SQLNumResultCols ( hstmt,&num_row );
	printf("num_row=%d\n",num_row);
	
	
	char *field[40]; //필드 이름의 집합
	char itoa_temp[15]="";
	char *itoa_temp_ptr;

	count_int=0; // int형 변수의 갯수.
	count_char=0;// char형 변수의 갯수.
		
	for(i=0;i<=num_row-1;i++)
	{
		 SQLColAttributes(hstmt,i+1,SQL_DESC_TYPE,NULL, 0, NULL,&cbValue);
		 if(cbValue==4 || cbValue==-6)
		 {	
			SQLBindCol(hstmt,i+1,SQL_C_LONG,(PTR) &type_int[count_int],(SDWORD)4, &type_int_len);
			count_int++;
			result_set[i]='i';
		 }else if(cbValue==1 || cbValue==12){
			SQLBindCol(hstmt,i+1,SQL_C_CHAR,(PTR) type_char[count_char],(SDWORD)sizeof(type_char[count_char]),&type_char_len);
			count_char++;
			result_set[i]='c';
		 }
	
	}

	result_set[i+1]=0;

	_ret=SQLExecute(hstmt);
	if(_ret==SQL_ERROR)	
	{
		db_err_msg=db_error(henv, hdbc, hstmt);
		return db_err_msg;
	}

	while(1)
	{	
		int count_int2=0;
		int count_char2=0;
		int len_type_char=0;

		_ret=SQLFetch(hstmt);

		if(_ret!=SQL_SUCCESS) break;
		if(_ret==100)
		{	
			puts("NOt POUNd");
			return not_found_ptr;
		}
		for(i=0;i<=num_row-1;i++)
		{
			if(result_set[i]=='i')
			{
				itoa(type_int[count_int2],itoa_temp,10);
				itoa_temp_ptr=itoa_temp;
				field[i]=itoa_temp_ptr;
				strtok(field[i]," ");
				count_int2++;
				strncat(field[i],"^",1);
				strncat(result,field[i],strlen(field[i]));
			}else if(result_set[i]=='c'){

				if(strlen(type_char[count_char2])!=0)
				{
				strncat(type_char[count_char2],"^",1);
				strcat(result,type_char[count_char2]);
				memset(type_char[count_char2],0,strlen(type_char[count_char2]));
				count_char2++;

				}else{

				memset(type_char[count_char2],0,strlen(type_char[count_char2]));
				strcat(result,"NULL^");
				count_char2++;
				
				}
			}//else if(result_set[i]+i=='c') ENd
		}//for(i=0;i<=val_num-1;i++) ENd
		result[strlen(result)-1]=0;
		strncat(result,"\n",1);
		
	}//while(1) ENd
	
	char *result_ptr=0;//result를 가리키는 포인터
	result_ptr=result;
	printf("result_ptr=%s\n",result_ptr); 
	printf("strlen result_ptr=%d\n",strlen(result_ptr));
	
	return result_ptr;
}

int field_count(char *message)
{
	int count=0;
	int i;
	char temp;
	for(i=0;i<=strlen(message);i++)
	{
		temp=message[i];
		if(temp==',') count++;
	}
	return count+1;

}

int row_count(char *message)
{
	long count=0;
	SDWORD count_len=SQL_NTS;
	char *message_temp;
	UCHAR query[200];
	message_temp=strstr(message,"from");
	puts(message_temp);
	getchar();
	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
		{
			sprintf((char *)query,"select count(*) from %s",message_temp);
			printf("query=%s\n",query);
			SQLPrepare(hstmt,(UCHAR *)select,SQL_NTS);
			SQLBindCol(hstmt,1,SQL_C_LONG,(PTR) &count,(SDWORD)sizeof(count), &count_len);
			_ret=SQLExecute(hstmt);
			printf("exe=%d\n",_ret);
			_ret=SQLFetch(hstmt);
			printf("fetch=%d\n",_ret);
			getchar();
		}
		printf("count=%d\n",count);
		getchar();
		return count;
}


char *db_error(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
 {
	char err_msg[200];
	char *err_msg_ptr;
	SQLINTEGER errNo; 
	SQLSMALLINT msgLength; 
	SQLCHAR errMsg[1000]; 
	if (SQL_SUCCESS == SQLError ( henv, hdbc, hstmt, NULL, &errNo, errMsg, 1000, &msgLength )) 
	{
		sprintf(err_msg," rCM_-%ld : %s\n", errNo, errMsg); 
	} 
	err_msg_ptr=err_msg;
	return err_msg_ptr; 
} 
/*
if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) {
        lpfVDMEnumTaskWOWEx = (INT (WINAPI *)(DWORD, TASKENUMPROCEX,
              LPARAM)) GetProcAddress(hInstLib2, "VDMEnumTaskWOWEx");
        if (lpfVDMEnumTaskWOWEx == NULL)
           __leave;
}
*/
