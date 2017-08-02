#include "header.h"
#include "header_s.h"
#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#include <process.h>		// Thread 관련
#include <windows.h>
#include <stdlib.h>

#if ( ( defined(WIN32) ) || ( defined(_WIN32) ) )
//#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#else
#include <sys/time.h>
#include <time.h>   
#endif

#define	X_RESULT_BODY_SUCCESS		0x00
#define	X_RESULT_BODY_BUSY			0x01
#define	X_RESULT_BODY_WRONGNUMBER	0x02
#define	X_RESULT_BODY_NOANSWEREXP	0x03
#define	X_RESULT_BODY_SMSSERVERDOWN	0x04
#define	X_RESULT_BODY_TRANSMITWAIT	0x05
#define	X_RESULT_BODY_TTSFAIL		0x06
#define	X_RESULT_BODY_TTFFAIL		0x07
#define	X_RESULT_BODY_GENERALERROR	0x09


/* Dest ttype */
#define	DEST_TEMP_TYPE		0
#define	DEST_FILE_TYPE		1


/* Response Code */
#define	X_RESULT_SUCCESS	0x00
#define	X_RESULT_ERROR		0x01
#define	X_RESULT_NOMEMBER	0x02
#define	X_RESULT_NOLENGTH	0x03
#define	X_RESULT_NOATTACH	0x04
#define	X_RESULT_QFULL		0x05
#define	X_RESULT_FAILURE	0x06
#define	X_RESULT_NOMONEY	0x07
#define	X_RESULT_NOPRO		0x08
#define	X_RESULT_HOLD		0x09
#define	X_RESULT_UNKNOWN	0x10
#define	X_RESULT_PASSWORD	0x11
#define	X_RESULT_NOTTS		0x12
#define	X_RESULT_NOTTF		0x13
#define	X_RESULT_EXIST		0x14
#define	X_RESULT_BOUND		0x15

#define X_RESULT_RESOURCE   0x16
#define X_RESULT_MIDNIGHT   0x17

#define	X_RESULT_PRO		0x99

/*  Message subtype. */
/* SMS */
#define	SMS_TEMP_FILE		0x01
#define	SMS_TEMP_TEMP		0x02

/* VOICE */
#define	VOICE_FILE_FILE		0x11
#define	VOICE_FILE_TEMP		0x12
#define	VOICE_IVR_FILE		0x13
#define	VOICE_IVR_TEMP		0x14
#define	VOICE_TEMP_FILE		0x15
#define	VOICE_TEMP_TEMP		0x16
#define	VOICE_VXML_FILE		0x17
#define	VOICE_VXML_TEMP		0x18

/* FAX */
#define	FAX_TEXT_FILE		0x21
#define	FAX_TEXT_TEMP		0x22
#define	FAX_TEMP_FILE		0x23
#define	FAX_TEMP_TEMP		0x24

/* VOICE */
#define	SPV_FILE_FILE		0x31 /* SPV = SMS PLUS VOICE  */
#define	SPV_FILE_TEMP		0x32
#define	SPV_IVR_FILE		0x33
#define	SPV_IVR_TEMP		0x34
#define	SPV_TEMP_FILE		0x35
#define	SPV_TEMP_TEMP		0x36
#define	SPV_VXML_FILE		0x37
#define	SPV_VXML_TEMP		0x38

/* VOICE */
#define	CONFERENCE			0x41

/* JOB_TYPE */
#ifndef JOBTYPE
#define JOBTYPE
/* JOBTYPE */
#define JOBTYPE_SMS             0x01        /* 문자 메시지 */
#define JOBTYPE_VOICE           0x02        /* 음성 메시지 */
#define JOBTYPE_FAX             0x03        /* 팩스 메시지 */
#define JOBTYPE_EMAIL           0x04        /* 메일 메시지 */
#endif

/* Messages  kinds */
#define X_MSG_BIND_INFO			0x00
#define	X_MSG_BIND				0x01
#define	X_MSG_LINK_STATUS		0x02
#define	X_MSG_MONEY_STATUS		0x03
#define	X_MSG_UNBIND			0x04

#define	X_MSG_SUBMIT			0x10
#define	X_MSG_REPORT			0x11
#define	X_MSG_STATUS			0x12
#define	X_MSG_STATUS_DETAIL		0x13

#define	X_MSG_CANCEL			0x20

#define	X_MSG_USER_ADD			0x30
#define	X_MSG_USER_DEL			0x31
#define	X_MSG_USER_CHANGE		0x32
#define	X_MSG_USER_INFO			0x33
#define	X_MSG_USER_CHANGE_RAW	0x34
#define	X_MSG_USER_ADD_2		0x35

#define	X_MSG_CON_INFO			0x40
#define X_MSG_DONE              0x41
#define X_MSG_ENQUEUE           0x43
#define	X_MSG_ALARM				0x44


int (*X_SUBMIT_CALLBACK)(int, char *, int, int, int, int);
int (*X_REPORT_CALLBACK)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*);
int	(*X_CANCEL_CALLBACK)(int, char *);
int	(*X_STATUS_CALLBACK)(int, char *, int, int);
int (*X_STATUSD_CALLBACK)(int, char *, int, int);


void print_menu();
void print_result();
void setupMessageRate();
void setupCallRate();
void setupSubscriber();
void setupSubscriberFile();
void setupTestTime();
void startCallSimulatation();
void try_bind();
void submitSms();
void submitVoice();
void submitFax();

void interrupt_handler();
void interrupt_handler2();


void tcp_receive_thread();


//---------------------
void help_basic();
void help_bind1();
void help_submit2();
void help_status3();
//void help_cancel4();
void help_report();

void help_submit2_1();
void help_submit2_2();
void help_submit2_3();
void help_submit2_4();
void help_submit2_5();
void help_submit2_6();
void help_submit2_7();
void help_submit2_8();

void help_submit2_1_1();
void help_submit2_1_2();	
void help_submit2_2_1();
void help_submit2_2_2();
void help_submit2_2_3();
void help_submit2_2_4();
void help_submit2_2_5();
void help_submit2_2_6();
void help_submit2_2_7();
void help_submit2_2_8();

void help_submit2_3_1();
void help_submit2_3_2();

void help_submit2_4_1();
void help_submit2_4_2();
void help_submit2_4_3();
void help_submit2_4_4();
void help_submit2_4_5();
void help_submit2_4_6();
void help_submit2_4_7();
void help_submit2_4_8();

void help_submit2_5_1();
void help_submit2_5_2();

void help_submit2_6_1();
void help_submit2_6_2();
void help_submit2_6_3();
void help_submit2_6_4();
void help_submit2_6_5();
void help_submit2_6_6();
void help_submit2_6_7();
void help_submit2_6_8();

void help_submit2_7_1();
void help_submit2_7_2();

void help_submit2_8_1();
void help_submit2_8_2();
void help_submit2_8_3();
void help_submit2_8_4();
void help_submit2_8_5();
void help_submit2_8_6();
void help_submit2_8_7();
void help_submit2_8_8();

void help_status3_1();
void help_status3_2();


void help_uadd();
void help_uadd2();
void help_uchange();
void help_uchangeraw();
void help_udel();
void help_uinfo();
void help_reportfile();

//------------------
int		ret_flag, ret;
char	*get_now();
char	*get_next();
void	*test_fn();
int		bind_flag = 0;
int		INTERVAL;
int		COUNT = 0;
int		waitCount = 0;

int		submitFlag = 0;

struct timeval start_t, end_t;


int		smsRate = 0;
int		voiceRate = 100;	
int		faxRate = 0;	
int		callsmsPeriod = 10;	
int		callvoicePeriod = 10;	
int		callfaxPeriod = 10;	
int		callspvPeriod = 10;	
int		subsCount = 500;	
char    subsArea[4] = "02";
char	subsPrefix[4] = "800";
char	subsStart[5] = "1200";	
char	subsEnd[5] = "1699";	
char	subsFilename[255] = "";	
int		subsFlag = 0;
int		testTime = 0;	

int		smsSent = 0;
int		smsSentAck = 0;
int		smsReport = 0;
int		smsFailCount = 0;
double	smsSuccessRate = 0;

int		voiceSent = 0;
int		voiceSentAck = 0;
int		voiceReport = 0;
int		voiceFailCount = 0;
double	voiceSuccessRate = 0;

int		faxSent = 0;
int		faxSentAck = 0;
int		faxReport = 0;
int		faxFailCount = 0;
double	faxSuccessRate = 0;

char	bindID[20];
char	bindPassword[20];

int		smsMsgID = 0;
int		voiceMsgID = 100000000;
int		faxMsgID = 200000000;
int		sentCountNumber = 0;
int		PrevsentCountNumber = 0;

char	inputSubs[1000][12];	
int		inputSubsCount = 500;
int		curSubsCount = 0;

char	*get_next();
void	*test_fn();
int		totalReportCount = 0;


int fn_x_cancel( int jobid, 
		int b,
		int *ret
	);


STATUSINFO  fn_x_status(
		char *stime,
		int period,
		int *ret
	);


int fn_x_bind( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	//int		lpfn_submit(int, char *, int, int, int, int),
	int	(*X_SUBMIT_CALLBACK)(int, char *, int, int, int, int),
	int		report_ack,
	//int		lpfn_report(char *, int, int, char *, int, char *, char*, int, int, int, int),
	int (*X_REPORT_CALLBACK)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*),
	int		cancel_ack,
	//int		lpfn_cancel(int, char *, int, int),
	int	(*X_CANCEL_CALLBACK)(int, char *),
	int		status_ack,
	//int		lpfn_status(int, char *, int, int),
	int	(*X_STATUS_CALLBACK)(int, char *, int, int),
	int		statusd_ack,
	//int		lpfn_statusd(int, char *, int, int),
	int (*X_STATUSD_CALLBACK)(int, char *, int, int),
	int		*ret);


int	fn_x_unbind( char *name, char *passwd );



STATUSDETAIL_INFO fn_x_statusd( int ret_flag,  int *ret_code );



int fn_x_submit( 
	int		msg_id,    
	int		jobtype,
	int		subjobtype,
	char	*subject,
	int		schedule_type,
	char	*schedule_stime,
	char	*schedule_dtime,
	char	*callback_number,
	char	*callback_url,
	int		dest_type,
	int		dest_file_count,
	char	*dest_file_info,
	int		dest_addr_count,
	char	*dest_addr,
	char	*sms_msg,
	char	*tts_content,
	int		attach_file_count,
	char	*attach_file,
	char	*temp_file,  /* Server에 존재하는 파일 이름 */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		*ret_code );


int fn_x_submit_2( 
	int		msg_id,    
	int		jobtype,
	int		subjobtype,
	char	*subject,
	int		schedule_type,
	char	*schedule_stime,
	char	*schedule_dtime,
	char	*callback_number,
	char	*callback_url,
	int		dest_type,
	int		dest_file_count,
	char	*dest_file_info,
	int		dest_addr_count,
	char	*dest_addr,
	char	*sms_msg,
	char	*tts_content,
	int		attach_file_count,
	char	*attach_file,
	char	*temp_file,  /* Server에 존재하는 파일 이름 */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,  /*  0 : Callee dtmf number */
	char	*counselor_num,  /* Counselor Number  */
	int		*ret_code );

int fn_x_submit_4 (
	int		msg_id,    
	int		jobtype,
	int		subjobtype,
	char	*subject,
	int		schedule_type,
	char	*schedule_stime,
	char	*schedule_dtime,
	char	*callback_number,
	char	*callback_url,
	int		dest_type,
	int		dest_file_count,
	char	*dest_file_info,
	int		dest_addr_count,
	char	*dest_addr,
	char	*sms_msg,
	char	*tts_content,
	int		attach_file_count,
	char	*attach_file,
	char	*temp_file,  /* Server에 존재하는 파일 이름 */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,
	char	*counselor_number,
	char	*header_msg,	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
	char	*tail_msg,		/* 맺음말 메시지, 파일만 사용 가능 */
	int		re_listen,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
	char    *kt_office,		/* 사원 번호 */
	int		*ret_code);


int fn_x_useradd(
	char	*userid,
	char	*username,
	char	*password,
	char	*mobile_no,
	char	*mobile_passwd,
	char	*reg_code,
	int		*ret_code );



int fn_x_useradd_2( 
	char	*username,
	char	*mail_password,
	char 	*ivr_password,
	char	*mobile_no,
	char	*reg_code,
	char	*mail_host,
	char	*mail_hostid,
	char	*pop3_host,
	char	*pop3_hostid,
	int		*ret_code );


int fn_x_userchange( 
	char	*username,
	char	*mail_password,
	char	*ivr_password,
	char	*mobile_no,
	char	*reg_code,
	int		*ret_code );


int fn_x_userchangeraw( 
	char	*query_string,
	int		*ret_code );


int fn_x_userdel(
	int		*ret_code );


USERINFO fn_x_userinfo( int *ret_code );

// ******************************************************************
// 콜백 함수 
// 전송
int	lpfn_submit( int ret_code, char *session_id, int msg_id, int job_id, int fee, int content_size );

// 레포트 
int	lpfn_report(
char	*session_id,
int		job_id,
int		subjob_id,
char	*dest_addr,
int		ret_code,
char	*start_time,
char	*end_time,
int		duration,
int		pages,
int		fee,
int		reply_dtmf,
char    *replied_file);

//  취소
int	lpfn_cancel( int ret_code, char *session_id );

// 상태 - 1
int	lpfn_status( int ret_code, char *session_id, int msg_id, int job_id )
{
	printf("[STATUS_CB] : ret_code | %d, msg_id | %d, job_id | %d \n", ret_code, msg_id, job_id );
	return 1;
}

// 상태 - 2
int	lpfn_statusd( int ret_code, char *session_id, int msg_id, int job_id )
{
	printf("[STATUSD_CB] : ret_code | %d, msg_id | %d, job_id | %d \n", ret_code, msg_id, job_id );
	return 1;
}
/* ********************************************************************************************
 * 사용자 정의 함수, 변수...
 ******************************************************************************************* */
// 로그 파일 
//#define FILE_SUBMIT_LOG		"d:\\callback\\submit\\%d.txt"
//#define FILE_SUBMIT_ERR		"d:\\callback\\submit_err\\%s.txt"
//#define FILE_REPORT_LOG		"d:\\callback\\report\\%d.txt"
//#define FILE_REPORT_ERR		"d:\\callback\\report_err\\%s.txt"

#define FILE_VOICE_WAV		"d:\\sendwav\\"		// 음성 파일 경로
#define FILE_FAX     		"d:\\sendfax\\"		// 팩스 파일 경로

// 쿼리문 버퍼 사이즈
#define BUFF_SIZE	128
#define INSERT_SIZE	2048
#define DELETE_SIZE	128

char *get_date();
char *get_time();
char *get_time2();
char *get_time4();
void exception();

//컴파일 옵션
#define NIGHT 0 //야간발송 제어 on

/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;
void SQL_EXCEPTION( int _ret );
int db_error(UCHAR *msg, SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);

int job_id;

// 로그 기록 함수 
//int submit_log(int, int);
//int report_log(int, char *, int, int, int, int, int);
//int submit_err(int, int);
//int report_err(int, char *, int, int, int, int, int);

/* *** 스래드 함수 *** */
HANDLE	hThread, hThread_2, hThread_c;
DWORD	hID, hID_2, hID_c;
CRITICAL_SECTION  cs, cs_2;							// 스래드 동기화 변수
CRITICAL_SECTION  cs_db, cs_c;

DWORD WINAPI getThread(void *);
DWORD WINAPI getThread_2(void *);

DWORD WINAPI help_cancel4(void *);		// 취소 기능

/* *** 큐 정의 *** */
void init_queue(void);
int put(int,int);
int	put_2(int job_id, char *tel, int ret_code,int talktime, int pages,int fee,int dtmf);

//int get();

//#define MAX_QUE 80000
#define		MAX_QUE 163840		// 한국통신 서버의 큐-버퍼는 8 만개이다.

struct _dnode
{
	int seq;
	int jobid;
//	struct _dnode *prev;
//	struct _dnode *next;
};
typedef struct _dnode dnode;

struct _dnode_2
{
	// 두번째 큐 자료형
	int job_id;
	char *tel; 
	int ret_code;
	int talktime;
	int pages;
	int fee;
	int dtmf;
};
typedef struct _dnode_2 dnode_2;

dnode	Q[MAX_QUE];
dnode_2	Q_2[MAX_QUE];
	
int F, R;				// Front, Rear : 두개의 큐 사용 
int F_2, R_2;
int q_flag = 1;			// 세마포어가 실행시 블록 상태로 시작
int q_flag_2 = 1;
int k, k_2;				// Q counter;
unsigned long CB_COUNTER = 0; // Cancel CB counter

char *meche="";
SDWORD meche_len=SQL_NTS;

int isNhic(void);
int isNight(void);
int isNight2(char *);
void ErrorHandling(char *message);

SOCKET hSocket2;

// ****************************
void main()
{
	int		retnum;
	int		ret;

	  WSADATA wsaData;
	  
//	  char message[BUFSIZE];
//	  int strLen;

	  SOCKADDR_IN servAddr;

	  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
			ErrorHandling("WSAStartup() error!"); 

	  hSocket2=socket(PF_INET, SOCK_STREAM, 0);   
	  if(hSocket2 == INVALID_SOCKET)
		ErrorHandling("socket() error");

	  memset(&servAddr, 0, sizeof(servAddr));
	  servAddr.sin_family=AF_INET;
	  servAddr.sin_addr.s_addr=inet_addr("211.224.130.165");
	  servAddr.sin_port=htons(atoi("8100"));
	  if(connect(hSocket2, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error!");
	



	ret=fn_x_set_conf_file("d:\\conf\\ver2\\xroshot.conf") ;
	// 바인드 는 Call back 함수 등록 한다. 
	retnum = fn_x_bind(	0, "sohi9","09421", "xroshot.com", 
			1, lpfn_submit,
			1, lpfn_report,
			1, lpfn_cancel,
			1, lpfn_status,
			1, lpfn_statusd,
			&ret);

			if ( retnum != X_RET_FUNC_SUCCESS )
			{
				printf("Can't connect to server.... exit.\n");
			}
			
			
			switch ( ret )
			{
				// BIND 성공 
				case 0x00 :
					printf( "바인드 되었습니다. \n" );
					break;
				// Exception - BIND
				case 0x01 : 
					printf( "Result Error.\n" );
					exception();
					break;
				case 0x02 :
					printf( "Not a member Error.\n" );
					exception();
					break;
				case 0x03 :
					printf( "Length Mismatch Error.\n" );
					exception();
					break;
				case 0x04 :
					printf( "Attach Mismatch Error.\n" );
					exception();
					break;
				case 0x05 :
					printf( "Queue Full Error.\n" );
					exception();
					break;
				case 0x06 :
					printf( "Failure Error.\n" );
					exception();
					break;
				case 0x07 :
					printf( "Not Enough Money Error.\n" );
					exception();
					break;
				case 0x08 :
					printf( "No Program Error.\n" );
					exception();
					break;
				case 0x09 :
					printf( "Holding now.\n" );
					exception();
					break;
				case 0x10 :
					printf( "UnKnown Error.\n" );
					exception();
					break;
				case 0x11 :
					printf( "Password Mismatch Error.\n" );
					exception();
					break;
				case 0x12 :
					printf( "TTS Service Error.\n" );
					exception();
					break;
				case 0x13 :
					printf( "TTF Service Error.\n" );
					exception();
					break;
				case 0x14 :
					printf( "Already Logged in ...\n" );
					exception();
					break;
				case 0x15 :
					printf( "Already Bind Error.\n" );
					exception();
					break;
				case 0x16 :
					printf( "Resource Error.\n" );
					exception();
					break;
				case 0x17 :
					printf( "Midnight... Prohibited.\n" );
					exception();
					break;
				default	:
					printf( "Error occurred...\n" );
					exception();
					break;
			}


	// 큐 초기화 
	init_queue();

	// 초기화 - MS SQL
	henv = hdbc = hstmt = 0;  

	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
	_ret=SQLConnect(hdbc,(UCHAR *)"karma",SQL_NTS,
		 (UCHAR *)"karma",SQL_NTS,(UCHAR *)"karmahaja8282",SQL_NTS);

	// SQL Connect Exception's
	SQL_EXCEPTION( _ret );

	// 스래드 동기화 선언 : cs 변수는 동기화 ID 이다.
	InitializeCriticalSection( &cs   );
//	InitializeCriticalSection( &cs_2 );
	InitializeCriticalSection( &cs_db);
	//InitializeCriticalSection( &cs_c );
	//InitializeCriticalSection( &cs_gc);
	// 스래드 실행
	// DWORD WINAPI getThread(void *args);
	// get(); 함수를 변영 했음.
	// F == R 일때 블록 된다.
	//hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int ) getThread, 
	//								(void *)NULL, 0, (unsigned long *)&hID);
	//hThread_2 = (HANDLE)_beginthreadex(NULL, 0, (unsigned int)getThread_2, 
	//								(void *)NULL, 0, (unsigned long *)&hID_2);
	
	EnterCriticalSection( &cs );	// 최초 대기 상태
//	EnterCriticalSection( &cs_2 );
//	EnterCriticalSection( &cs_c );

	hThread	  = (HANDLE)CreateThread(NULL, 0, getThread,    (void *)NULL, 0, (unsigned long *)&hID  );
//	hThread_2 = (HANDLE)CreateThread(NULL, 0, getThread_2,  (void *)NULL, 0, (unsigned long *)&hID_2);
	//hThread_gc = (HANDLE)CreateThread(NULL, 0, getCancel, (void *)NULL, 0, (unsigned long *)&hID_gc);
//	hThread_c = (HANDLE)CreateThread(NULL, 0, help_cancel4, (void *)NULL, 0, (unsigned long *)&hID_c);

/*	
	if ( hThread == 0 || hThread_2 == 0 || hThread_c == 0) {
	//if ( hThread == 0 || hThread_2 == 0 ) {
		fprintf(stderr, "\n Thread Created ERROR[%d] : [%d] : [%d] !! \n", hThread, hThread_2, hThread_c);
		fprintf(stderr, "HIT KEY....(EXIT)");
		getchar();
		exit(1);
	}
*/	
	// 문자, 음성 전송을 위한 주 함수.....
	// main() 에서 초기화 후 모든 제어권 이전 
    help_basic();
	
   
}

void exception()
{
	printf("예외발생입니다.~!\n");
	printf("프로그램이 종료됩니다.\n");
	fprintf(stderr, "HIT KEY....(EXIT)");
	getchar();
	exit(1);
}

void SQL_EXCEPTION( int _ret ) 
{
	switch ( _ret ) {
		case SQL_SUCCESS : 
			printf("SQL_SUCCESS !!\n");
			break;
		case SQL_SUCCESS_WITH_INFO : 
			printf("SQL_SUCCESS_WITH_INFO !!\n");
			break;
		case SQL_INVALID_HANDLE : 
			printf("SQL_INVALID_HANDLE !!\n");
			printf("Hit ....");
			getchar();
			exit(1);
			break;	
		case SQL_ERROR : 
			printf("SQL_ERROR !!\n");
			printf("Hit ....");
			getchar();
			exit(1);
			break;
	//	case SQL_SUCCESS : 
	//		printf("SQL_SUCCESS !!\n");
	//		break;
		default : 
			break;
	}
}

int db_error( UCHAR *msg, SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
{
	SQLINTEGER errNo;
	SQLSMALLINT msgLength;
	SQLCHAR errMsg[80];

	printf("%s -> ", msg);

	if ( SQL_SUCCESS == SQLError( henv, hdbc, hstmt, NULL, &errNo, errMsg, 80, &msgLength) ) {
		printf(" rCM_-%ld : %s \n", errNo, errMsg);
	}
	else {
		printf(" rCM_-OK! : SQL 성공!! \n", errNo, errMsg);
	}

	return SQL_ERROR;
}




/*
// ????
char *meche="";
SDWORD meche_len=SQL_NTS;.
*/

// ** sms 수신 및 레프트 등록 ** 
void help_basic()
{
	SQLRETURN res_sql;
	
	while(1)
	{
	//printf("help_basic() Ent[help_basic]->\n");		// SQL 동기화
	EnterCriticalSection( &cs_db );
				
	meche=(char *)malloc(sizeof(char)*2);
	
	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR select[55]="SELECT meche FROM message2";
		SQLPrepare(hstmt,select,SQL_NTS);
		SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) meche,
				  (SDWORD)sizeof(meche),&meche_len);
			
	/*	while((res_sql=SQLExecute(hstmt)) == -1) {  
			printf("전송 자료 재질의 ...[%s] \n", select);
			Sleep(500);
		}*/
		res_sql=SQLExecute(hstmt);
		_ret=SQLFetch(hstmt);
		
		if (_ret == 100){
			Sleep(20);
			//Sleep(1000);
			free(meche);
			SQLFreeStmt(hstmt,SQL_DROP);
	//		printf("sleeping>.<\n");
	//		printf("help_basic() Lev[help_basic]<-\n");	// 수정 금지 !!!
			LeaveCriticalSection( &cs_db );
			continue;	
		}
		
		if (_ret == 0 || _ret== 1 ){
			
			int i=atoi(meche);
			switch(i)
			{
				// 문자 즉시 
				case 1 :
				printf("call help_submit2_1_1\n");
				_ret = SQLFreeStmt(hstmt,SQL_DROP);
				printf("SQL FREE STMT ...  !!!\n");
				free(meche);
				//Sleep(10);
				help_submit2_1_1();	
				printf("call sumbit end");
				break;
				// 문자 예약 
				case 2 :
				printf("call help_submit2_5_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				free(meche);
				//Sleep(10);
				help_submit2_5_1();
				break;
				// 음성 즉시 
				case 3 : 
				printf("call help_submit2_2_5\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				free(meche);
				//Sleep(10);
				help_submit2_2_5();
				break;
				// 음성 예약 
				case 4 :
				printf("call help_submit2_6_5\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				free(meche);
				//Sleep(10);
				help_submit2_6_5();
				break;
				// 음성 즉시 , 첨부 파일
				case 5 :
				printf("call help_submit2_2_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				help_submit2_2_1();
				break;
				
				case 6 :
				printf("call help_submit2_6_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				help_submit2_6_1();
				break;
				// 팩스 즉시 
				case 7 :
				printf("call help_submit2_3_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				help_submit2_3_1();
				break;
				// 팩스 예약 
				case 8 :
				printf("call help_submit2_7_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				help_submit2_7_1();
				break;
				// 음성 즉시 , 첨부 파일
				case 9 :
				printf("call help_submit2_2_1\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				help_submit2_2_1();
				break;
				default :
				/*	printf("Data ERR !!! \n\n");
					getchar();
				*/
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
				break;
			} // switch
		} // if()
	
		}// DB block
//		printf("help_basic() Lev[while]<-\n"); // 수정 금지  
//		LeaveCriticalSection( &cs_db );
	} 
}


//문자즉시 - neverdie OLD(GOOD!!)
void help_submit2_1_1()
{

	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[1002]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	SDWORD gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;
	UCHAR delete_message2[DELETE_SIZE]="";

   
	int	ret=0;
	int cnt=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM message2 where meche='1'");	
	SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
              (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
	          (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
	          (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
	          (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
	          (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
	          (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
	          (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
		      (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			  (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			  (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			  (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12,SQL_C_LONG,(PTR) &gid_ptr, 
	          (SDWORD)sizeof(gid_ptr), &gid_len);
	
	_ret=SQLExecute(hstmt); 
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	
	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);
	char tel_set2[65]="";
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time2();
	strtok(tel_set2," ");
	

	int i;

	for (i=1002; i >= 0; --i) {
		if (msg_db_ptr[i] != ' ') {
			msg_db_ptr[i+1] = '\0';
			break;
		}
	}
#if defined(NIGHT)
	if(isNight()==0 && isNhic()==0)
	{
		puts("야간발송 error");
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)insert_data,"insert into message_bak2(gid,seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,rettel,ret_code,talktime,fee,page) values(%d,%d,'%s','%s',1,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',88,0,0,0)",gid_n,seq_n,j_no_ptr,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,rettel_ptr); 
		printf("%s\n",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS);
		_ret = SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
		

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)delete_message2,"delete from message2 where seq=%d",seq_n);
		SQLPrepare(hstmt,delete_message2,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		SQLFreeStmt(hstmt,SQL_DROP) ;
		}
    
		printf("help_basic() Lev[문자]<-\n");
		LeaveCriticalSection( &cs_db );
		return;
	}	
#endif
	printf("msg length : %d\n", strlen(msg_db_ptr));
fn_x_submit_4 (
		seq_n,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		title_ptr,
		0,
		now,
		now,
		rettel_ptr,	//rettel
		"http://www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,     //cnt
		tel_set2,//phone_set
		msg_db_ptr,
		"",
		0,
		"",
		"",
		0,
		0,
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,
		"",
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);


 	

		printf(" SUBMIT 완료 되었습니다. ");
	  
	
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)insert_data,"insert into message_bak2(gid,seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,rettel) values(%d,%d,'%s','%s',1,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",gid_n,seq_n,j_no_ptr,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,rettel_ptr); 
		printf("%s\n",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS);
		_ret = SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
		

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)delete_message2,"delete from message2 where seq=%d",seq_n);
		SQLPrepare(hstmt,delete_message2,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		SQLFreeStmt(hstmt,SQL_DROP) ;
		}
    }
	printf("help_basic() Lev[문자]<-\n");
	LeaveCriticalSection( &cs_db );
	return;
}

/*
// 문자 즉시 - 9일 
void help_submit2_1_1()
{
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[500]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[9]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	UCHAR *buff;
	UCHAR *insert_data;
	UCHAR *delete_data;

	int		i = 0;
	int		ret=0;

	hstmt=0;

	_ret=SQLAllocStmt(hdbc,&hstmt);

	{
	buff=(UCHAR *)malloc(130);	
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM goodshot.message2 where meche='1'");
	SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);

	_ret=SQLExecute(hstmt);  
printf("SQL preparse\n");

	_ret=SQLFetch(hstmt);
printf("SQL Fetch\n");

	//________________
	printf("____m_________________________________________________________________\n");
	printf("rettel_ptr[%s] , msg_db_ptr[%s] , tel_ptr[%s], seq_ptr[%d],\n name_db_ptr[%s], j_no_ptr[%s], \n rdate_ptr[%s], rtime_ptr[%s], userid_ptr[%s], title_ptr[%s], id_no_ptr[%s], gid_ptr[%d]\n", 
		    rettel_ptr,      msg_db_ptr,      tel_ptr,     seq_ptr,       name_db_ptr,    j_no_ptr,          rdate_ptr,     rtime_ptr,     userid_ptr,     title_ptr,     id_no_ptr,     gid_ptr);
	printf("_>./<__;l;l;____\n");
	printf("rettel_len[%d] , msg_db_len[%d] , tel_len[%d], seq_len[%d],\n name_db_len[%d], j_no_len[%d], \n rdate_len[%d], rtime_len[%d], userid_len[%d], title_len[%d], id_no_len[%d], gid_len[%d]\n", 
		    rettel_len,      msg_db_len,      tel_len,     seq_len,       name_db_len,    j_no_len,          rdate_len,     rtime_len,     userid_len,     title_len,     id_no_len,     gid_len);
	printf("____m___________________________________________________________________\n");
	//__________________
	


	SQLFreeStmt(hstmt,SQL_DROP);
printf("SQL Free\n");

	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	char *name_set;
		name_set = strcat(strtok(name_db_ptr," "),"^");
	char *tel_set;
		tel_set = strcat(name_set,tel_ptr);	
	char tel_set2[18]="";
		strcpy(tel_set2,tel_set);
		strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time();
		strtok(tel_set2," ");
//	strtok(msg_db_ptr,".");
	
printf("SQL VAR DECL\n");
printf("\n[완성]\n title_ptr[%s] now[%s]\n rettel_ptr[%s] tel_set2[%s]\n msg_db_ptr[%s]\n",
	   title_ptr, now, rettel_ptr, tel_set2, msg_db_ptr);
//getchar();
	job_id=fn_x_submit(	
		seq_n, 
		JOBTYPE_SMS, 
		VOICE_TEMP_TEMP, 
		title_ptr, 
		0, 
		now,//get_now()
		now,//get_now()
		rettel_ptr,  
		"http^//www.xroshot.com", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		0,					// addr count 
		tel_set2,
		"", 				// sms
		msg_db_ptr, 			// tts
		0, 					// content file count
		"",
		"",
		0, 
		0, 
		0,
		"",
		0,
		1,
		&ret
	);

printf("submit!!\n");

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	insert_data=(UCHAR *)malloc(1000);
	sprintf((char *)insert_data,"insert into goodshot.message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,1,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("insert ok\n\t[%s]\n", insert_data);
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	delete_data=(UCHAR *)malloc(100);
	sprintf((char *)delete_data,"delete from goodshot.message2 where seq=%d",seq_n);
	printf("delete_data=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("delete ok");
	free(buff);
	free(insert_data);      
	free(delete_data);
  }
  // 동기화 
  printf("help_basic() Lev[문자]<-\n");
  LeaveCriticalSection( &cs_db );
 
  return ;
}
*/
  

/*
// 문자 즉시 
void help_submit2_1_1()
{
	printf("_______________________________________시작 \n");
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[80]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	SDWORD gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	char name_db_ptr[9]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	printf("rrrrrrrrrrr\n");
//	getchar();
	//UCHAR *buff;
	//UCHAR *insert_data;
	//UCHAR *delete_message2;
   
	int	ret=0;
	int cnt=0;


	//int gid_n=gid_ptr;
	//int seq_n=seq_ptr;
	//char tel_db[12]="";
	
	
	//char tel_set2[18]="";
	//char *date=get_date();
	//char *time=get_time2();
	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	//buff=(UCHAR  *)malloc(150);
	UCHAR buff[1000]="SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM goodshot.message2 where meche='1'";
	//sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM message2 where meche='1'");	
	SQLPrepare(hstmt,buff,SQL_NTS);
    
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
		           (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
	          (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
	          (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
	          (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
	          (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
	          (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
	          (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
		      (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			  (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			  (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			  (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12,SQL_C_LONG,(PTR) &gid_ptr, 
	          (SDWORD)sizeof(gid_ptr), &gid_len);
	
	_ret=SQLExecute(hstmt); 
	_ret=SQLFetch(hstmt);
	printf("_ret=%d\n",_ret);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	
	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	char tel_set2[18]="";
	char *date=get_date();
	char *time=get_time2();
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	printf("name_set=%s\n",name_set);
	printf("+++++++++++++++++++++++++++++\n");
	
	char *tel_set=strcat(name_set,tel_ptr);
		printf("[1]name_set-<tel_ptr=%s<-%s\n",name_set, tel_ptr);

//	strcpy(tel_set2, name_set);
	char *name_db_n=strtok(name_set,"^");
		printf("name_db_n:%s\n", name_db_n);
		printf("[2]name_set=%s\n",name_set);
	char *_msg=msg_db_ptr;
		printf("_msg=%s\n",_msg);


	strcpy(tel_set2,tel_set);
		printf("tel_set2=%s\n",tel_set2);
	strtok(rettel_ptr," ");
		printf("rettel_ptr=%s\n",rettel_ptr);
	strtok(tel_set2," ");
		printf("tel_set2=%s\n",tel_set2);

	strcpy(tel_db,tel_ptr);
		printf("tel_db=%s\n", tel_db);
	
	
	printf("wwwwwwwwwwwwwwww\n");
//	getchar();
	strcat(tel_set2, "^");
	strcat(tel_set2, tel_db);
	printf("tel_set2=%s\n", tel_set2);
	printf("=====\n");
	printf("seq_n=%d\n title=%s\n rettel=%s\n tel_set2=%s\n msg=%s\n",seq_n,title_ptr,rettel_ptr,tel_set2,msg_db_ptr);
	printf("=====\n");
	//getchar();
	ret_flag=fn_x_submit( seq_n,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		title_ptr,
		0,
		get_now(),
		get_now(),
		rettel_ptr,	//rettel
		"http://www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,        //cnt -> 대상자 수 
		tel_set2,   //phone_set -> "이름^0112342345"
		_msg,		// 메세지 
		"",
		0,
		"",
		"",
		0,
		0,
		0,
		"",
		0,
		0,
		&ret
	    );
	
	
		printf("submit_call [ret_code : %d - %x]\n", ret_flag, ret_flag);
		printf(" SUBMIT 완료 되었습니다. ");
		printf("name=%s\n",name_db_n);
	
		hstmt1=0;
		_ret=SQLAllocStmt(hdbc,&hstmt1);
		{
		//insert_data=(UCHAR *)malloc(700);
		UCHAR insert_data[500];
		sprintf((char *)insert_data,"insert into goodshot.message_bak2(gid,seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,rettel) values(%d,%d,'%s','%s',1,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",gid_n,seq_n,j_no_ptr,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_ptr,_msg,title_ptr,tel_db,id_no_ptr,rettel_ptr); 
		printf("%s\n",insert_data);
		_ret = SQLPrepare(hstmt1,insert_data,SQL_NTS);
		_ret=SQLExecute(hstmt1);
		printf("_ret=%d\n",_ret);
		_ret=SQLFreeStmt(hstmt1,SQL_DROP);
		}
		

		hstmt1=0;
		_ret=SQLAllocStmt(hdbc,&hstmt1);
		{
		//delete_message2=(UCHAR *)malloc(100);
		UCHAR delete_message2[100];
		sprintf((char *)delete_message2,"delete from goodshot.message2 where seq=%d",seq_n);
		SQLPrepare(hstmt1,delete_message2,SQL_NTS); 
		printf("%s\n",delete_message2);
   		_ret=SQLExecute(hstmt1);
		printf("_ret=%d\n",_ret);
		SQLFreeStmt(hstmt1,SQL_DROP) ;
		}
		//free(buff);
		//free(insert_data);
		//free(delete_message2);
    }
	printf("_______________________________________끝 \n");
	return;

}*/
//문자즉시 - 2
/*
void help_submit2_1_1()
{
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[80]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	SDWORD gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	char name_db_ptr[9]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	UCHAR *buff;
	UCHAR *insert_data;
	UCHAR *delete_message2;
   
	int	ret=0;
	int cnt=0;

	hstmt=0;
	
	_ret=SQLAllocStmt(hdbc,&hstmt);

	{
	buff=(UCHAR  *)malloc(150);
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM goodshot.message2 where meche='1'");	
	SQLPrepare(hstmt,buff,SQL_NTS);
    
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
		           (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
	          (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
	          (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
	          (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
	          (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
	          (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
	          (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
		      (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			  (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			  (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			  (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12,SQL_C_LONG,(PTR) &gid_ptr, 
	          (SDWORD)sizeof(gid_ptr), &gid_len);
	
	_ret=SQLExecute(hstmt); 
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);
	char tel_set2[18]="";
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time2();
	char *_msg=msg_db_ptr;
	strtok(tel_set2," ");
	strtok(_msg,"\n");

		
    job_id=fn_x_submit( 
		seq_n,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		title_ptr,
		0,
		now,
		now,
		rettel_ptr,	//rettel
		"http://www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,   //cnt
		tel_set2,//phone_set
		_msg,
		"",
		0,
		"",
		"",
		0,
		0,
		0,
		"",
		0,
		0,
		&ret
	    );
		

		printf(" SUBMIT 완료 되었습니다. ");
	  
	
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		insert_data=(UCHAR *)malloc(700);
		sprintf((char *)insert_data,"insert into goodshot.message_bak2(gid,seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,rettel) values(%d,%d,'%s','%s',1,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",gid_n,seq_n,j_no_ptr,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,rettel_ptr); 
		printf("%s\n",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS);
		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
		

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		delete_message2=(UCHAR *)malloc(100);
		sprintf((char *)delete_message2,"delete from goodshot.message2 where seq=%d",seq_n);
		SQLPrepare(hstmt,delete_message2,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		SQLFreeStmt(hstmt,SQL_DROP) ;
		}
		free(buff);
		free(insert_data);
		free(delete_message2);
    }
	return;
}

*/

/*void 
help_submit2_1_2()
{
	char	msg_num[20], title[20], repnum[20], msg[100], filename[100];
	int		msgnum;

	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 메시지 : ");
	gets( msg );
	printf(" 수신자파일 --> ");
	gets( filename );
	printf(" ----------------- \n");

	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_SMS, 
		SMS_TEMP_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"http^//www.xroshot.com", 
		DEST_FILE_TYPE, 
		1,
		filename, 
		0, 
		"", 
		msg, 
		"", 
		0, 
		"", 
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		&ret
	);


	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

//	help_submit2_1();
}


void 
help_submit2_2()
{
	printf(" \n\n " );
	printf(" 2-2-1. 음성메시지 음성파일 (동보 가능) \n");
	printf(" 2-2-2. 음성메시지 음성파일 (파일 전송) \n");
	printf(" 2-2-3. 음성메시지 IVR 파일 (동보 가능) \n");
	printf(" 2-2-4. 음성메시지 IVR 파일 (파일 전송) \n");
	printf(" 2-2-5. 음성메시지 TTS변환  (동보 가능) \n");
	printf(" 2-2-6. 음성메시지 TTS변환  (파일 전송) \n");
	printf(" 2-2-7. 음성메시지 VXML파일 (동보 가능) \n");
	printf(" 2-2-8. 음성메시지 VXML파일 (파일 전송) \n");
	printf(" x. 이전화면  \n");
	gets( in_str );
	if ( strncmp( in_str, "2-2-1", 5 ) == 0 ) help_submit2_2_1();
	else if ( strncmp( in_str, "2-2-2", 5 ) == 0 ) help_submit2_2_2();
	else if ( strncmp( in_str, "2-2-3", 5 ) == 0 ) help_submit2_2_3();
	else if ( strncmp( in_str, "2-2-4", 5 ) == 0 ) help_submit2_2_4();
	else if ( strncmp( in_str, "2-2-5", 5 ) == 0 ) help_submit2_2_5();
	else if ( strncmp( in_str, "2-2-6", 5 ) == 0 ) help_submit2_2_6();
	else if ( strncmp( in_str, "2-2-7", 5 ) == 0 ) help_submit2_2_7();
	else if ( strncmp( in_str, "2-2-8", 5 ) == 0 ) help_submit2_2_8();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_2();
}


*/
// 첨부 음성 발송...
void help_submit2_2_1()
{
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[500]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[9]="";
	SDWORD name_db_len=SQL_NTS; 
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;
	UCHAR delete_data[DELETE_SIZE]="";

	char file[40];

	int		i = 0;
	int		ret=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM message2 where meche='5'");
	SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	SQLFreeStmt(hstmt,SQL_DROP);

	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);	
	char tel_set2[18]="";
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time();
	strtok(tel_set2," ");
/*
	int i;

	for (i=498; i >= 0; --i) {
		if (msg_db_ptr[i] != ' ') {
			msg_db_ptr[i+1] = '\0';
			break;
		}
	}
*/
	

	strcpy(file, FILE_VOICE_WAV);
	strcat(file, msg_db_ptr);
	

	fn_x_submit_4 (
		seq_n, 
		JOBTYPE_VOICE, 
		VOICE_FILE_TEMP, 
		title_ptr, 
		0, 
		now,//get_now()
		now,//get_now()
		rettel_ptr,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		0,					// addr count 
		tel_set2,
		"", 				// sms
		"", 			// tts
		0, 					// content file count
		file,
		"",
		0, 
		0, 
		0,
		"", 
		0,  
		0,  
		0,
		"",
		"",	
		"",	
		0,	
		"251186",
		&ret);
printf("[File] rettel_ptr : %s, tel_set2 : %s, file :%s\n", rettel_ptr, tel_set2, file);


	hstmt=0;

	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,9,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("insert ok\n");
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	printf("delete_data=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("delete ok");
  }
  // 동기화 
  printf("help_basic() Lev[음성첨부]<-\n");
  LeaveCriticalSection( &cs_db );
  return ;

}


/*
void 
help_submit2_2_2()
{
	char	msg_num[20], title[20], repnum[20], 
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_FILE_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_FILE_TYPE, 
		1,
		ufile, 
		0, 
		"", 
		"", 
		"", 
		1, 
		infile, 
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_2();
}



void 
help_submit2_2_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			file[100], list[1000], list1[1000], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_IVR_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		i,					// addr count 
		list,				// dest addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		file,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_2();
}



void 
help_submit2_2_4()
{
	char	msg_num[20], title[20], repnum[20], 
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_VOICE, 
		VOICE_IVR_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		ufile,				// file name
		0,					// addr count 
		"",					// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_2();
}

*/
//음성 즉시
void help_submit2_2_5()
{
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[1002]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	UCHAR delete_data[DELETE_SIZE]="";
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";


	int		i = 0;
	int		ret=0;

	hstmt=0;

	_ret=SQLAllocStmt(hdbc,&hstmt);
	db_error((UCHAR *)"음성 즉시(Alloc) ", henv, hdbc, hstmt);

	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM message2 where meche='3'");
	SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	SQLFreeStmt(hstmt,SQL_DROP);

	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
   	char *tel_set=strcat(name_set,tel_ptr);	
	char tel_set2[65]="";
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time();
	strtok(tel_set2," ");
#if defined(NIGHT)
	if(isNight()==0 && isNhic()==0)
	{
		puts("야간발송 error");
		hstmt=0;

		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel,ret_code,talktime,fee,page) values(%d,3,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',88,0,0,0)",seq_n,gid_n,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
		printf("insert_data=%s\n",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
		_ret = SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
		
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
		printf("delete_data=%s\n",delete_data);
		_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
		printf("delete ok");
  
	  // 동기화 
		  printf("help_basic() Lev[음성]<-\n");
		  LeaveCriticalSection( &cs_db );
  		  return ;
	}
#endif

	int i;

	for (i=1002; i >= 0; --i) {
		if (msg_db_ptr[i] != ' ') {
			msg_db_ptr[i+1] = '\0';
			break;
		}
	}


	fn_x_submit_4 (
		seq_n, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
		title_ptr, 
		0, 
		now,//get_now()
		now,//get_now()
		rettel_ptr,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		0,					// addr count 
		tel_set2,
		"", 				// sms
		msg_db_ptr, 			// tts
		0, 					// content file count
		"",
		"",
		0, 
		0, 
		0,
		"", 
		0,  
		0,  
		0,
		"",
		"",	
		"",	
		0,	
		"251186",		
		&ret);


	hstmt=0;

	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,3,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	printf("!!!!!!!!!!!!!%d\n",_ret);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("insert ok\n");
		
printf("삭제 시작 \n");

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	printf("delete_data=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("delete ok");
  }
  // 동기화 
  printf("help_basic() Lev[음성]<-\n");
  LeaveCriticalSection( &cs_db );
  
  return ;
}

/*
void 
help_submit2_2_6()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			msg[1000], ufile[100], infile[100];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자화일: ");
	gets( ufile );


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		ufile,					// file name
		0,						// addr count 
		"", 					// dest addr
		"", 					// sms
		msg,					// tts
		0, 						// content file count
		"",						// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_2();
}


void 
help_submit2_2_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 수신자리스트: ");
	gets(list);
	printf( "첨부 화일 : " );
	gets(infile);

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_VXML_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_TEMP_TYPE,			// customer type
		0,						// customer file count
		"",						// file name
		i,						// addr count 
		list,
		"", 					// sms
		"", 					// tts
		1, 						// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret 
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_2();
}


void 
help_submit2_2_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			msg[1000], ufile[100], list1[1000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_VXML_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_2();
}



void 
help_submit2_3()
{
	printf(" \n\n");
	printf(" 2-3-1. 팩스메시지 (동보 가능) \n");
	printf(" 2-3-2. 팩스메시지 (파일 전송) \n");
	printf(" x. 이전화면\n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-3-1", 5 ) == 0 ) help_submit2_3_1();
	else if ( strncmp( in_str, "2-3-2", 5 ) == 0 ) help_submit2_3_2();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_3();
}

*/
//팩스즉시
void help_submit2_3_1()
{
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[200]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	int	 ret=0;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;

 	int	i = 0;

	char file[40];
 
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,id_no,gid FROM message2 where meche='7'");
    _ret = SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
			
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);

	printf("seq=%d\n",seq_ptr);
	int gid_n=gid_ptr;
	int seq_n=seq_ptr;
	char tel_db[12]="";
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);
	char tel_set2[65]="";
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
	char *now=get_now();
	char *date=get_date();
	char *time=get_time2();
	strtok(tel_set2," ");

	strcpy(file, FILE_FAX);
	strcat(file, msg_db_ptr);
	//strtok(msg_db_ptr," ");

	int i;

	for (i=200; i >= 0; --i) {
		if (msg_db_ptr[i] != ' ') {
			msg_db_ptr[i+1] = '\0';
			break;
		}
	}

	printf("title=%s\n rettel=%s\n tel_set2=%s\n file=%s\n",title_ptr,rettel_ptr,tel_set2,file);
	Sleep(50);
fn_x_submit_4 (
		seq_n, 
		JOBTYPE_FAX, 
		FAX_TEXT_TEMP, 
		title_ptr, 
		0, 
		now,
		now,
		"",  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",					// file name
		1,					// addr count 
		tel_set2, 			// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		file,
		"",
		0, 
		0, 
		0,
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,
		"",
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);

	printf(" SUBMIT 완료 되었습니다. \n" );
	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into message_bak2(seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,gid,rettel) values(%d,'%s','%s',7,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s')",seq_n,j_no_ptr,userid_ptr,date,time,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,gid_n,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
			
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR delete_data[DELETE_SIZE];
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	printf("delete=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	printf("del=%d\n",_ret);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
  }
    printf("help_basic() Lev[팩스]<-\n");
	LeaveCriticalSection( &cs_db );
	return;
}
/*
void 
help_submit2_3_2()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], ufile[100],
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	qnum = atoi( in_str );
	printf( "첨부 화일 : " );
	gets( infile );
	printf(" 수신자화일 : ");
	gets( ufile );

	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_FAX, 
		FAX_TEXT_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"", 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_3();
}




void 
help_submit2_4()
{
	printf(" \n\n ");
	printf(" 2-4-1. 문자플러스 음성파일 (동보 가능) \n");
	printf(" 2-4-2. 문자플러스 음성파일 (파일 전송) \n");
	printf(" 2-4-3. 문자플러스 IVR 파일 (동보 가능) \n");
	printf(" 2-4-4. 문자플러스 IVR 파일 (파일 전송) \n");
	printf(" 2-4-5. 문자플러스 TTS변환  (동보 가능) \n");
	printf(" 2-4-6. 문자플러스 TTS변환  (파일 전송) \n");
	printf(" 2-4-7. 문자플러스 VXML파일 (동보 가능) \n");
	printf(" 2-4-8. 문자플러스 VXML파일 (파일 전송) \n");
	printf(" x. 이전화면  \n");
	printf(" 선택하세요 : ");
	gets( in_str );
	
	if ( strncmp( in_str, "2-4-1", 5 ) == 0 ) help_submit2_4_1();
	else if ( strncmp( in_str, "2-4-2", 5 ) == 0 ) help_submit2_4_2();
	else if ( strncmp( in_str, "2-4-3", 5 ) == 0 ) help_submit2_4_3();
	else if ( strncmp( in_str, "2-4-4", 5 ) == 0 ) help_submit2_4_4();
	else if ( strncmp( in_str, "2-4-5", 5 ) == 0 ) help_submit2_4_5();
	else if ( strncmp( in_str, "2-4-6", 5 ) == 0 ) help_submit2_4_6();
	else if ( strncmp( in_str, "2-4-7", 5 ) == 0 ) help_submit2_4_7();
	else if ( strncmp( in_str, "2-4-8", 5 ) == 0 ) help_submit2_4_8();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_4();
}



void 
help_submit2_4_1()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[1000], list1[1000], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	strcpy( list1, list );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_FILE_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_TEMP_TYPE,				// customer type
		0,							// file count
		"",      					// file name
		i,							// addr count 
		list,						// addr
		smsmsg, 						// sms
		"", 						// tts
		1, 							// content file count
		file,			// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_4();
}



void 
help_submit2_4_2()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_FILE_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_FILE_TYPE, 
		1,
		ufile, 
		0, 
		"", 
		smsmsg,
		"", 
		1, 
		infile, 
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_4();
}



void 
help_submit2_4_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[1000], list1[1000], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지 : ");
	gets( smsmsg );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_IVR_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		i,					// addr count 
		list,				// dest addr
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		file,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_4();
}



void 
help_submit2_4_4()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_VOICE, 
		SPV_IVR_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,
		repurl,
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		ufile,				// file name
		0,					// addr count 
		"",					// addr
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		infile,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_4();
}

*//*
//문자플러스즉시
void help_submit2_4_5()
{
	SDWORD meche_len=SQL_NTS;

	char rettel[15];
	char rettel_ptr[15];
	SDWORD rettel_len=SQL_NTS;
	char msg_db[500];
	char msg_db_ptr[500];
	SDWORD msg_db_len=SQL_NTS;
	char tel[12];
	char tel_ptr[12];
	SDWORD tel_len=SQL_NTS;
	SDWORD seq;
	SDWORD seq_ptr;
	SDWORD seq_len=SQL_NTS;
	char name_db[9];
	char name_db_ptr[9];
	SDWORD name_db_len=SQL_NTS;
	char j_no[18];
	char j_no_ptr[18];
	SDWORD j_no_len=SQL_NTS;
	char rdate[12];
	char rdate_ptr[12];
	SDWORD rdate_len=SQL_NTS;
	char rtime[12];
	char rtime_ptr[12];
	SDWORD rtime_len=SQL_NTS;
	char userid[18];
	char userid_ptr[18];
	SDWORD userid_len=SQL_NTS;
	char title[50];
	char title_ptr[50];
	SDWORD title_len=SQL_NTS;
	char msg_plus[80];
	char msg_plus_ptr[80];
	SDWORD msg_plus_len=SQL_NTS;
	char id_no[15];
	char id_no_ptr[15];
	SDWORD id_no_len=SQL_NTS;

	int gid;
	int gid_ptr;
	SDWORD gid_len=SQL_NTS;

	int	qnum,ret=0;
	int cnt=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR buff[]="SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,msg_plus,id_no,gid FROM goodshot.message2 where meche='5'";	
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);
	_ret=SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
				   (SDWORD)sizeof(rettel),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msg_plus_ptr,
			 (SDWORD)sizeof(msg_plus),&msg_plus_len);
	SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no),&id_no_len);
	SQLBindCol(hstmt,13, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid), &gid_len);
	_ret=SQLExecute(hstmt);  
	SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
    
	SDWORD seq_n=(int)seq_ptr;

	char tel_db[12];
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);	
	char tel_set2[18];
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char *name_db_n=strtok(name_set,"^");
    char *system_date=get_date();
	   
	char *_msg;
	_msg=msg_db_ptr;

	job_id=fn_x_submit(	
		seq_ptr, 
		JOBTYPE_VOICE, 
		SPV_TEMP_TEMP, 
		title_ptr, 
		0, 
		get_now(),
		get_now(),
		rettel_ptr,  
		"", 
		DEST_TEMP_TYPE,		// customer type 
		0,					// file count
		"",					// file name
		1,					// addr count 
		tel_set2,
		msg_plus_ptr, 			// sms
		_msg, 				// tts
		0, 					// content file count
		"",
		"",
		1, 
		1, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	
	SDWORD ret_len=SQL_NTS;
	char *msdate_db=get_date();
	char msdate[10];
	char *mstime_db=get_time4();
	char mstime[10];
	SDWORD msdate_db_len=SQL_NTS;
	SDWORD mstime_db_len=SQL_NTS;

	char meche;
	char *meche_ptr;
	char name_insert[9];
	
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR insert_data[500];
	sprintf((char *)insert_data,"insert into goodshot.message_bak2(seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,msg_plus,id_no,gid) values(%d,'%s','%s',5,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)",seq_n,j_no_ptr,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,_msg,title_ptr,tel_db,msg_plus_ptr,id_no_ptr,gid_ptr); 
		
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
    }


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR delete_message2[100];
	sprintf((char *)delete_message2,"delete from goodshot.message2 where seq=%d",seq_ptr);
	_ret=SQLPrepare(hstmt,delete_message2,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret =SQLFreeStmt(hstmt,SQL_DROP) ;
	_ret = SQLTransact(henv, hdbc, SQL_COMMIT);
	}
  }
  help_basic();
}*/

/*

void 
help_submit2_4_6()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], ufile[100], infile[100];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf("  문자메시지  : ");
	gets( smsmsg );
	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자화일: ");
	gets( ufile );


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_TEMP_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		ufile,					// file name
		0,						// addr count 
		"", 					// dest addr
		smsmsg,					// sms
		msg,					// tts
		0, 						// content file count
		"",					// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_4();
}



void 
help_submit2_4_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf( "문자 메시지 : " );
	gets( smsmsg );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 수신자리스트: ");
	gets(list);

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_VXML_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,
		repurl,
		DEST_TEMP_TYPE,			// customer type
		0,						// customer file count
		"",						// file name
		i,						// addr count 
		list,
		smsmsg,					// sms
		"", 					// tts
		1, 						// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret 
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_4();
}



void 
help_submit2_4_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], ufile[100], list1[1000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 수신자화일: ");
	gets( ufile );


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_VXML_FILE, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"",
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_4();
}



void 
help_submit2_5()
{
	printf("\n\n");
	printf(" 2-5-1. 문자메시지 (동보 가능) \n");
	printf(" 2-5-2. 문자메시지 (파일 전송)  \n");
	printf(" x. 이전화면  \n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-5-1", 5 ) == 0 ) help_submit2_5_1();
	else if ( strncmp( in_str, "2-5-2", 5 ) == 0 ) help_submit2_5_2();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_5();
}


*/


//문자예약 - neverdir OLD

void help_submit2_5_1()
{
	char rettel_ptr[12]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[1002]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char msdate_ptr[10]="";
	SDWORD msdate_len=SQL_NTS;
	char mstime_ptr[10]="";
	SDWORD mstime_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	char send_date[12]="";

	int		cnt= 0;
	int		ret=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR buff[BUFF_SIZE];
	sprintf((char *)buff,"SELECT rettel,msg,tel,name,j_no,rdate,rtime,userid,title,seq,msdate,mstime,id_no,gid FROM message2 where meche='2'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);

	_ret=SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	_ret=SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	_ret=SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	_ret=SQLBindCol(hstmt,4, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	_ret=SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	_ret=SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) rdate_ptr,  
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	_ret=SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	_ret=SQLBindCol(hstmt,8,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	_ret=SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	_ret=SQLBindCol(hstmt,10, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	_ret=SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate_ptr),&msdate_len);
	_ret=SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime_ptr),&mstime_len);
	_ret=SQLBindCol(hstmt,13,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	_ret=SQLBindCol(hstmt,14, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);

		int gid_n=gid_ptr;
		int seq_n=seq_ptr;
		char tel_db[12];
		char title_name[50]="";
		strcpy(title_name,title_ptr);
		strcpy(tel_db,tel_ptr);
		char *name_set=strcat(strtok(name_db_ptr," "),"^");
	    char *tel_set=strcat(name_set,tel_ptr);	
		char tel_set2[65];
		strcpy(tel_set2,tel_set);
		strtok(rettel_ptr," ");
		char msdate_db[9];
		strcpy(msdate_db,msdate_ptr);
		char mstime_db[10];
		strcpy(mstime_db,mstime_ptr);
		char *name_db_n=strtok(name_set,"^");
		strcat(strtok(msdate_ptr," "),mstime_ptr);
		strtok(msdate_ptr," ");
		strtok(tel_set2," ");
		char now[2]="";
		
	if(isNight()==0 && isNhic()==0)
	{
		puts("야간발송 error");
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR insert_data[INSERT_SIZE]="";
		sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel,ret_code,talktime,fee,page) values(%d,2,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',88,0,0,0)",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr ,title_name,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
		printf("insert_data=%s",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
		_ret=SQLExecute(hstmt);
	    _ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
			
		hstmt=0;
	    _ret=SQLAllocStmt(hdbc,&hstmt);
		{
	    UCHAR delete_data[DELETE_SIZE]="";
	    sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
		_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
        _ret=SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);

		}
	
  		printf("help_basic() Lev[문자예약]<-\n");
		LeaveCriticalSection( &cs_db );
		return;
	}


		int i;
		for (i=1002; i >= 0; --i) {
			if (msg_db_ptr[i] != ' ') {
				msg_db_ptr[i+1] = '\0';
				break;
			}
		}



printf("[title] : %s\n", title_name);
    fn_x_submit_4 (
		seq_n,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		title_name,
		0,
		msdate_ptr,
		msdate_ptr,
		rettel_ptr,	//rettel
		"http^//www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,   //cnt
		tel_set2,//phone_set
		msg_db_ptr,
		"",
		0,
		"",
		"",
		0,
		0,
		0,
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,
		"",
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);

		printf(" SUBMIT 완료 되었습니다.\n");


			
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR insert_data[INSERT_SIZE]="";
		sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,2,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr ,title_name,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
		printf("insert_data=%s",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
		_ret=SQLExecute(hstmt);
	    _ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
			
		hstmt=0;
	    _ret=SQLAllocStmt(hdbc,&hstmt);
		{
	    UCHAR delete_data[DELETE_SIZE]="";
	    sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
		_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
        _ret=SQLExecute(hstmt);
		_ret = SQLFreeStmt(hstmt,SQL_DROP);

		}
	}
  	printf("help_basic() Lev[문자예약]<-\n");
	LeaveCriticalSection( &cs_db );
	return;
}


/*
//문자예약

void help_submit2_5_1()
{
SDWORD meche_len=SQL_NTS;
char rettel_ptr[12]="";
SDWORD rettel_len=SQL_NTS;
char msg_db_ptr[80]="";
SDWORD msg_db_len=SQL_NTS;
char tel_ptr[12]="";
SDWORD tel_len=SQL_NTS;
SDWORD seq_ptr=0;
SDWORD seq_len=SQL_NTS;
char name_db_ptr[9]="";
SDWORD name_db_len=SQL_NTS;
char j_no_ptr[18]="";
SDWORD j_no_len=SQL_NTS;
char rdate_ptr[12]="";
SDWORD rdate_len=SQL_NTS;
char rtime_ptr[12]="";
SDWORD rtime_len=SQL_NTS;
char userid_ptr[18]="";
SDWORD userid_len=SQL_NTS;
char title_ptr[50]="";
SDWORD title_len=SQL_NTS;
char msdate_ptr[10]="";
SDWORD msdate_len=SQL_NTS;
char mstime_ptr[10]="";
SDWORD mstime_len=SQL_NTS;
char id_no_ptr[15]="";
SDWORD id_no_len=SQL_NTS;
int gid_ptr=0;
SDWORD gid_len=SQL_NTS;
char send_date[12]="";



	int		cnt= 0;
	int		ret=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR buff[500];
	sprintf((char *)buff,"SELECT rettel,msg,tel,name,j_no,rdate,rtime,userid,title,seq,msdate,mstime,id_no,gid FROM goodshot.message2 where meche='2'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);

	_ret=SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	_ret=SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	_ret=SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	_ret=SQLBindCol(hstmt,4, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	_ret=SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	_ret=SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	_ret=SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	_ret=SQLBindCol(hstmt,8,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	_ret=SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	_ret=SQLBindCol(hstmt,10, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	_ret=SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate_ptr),&msdate_len);
	_ret=SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime_ptr),&mstime_len);
	_ret=SQLBindCol(hstmt,13,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	_ret=SQLBindCol(hstmt,14, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);

		int gid_n=gid_ptr;
		int seq_n=seq_ptr;
		char tel_db[12];
		strcpy(tel_db,tel_ptr);
		char *name_set=strcat(strtok(name_db_ptr," "),"^");
	    char *tel_set=strcat(name_set,tel_ptr);	
		char tel_set2[18];
		strcpy(tel_set2,tel_set);
		strtok(rettel_ptr," ");
		char msdate_db[9];
		strcpy(msdate_db,msdate_ptr);
		char mstime_db[10];
		strcpy(mstime_db,mstime_ptr);
		char *name_db_n=strtok(name_set,"^");
		strcat(strtok(msdate_ptr," "),mstime_ptr);
		strtok(msdate_ptr," ");
		strtok(tel_set2," ");
		printf("msdate_ptr=%s\n",msdate_ptr);
//		char *_msg=msg_db_ptr;

    job_id=fn_x_submit( 
		seq_n,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		title_ptr,
		0,
		msdate_ptr,
		msdate_ptr,
		rettel_ptr,	//rettel
		"http^//www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,   //cnt
		tel_set2,//phone_set
		msg_db_ptr,
		"",
		0,
		"",
		"",
		0,
		0,
		0,
		"",
		0,
		0,
		&ret
	    );

		printf(" SUBMIT 완료 되었습니다. ");


			
		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR insert_data[700]="";
		sprintf((char *)insert_data,"insert into goodshot.message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,2,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
		printf("insert_data=%s",insert_data);
		_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
		_ret=SQLExecute(hstmt);
		printf("_ret=%d",_ret);
	    _ret = SQLFreeStmt(hstmt,SQL_DROP);
		}
			
		hstmt=0;
	    _ret=SQLAllocStmt(hdbc,&hstmt);
		{
	    UCHAR delete_data[100]="";
	    sprintf((char *)delete_data,"delete from goodshot.message2 where seq=%d",seq_n);
		_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
        _ret=SQLExecute(hstmt);
		printf("del=%d",_ret);
		
		_ret = SQLFreeStmt(hstmt,SQL_DROP);
//		_ret = SQLTransact(henv, hdbc, SQL_COMMIT);
		}
	}
	return;
}
*/

/*

void 
help_submit2_5_2()
{
	char	msg_num[20], title[20], repnum[20], msg[100], filename[100];
	char	re_start[20], re_end[20];
	int		msgnum;

	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 메시지 : ");
	gets( msg );
	printf(" 수신자파일 : ");
	gets( filename );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_5_2();
	}


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_SMS, 
		SMS_TEMP_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"http^//www.xroshot.com", 
		DEST_FILE_TYPE, 
		1,
		filename, 
		0, 
		"", 
		msg, 
		"", 
		0, 
		"", 
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_5();
}



void 
help_submit2_6()
{
	printf(" \n\n " );
	printf(" 2-6-1. 음성메시지 음성파일 (동보 가능) \n");
	printf(" 2-6-2. 음성메시지 음성파일 (파일 전송) \n");
	printf(" 2-6-3. 음성메시지 IVR 파일 (동보 가능) \n");
	printf(" 2-6-4. 음성메시지 IVR 파일 (파일 전송) \n");
	printf(" 2-6-5. 음성메시지 TTS변환  (동보 가능) \n");
	printf(" 2-6-6. 음성메시지 TTS변환  (파일 전송) \n");
	printf(" 2-6-7. 음성메시지 VXML파일 (동보 가능) \n");
	printf(" 2-6-8. 음성메시지 VXML파일 (파일 전송) \n");
	printf(" x. 이전화면  \n");
	gets( in_str );
	if ( strncmp( in_str, "2-6-1", 5 ) == 0 ) help_submit2_6_1();
	else if ( strncmp( in_str, "2-6-2", 5 ) == 0 ) help_submit2_6_2();
	else if ( strncmp( in_str, "2-6-3", 5 ) == 0 ) help_submit2_6_3();
	else if ( strncmp( in_str, "2-6-4", 5 ) == 0 ) help_submit2_6_4();
	else if ( strncmp( in_str, "2-6-5", 5 ) == 0 ) help_submit2_6_5();
	else if ( strncmp( in_str, "2-6-6", 5 ) == 0 ) help_submit2_6_6();
	else if ( strncmp( in_str, "2-6-7", 5 ) == 0 ) help_submit2_6_7();
	else if ( strncmp( in_str, "2-6-8", 5 ) == 0 ) help_submit2_6_8();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_6();
}



void 
help_submit2_6_1()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			file[100], list[1000], list1[1000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_1();
	}

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_FILE_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_TEMP_TYPE,				// customer type
		0,							// file count
		"",      					// file name
		i,							// addr count 
		list,						// addr
		"", 						// sms
		"", 						// tts
		1, 							// content file count
		file,			// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_6();
}
*/


//음성 파일첨부 예약
void help_submit2_6_1()
{
	
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[1002]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char msdate_ptr[15]="";
	SDWORD msdate_len=SQL_NTS;
	char mstime_ptr[10]="";
	SDWORD mstime_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;
	UCHAR delete_data[DELETE_SIZE]="";
	char send_data[20]="";
	char file[40];

	int		i = 0;
	int		ret=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,userid,title,msdate,mstime,id_no,gid FROM message2 where meche='6'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);

	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,8,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate_ptr),&msdate_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime_ptr),&mstime_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,12, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);

	_ret=SQLExecute(hstmt);  
   	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);		


		int gid_n=gid_ptr;
		int seq_n=seq_ptr;
		char tel_db[12];
		char title_name[50]="";
		strcpy(title_name,title_ptr);
		strcpy(tel_db,tel_ptr);
		char *name_set=strcat(strtok(name_db_ptr," "),"^");
	    char *tel_set=strcat(name_set,tel_ptr);	
		char tel_set2[65];
		strcpy(tel_set2,tel_set);
		strtok(rettel_ptr," ");
		char msdate_db[9];
		strcpy(msdate_db,msdate_ptr);
		char mstime_db[10];
		strcpy(mstime_db,mstime_ptr);
		char *name_db_n=strtok(name_set,"^");
		strcat(strtok(msdate_ptr," "),mstime_ptr);
		strtok(msdate_ptr," ");
		printf("msdate=%s\n",msdate_ptr);
		strtok(tel_set2," ");

		strcpy(file, FILE_VOICE_WAV);
		strcat(file, msg_db_ptr);
		strtok(file," ");
//printf("title_ptr=\\%s\\\n,tel_se2=\\%s\\\n,file=\\%s\\\n,msdate_ptr=\\%s\\\n",title_ptr,tel_set2,file,msdate_ptr);
	fn_x_submit_4 (
		seq_n, 
		JOBTYPE_VOICE, 
		VOICE_FILE_TEMP, 
		title_name, 
		0, 
		msdate_ptr,//get_now()
		msdate_ptr,//get_now()
		rettel_ptr,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		0,					// addr count 
		tel_set2,
		"", 				// sms
		"", 			// tts
		1, 					// content file count
		file,
		"",
		0, 
		0, 
		0,
		"", 
		0,  
		0,  
		0,
		"",
		"",	
		"",	
		0,	
		"251186",
		&ret);
	
	printf(" SUBMIT 완료 되었습니다. \n" );
	printf("telset : %s, msdate_ptr : %s\n", tel_set2, msdate_ptr);

	hstmt=0;
    _ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into dbo.message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,4,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_name,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	printf("delete_data=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
    _ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}

	
  }
  	printf("help_basic() Lev[음성예약]<-\n");
	LeaveCriticalSection( &cs_db );
	
	return;

	
}

/*
void 
help_submit2_6_2()
{
	char	msg_num[20], title[20], repnum[20], 
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_2();
	}


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_FILE_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_FILE_TYPE, 
		1,
		ufile, 
		0, 
		"", 
		"", 
		"", 
		1, 
		infile, 
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_6();
}



void 
help_submit2_6_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			file[100], list[1000], list1[1000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_3();
	}

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_IVR_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		i,					// addr count 
		list,				// dest addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		file,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_6();
}



void 
help_submit2_6_4()
{
	char	msg_num[20], title[20], repnum[20], 
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_4();
	}


	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_VOICE, 
		VOICE_IVR_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		ufile,				// file name
		0,					// addr count 
		"",					// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_6();
}

*/
//음성예약 - neveridr OLD
void help_submit2_6_5()
{
	char rettel_ptr[15]="";
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[1002]="";
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12]="";
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr=0;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50]="";
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18]="";
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12]="";
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12]="";
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18]="";
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50]="";
	SDWORD title_len=SQL_NTS;
	char msdate_ptr[9]="";
	SDWORD msdate_len=SQL_NTS;
	char mstime_ptr[10]="";
	SDWORD mstime_len=SQL_NTS;
	char id_no_ptr[15]="";
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr=0;
	SDWORD gid_len=SQL_NTS;
	char now[2]="";
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;
	UCHAR delete_data[DELETE_SIZE]="";

	int		i = 0;
	int		ret=0;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,msdate,mstime,id_no,gid FROM message2 where meche='4'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);
	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate_ptr),&msdate_len);
	SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime_ptr),&mstime_len);
	SQLBindCol(hstmt,13,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,14, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);

	_ret=SQLExecute(hstmt);  
   	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);		

		int gid_n=gid_ptr;
		int seq_n=seq_ptr;
		char tel_db[12];
		char title_name[50]="";
		strcpy(title_name,title_ptr);
		strcpy(tel_db,tel_ptr);
		char *name_set=strcat(strtok(name_db_ptr," "),"^");
	    char *tel_set=strcat(name_set,tel_ptr);	
		char tel_set2[65];
		strcpy(tel_set2,tel_set);
		strtok(rettel_ptr," ");
		char msdate_db[9];
		strcpy(msdate_db,msdate_ptr);
		char mstime_db[10];
		strcpy(mstime_db,mstime_ptr);
		char *name_db_n=strtok(name_set,"^");
		strcat(strtok(msdate_ptr," "),mstime_ptr);
		strtok(msdate_ptr," ");
		printf("msdate=%s\n",msdate_ptr);
		strtok(tel_set2," ");
		strncpy(now,mstime_db,2);
#if defined(NIGHT)		
	if(isNight2(now)==0 && isNhic()==0)
		{
			puts("야간발송 error");	
			hstmt=0;
			_ret=SQLAllocStmt(hdbc,&hstmt);
			{
			sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel,ret_code,talktime,fee,page) values(%d,4,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',88,0,0,0)",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_name,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
			printf("insert_data=%s\n",insert_data);
			_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
			_ret=SQLExecute(hstmt);
			_ret = SQLFreeStmt(hstmt,SQL_DROP);
			}
					
			hstmt=0;
			_ret=SQLAllocStmt(hdbc,&hstmt);
			{
			sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
			printf("delete_data=%s\n",delete_data);
			_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
			_ret=SQLExecute(hstmt);
			_ret = SQLFreeStmt(hstmt,SQL_DROP);
			}

  			printf("help_basic() Lev[음성예약]<-\n");
			LeaveCriticalSection( &cs_db );
			return;
		}
#endif

		int i;

		for (i=1002; i >= 0; --i) {
			if (msg_db_ptr[i] != ' ') {
				msg_db_ptr[i+1] = '\0';
				break;
			}
		}
	
	fn_x_submit_4 (
		seq_n,
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
		title_name,//title_ptr 
		0, 
		msdate_ptr,//msdate
		msdate_ptr,//msdate
		rettel_ptr,  //rettel_ptr
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,
		"",
		0,   //cnt  //1213 0 -> 1
		tel_set2,//tel_set2,//phone_set
		"",
		msg_db_ptr,//msg_db_ptr,
		0,
		"",
		"",
		0,
		0,
		0,
		"", 
		0,  
		0,  
		0,
		"",
		"",	
		"",	
		0,	
		"251186",	
		&ret);

	printf(" SUBMIT 완료 되었습니다. \n" );
	printf("telset : %s, msdate_ptr : %s\n", tel_set2, msdate_ptr);

	hstmt=0;
    _ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into message_bak2(seq,meche,gid,userid,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,j_no,rettel) values(%d,4,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",seq_n,gid_n,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_name,tel_db,id_no_ptr,j_no_ptr,rettel_ptr); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
			
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	printf("delete_data=%s\n",delete_data);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
    _ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
  }
  	printf("help_basic() Lev[음성예약]<-\n");
	LeaveCriticalSection( &cs_db );
	return;
}

/*
void 
help_submit2_6_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 수신자리스트: ");
	gets(list);
	printf( "첨부 화일 : " );
	gets(infile);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_7();
	}


	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_VXML_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_TEMP_TYPE,			// customer type
		0,						// customer file count
		"",						// file name
		i,						// addr count 
		list,
		"", 					// sms
		"", 					// tts
		1, 						// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret 
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_6();
}


void 
help_submit2_6_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], 
			msg[1000], ufile[100], list1[1000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_8();
	}



	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_VXML_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_6();
}



void 
help_submit2_7()
{
	printf(" \n\n");
	printf(" 2-7-1. 팩스메시지 (동보 가능) \n");
	printf(" 2-7-2. 팩스메시지 (파일 전송) \n");
	printf(" x. 이전화면\n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-7-1", 5 ) == 0 ) help_submit2_7_1();
	else if ( strncmp( in_str, "2-7-2", 5 ) == 0 ) help_submit2_7_2();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_7();
}


*/
void help_submit2_7_1()
{
	SDWORD meche_len=SQL_NTS;
	char rettel_ptr[15];
	SDWORD rettel_len=SQL_NTS;
	char msg_db_ptr[500];
	SDWORD msg_db_len=SQL_NTS;
	char tel_ptr[12];
	SDWORD tel_len=SQL_NTS;
	SDWORD seq_ptr;
	SDWORD seq_len=SQL_NTS;
	char name_db_ptr[50];
	SDWORD name_db_len=SQL_NTS;
	char j_no_ptr[18];
	SDWORD j_no_len=SQL_NTS;
	char rdate_ptr[12];
	SDWORD rdate_len=SQL_NTS;
	char rtime_ptr[12];
	SDWORD rtime_len=SQL_NTS;
	char userid_ptr[18];
	SDWORD userid_len=SQL_NTS;
	char title_ptr[50];
	SDWORD title_len=SQL_NTS;
	char msdate_ptr[9];
	SDWORD msdate_len=SQL_NTS;
	char mstime_ptr[10];
	SDWORD mstime_len=SQL_NTS;
	char id_no_ptr[15];
	SDWORD id_no_len=SQL_NTS;
	int gid_ptr;
	SDWORD gid_len=SQL_NTS;
////////
	UCHAR buff[BUFF_SIZE]="";
	UCHAR insert_data[INSERT_SIZE]="";;
////////	
	int		i = 0;
	int		ret=0;
	char file[40];

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR buff[BUFF_SIZE];
	sprintf((char *)buff,"SELECT rettel,msg,tel,seq,name,j_no,rdate,rtime,userid,title,msdate,mstime,id_no,gid FROM message2 where meche='8'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);

	SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel_ptr),&rettel_len);
	SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db_ptr),&msg_db_len);
	SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel_ptr),&tel_len);
	SQLBindCol(hstmt,4, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq_ptr), &seq_len);
	SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db_ptr), &name_db_len);
	SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no_ptr), &j_no_len);
	SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate_ptr), &rdate_len);
	SQLBindCol(hstmt,8, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime_ptr), &rtime_len);
	SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid_ptr),&userid_len);
	SQLBindCol(hstmt,10,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title_ptr),&title_len);
	SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate_ptr),&msdate_len);
	SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime_ptr),&mstime_len);
	SQLBindCol(hstmt,13,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no_ptr),&id_no_len);
	SQLBindCol(hstmt,14, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid_ptr), &gid_len);
	
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);	
	
	int seq_n=seq_ptr;
	int gid_n=gid_ptr;
	char tel_db[12];
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
    char *tel_set=strcat(name_set,tel_ptr);	

	char tel_set2[65];
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char msdate_db[9];
	strcpy(msdate_db,msdate_ptr);
	char mstime_db[10];
	strcpy(mstime_db,mstime_ptr);
	
	char *name_db_n=strtok(name_set,"^");
	strncat(msdate_ptr,mstime_ptr,4);
	strtok(msdate_ptr," ");
	strcpy(file, FILE_FAX);
	strtok(msg_db_ptr," ");
	strcat(file, msg_db_ptr);
	strtok(tel_set2," ");
//printf("title_ptr=\\%s\\\n,msdate_ptr=\\%s\\\n,telset2=\\%s\\\n,fiel=\\%s\\\n",title_ptr,msdate_ptr,tel_set2,file);
fn_x_submit_4 (
		seq_n, 
		JOBTYPE_FAX, 
		FAX_TEXT_TEMP, 
		title_ptr, 
		0, //스케쥴 타입
		msdate_ptr,//스케쥴 시작시간
		msdate_ptr,//스케쥴 종료시간
		rettel_ptr,  //회신번호
		"",  //회신 URL
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file info name
		0,					// addr count  
		tel_set2,
		"", 				// sms
		"", 			// tts
		1, 					// content file count
		file,//첨부파일 이름
		"",//임시 파일
		0, //답변받기 
		0, //답변 개수
		0,//사용x
		"", 
		0,  
		0,  
		0,
		"",
		"",	
		"",	
		0,	
		"251186",		
		&ret);


	printf(" SUBMIT 완료 되었습니다. \n" );
	
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR insert_data[INSERT_SIZE];
	sprintf((char *)insert_data,"insert into message_bak2(seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,id_no,gid,rettel) values(%d,'%s','%s',8,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s')",seq_n,j_no_ptr,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,msg_db_ptr,title_ptr,tel_db,id_no_ptr,gid_n,rettel_ptr); 
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
			
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR delete_data[DELETE_SIZE];
	sprintf((char *)delete_data,"delete from message2 where seq=%d",seq_n);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	_ret = SQLTransact(henv, hdbc, SQL_COMMIT);
	}
  }
  printf("help_basic() Lev[팩스예약]<-\n");
  LeaveCriticalSection( &cs_db );
  return;
}


/*
void 
help_submit2_7_2()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], ufile[100],
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf( "첨부 화일 : " );
	gets( infile );
	printf(" 수신자화일 : ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_7_2();
	}


	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_FAX, 
		FAX_TEXT_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"", 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_7();
}



void 
help_submit2_8()
{
	printf(" \n\n ");
	printf(" 2-8-1. 문자플러스 음성파일 (동보 가능) \n");
	printf(" 2-8-2. 문자플러스 음성파일 (파일 전송) \n");
	printf(" 2-8-3. 문자플러스 IVR 파일 (동보 가능) \n");
	printf(" 2-8-4. 문자플러스 IVR 파일 (파일 전송) \n");
	printf(" 2-8-5. 문자플러스 TTS변환  (동보 가능) \n");
	printf(" 2-8-6. 문자플러스 TTS변환  (파일 전송) \n");
	printf(" 2-8-7. 문자플러스 VXML파일 (동보 가능) \n");
	printf(" 2-8-8. 문자플러스 VXML파일 (파일 전송) \n");
	printf(" x. 이전화면  \n");
	printf(" 선택하세요 : ");
	gets( in_str );
	
	if ( strncmp( in_str, "2-8-1", 5 ) == 0 ) help_submit2_8_1();
	else if ( strncmp( in_str, "2-8-2", 5 ) == 0 ) help_submit2_8_2();
	else if ( strncmp( in_str, "2-8-3", 5 ) == 0 ) help_submit2_8_3();
	else if ( strncmp( in_str, "2-8-4", 5 ) == 0 ) help_submit2_8_4();
	else if ( strncmp( in_str, "2-8-5", 5 ) == 0 ) help_submit2_8_5();
	else if ( strncmp( in_str, "2-8-6", 5 ) == 0 ) help_submit2_8_6();
	else if ( strncmp( in_str, "2-8-7", 5 ) == 0 ) help_submit2_8_7();
	else if ( strncmp( in_str, "2-8-8", 5 ) == 0 ) help_submit2_8_8();
//	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_8();
}



void 
help_submit2_8_1()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[1000], list1[1000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	qnum = atoi( in_str );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_1();
	}

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_FILE_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		repurl, 
		DEST_TEMP_TYPE,				// customer type
		0,							// file count
		"",      					// file name
		i,							// addr count 
		list,						// addr
		smsmsg, 						// sms
		"", 						// tts
		1, 							// content file count
		file,			// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_8();
}



void 
help_submit2_8_2()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	qnum = atoi( in_str );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_2();
	}


	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_FILE_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		repurl, 
		DEST_FILE_TYPE, 
		1,
		ufile, 
		0, 
		"", 
		smsmsg,
		"", 
		1, 
		infile, 
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_8();
}



void 
help_submit2_8_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[1000], list1[1000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지 : ");
	gets( smsmsg );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_3();
	}

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_IVR_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		repurl, 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		i,					// addr count 
		list,				// dest addr
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		file,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_8();
}



void 
help_submit2_8_4()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;

	printf(" \n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf("응답유형:[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf( "입력 : ");
	gets( in_str );
	reptype = atoi( in_str );
	printf(" 설문항목수: ");
	gets( in_str );
	qnum = atoi( in_str );
	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_4();
	}


	job_id=fn_x_submit(	msgnum, 
		JOBTYPE_VOICE, 
		SPV_IVR_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,
		repurl,
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		ufile,				// file name
		0,					// addr count 
		"",					// addr
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		infile,				// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	help_submit2_8();
}

*//*
//문자플러스예약
void help_submit2_8_5()
{
SDWORD meche_len=SQL_NTS;
char rettel[12];
char rettel_ptr[12];
SDWORD rettel_len=SQL_NTS;
char msg_db[80];
char msg_db_ptr[80];
SDWORD msg_db_len=SQL_NTS;
char tel[12];
char tel_ptr[12];
SDWORD tel_len=SQL_NTS;
SDWORD seq;
SDWORD seq_ptr;
SDWORD seq_len=SQL_NTS;
char name_db[9];
char name_db_ptr[9];
SDWORD name_db_len=SQL_NTS;
char j_no[18];
char j_no_ptr[18];
SDWORD j_no_len=SQL_NTS;
char rdate[12];
char rdate_ptr[12];
SDWORD rdate_len=SQL_NTS;
char rtime[12];
char rtime_ptr[12];
SDWORD rtime_len=SQL_NTS;
char userid[18];
char userid_ptr[18];
SDWORD userid_len=SQL_NTS;
char title[50];
char title_ptr[50];
SDWORD title_len=SQL_NTS;
char msdate[9];
char msdate_ptr[9];
SDWORD msdate_len=SQL_NTS;
char mstime[10];
char mstime_ptr[10];
SDWORD mstime_len=SQL_NTS;
char msg_plus[80];
char msg_plus_ptr[80];
SDWORD msg_plus_len=SQL_NTS;
int	  i = 0;
int ret=0;
char id_no[15];
char id_no_ptr[15];
SDWORD id_no_len=SQL_NTS;
int gid;
int gid_ptr;
SDWORD gid_len=SQL_NTS;

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR buff[500];
	sprintf((char *)buff,"SELECT rettel,msg,tel,name,j_no,rdate,rtime,userid,title,seq,msdate,mstime,msg_plus,id_no,gid FROM goodshot.message2 where meche='6'");
	_ret = SQLPrepare(hstmt,buff,SQL_NTS);
	_ret=SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) rettel_ptr,
			 (SDWORD)sizeof(rettel),&rettel_len);

	_ret=SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) msg_db_ptr,
			 (SDWORD)sizeof(msg_db),&msg_db_len);

	_ret=SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) tel_ptr,
			 (SDWORD)sizeof(tel),&tel_len);

	_ret=SQLBindCol(hstmt,4, SQL_C_CHAR,(PTR) name_db_ptr, 
			 (SDWORD)sizeof(name_db), &name_db_len);
	
	_ret=SQLBindCol(hstmt,5, SQL_C_CHAR,(PTR) j_no_ptr, 
			 (SDWORD)sizeof(j_no), &j_no_len);
	
	_ret=SQLBindCol(hstmt,6, SQL_C_CHAR,(PTR) rdate_ptr, 
			 (SDWORD)sizeof(rdate), &rdate_len);
	
	_ret=SQLBindCol(hstmt,7, SQL_C_CHAR,(PTR) rtime_ptr, 
			 (SDWORD)sizeof(rtime), &rtime_len);
	
	_ret=SQLBindCol(hstmt,8,SQL_C_CHAR,(PTR) userid_ptr,
			 (SDWORD)sizeof(userid),&userid_len);
	
	_ret=SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) title_ptr,
			 (SDWORD)sizeof(title),&title_len);
	
	_ret=SQLBindCol(hstmt,10, SQL_C_LONG, (PTR) &seq_ptr, 
			 (SDWORD)sizeof(seq), &seq_len);

	_ret=SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) msdate_ptr,
			 (SDWORD)sizeof(msdate),&msdate_len);

	_ret=SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) mstime_ptr,
			 (SDWORD)sizeof(mstime),&mstime_len);

	_ret=SQLBindCol(hstmt,13,SQL_C_CHAR,(PTR) msg_plus_ptr,
			 (SDWORD)sizeof(msg_plus),&msg_plus_len);

	_ret=SQLBindCol(hstmt,14,SQL_C_CHAR,(PTR) id_no_ptr,
			 (SDWORD)sizeof(id_no),&id_no_len);

	_ret=SQLBindCol(hstmt,15, SQL_C_LONG, (PTR) &gid_ptr, 
			 (SDWORD)sizeof(gid), &gid_len);
	
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);	
	
	SDWORD seq_n=(int)seq_ptr;
    char tel_db[12];
	strcpy(tel_db,tel_ptr);
	char *name_set=strcat(strtok(name_db_ptr," "),"^");
	char *tel_set=strcat(name_set,tel_ptr);	
	char tel_set2[18];
	strcpy(tel_set2,tel_set);
	strtok(rettel_ptr," ");
	char msdate_db[9];
	strcpy(msdate_db,msdate_ptr);
	char mstime_db[10];
	strcpy(mstime_db,mstime_ptr);
	char *name_db_n=strtok(name_set,"^");
		
	char *system_date=get_date();
	strncat(msdate_ptr,mstime_ptr,4);
			
	char *_msg;
	_msg=msg_db_ptr;

	job_id=fn_x_submit(	seq_ptr, 
		JOBTYPE_VOICE, 
		SPV_TEMP_TEMP, 
		title, 
		0, 
		msdate_ptr,
		msdate_ptr,
		rettel_ptr,  
		"http^//www.xroshot.com", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		1,					// addr count 
		tel_set2,
		msg_plus, 				// sms
		_msg, 				// tts
		0, 					// content file count
		"",
		"",
		1, 
		1, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );

	SDWORD ret_len=SQL_NTS;
	char msdate[10];
	char mstime[10];
	SDWORD msdate_db_len=SQL_NTS;
	SDWORD mstime_db_len=SQL_NTS;
	char meche;
	char *meche_ptr;
	char name_insert[9];
		
	hstmt=0;
    _ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR insert_data[500];
	sprintf((char *)insert_data,"insert into goodshot.message_bak2(seq,j_no,userid,meche,msdate,mstime,rdate,rtime,name,msg,title,tel,msg_plus,id_no,gid) values(%d,'%s','%s',6,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d)",seq_n,j_no_ptr,userid_ptr,msdate_db,mstime_db,rdate_ptr,rtime_ptr,name_db_n,_msg,title_ptr,tel_db,msg_plus_ptr,id_no,gid_ptr); 
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
			
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR delete_data[100];
	sprintf((char *)delete_data,"delete from goodshot.message2 where seq=%d",seq_ptr);
	_ret=SQLPrepare(hstmt,delete_data,SQL_NTS); 
    _ret=SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	_ret = SQLTransact(henv, hdbc, SQL_COMMIT);
	}
  }
  help_basic();
}
*/
/*

void 
help_submit2_8_6()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], ufile[100], infile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf("  문자메시지  : ");
	gets( smsmsg );
	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자화일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_6();
	}



	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_TEMP_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		repurl, 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		ufile,					// file name
		0,						// addr count 
		"", 					// dest addr
		smsmsg,					// sms
		msg,					// tts
		0, 						// content file count
		"",					// content file name
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_8();
}


void 
help_submit2_8_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], list[1000], list1[1000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf( "문자 메시지 : " );
	gets( smsmsg );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 수신자리스트: ");
	gets(list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_7();
	}


	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_VXML_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,
		repurl,
		DEST_TEMP_TYPE,			// customer type
		0,						// customer file count
		"",						// file name
		i,						// addr count 
		list,
		smsmsg,					// sms
		"", 					// tts
		1, 						// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret 
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_8();
}


void 
help_submit2_8_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[1000],
			msg[1000], ufile[100], list1[1000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 수신자화일: ");
	gets( ufile );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_8();
	}



	job_id = fn_x_submit( msgnum, 
		JOBTYPE_VOICE, 
		SPV_VXML_FILE, 
		title, 
		0, 
		re_start,
		re_end,
		repnum,  
		repurl, 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		ufile,				// file name
		0,					// addr count 
		"",
		smsmsg,				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		&ret
	);
	printf(" SUBMIT 완료 되었습니다. \n" );
	if ( ret == 0 ) printf( " 성공적으로 수행되었습니다. \n");
	else printf( "에러 발생입니다. \n" );
	help_submit2_8();
}

*/


/*
void 
help_status3()
{
	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}

	printf(" \n\n ");
	printf(" 3-1. 전송결과 요청 \n");
	printf(" 3-2. 상세 전송결과 요청 \n");
	printf(" x. 이전화면  \n");

	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "3-1", 3 ) == 0 ) help_status3_1();
	else if ( strncmp( in_str, "3-2", 3 ) == 0 ) help_status3_2();
	else if ( in_str[0] == 'x' ) help_basic();
	else help_status3();
}


void 
help_status3_1()
{
	int		jobid;
	char	stime[20];
	int		period;
	STATUSINFO  ret_stat;

	printf(" JOBID : ");
	gets( in_str );
	jobid = atoi( in_str );

	printf(" Start Time (yyyymmdd) : ");
	gets( stime );

	printf(" PERIOD : ");
	gets( in_str );
	period = atoi( in_str );

	ret_stat = fn_x_status( 
		stime,
		period,
		&ret
	);

	if (ret == 0) {
		printf( "Status Complete..... JOB ID : %d \n", job_id );

		printf( "msgid[%d], jobid[%d], subject[%s] \n",
		ret_stat.msgid, ret_stat.jobid, ret_stat.subject);
		printf( "stime[%s], etime[%s], reserved count[%d] \n",
		ret_stat.schedule_stime, ret_stat.schedule_etime, ret_stat.reserved_count);
		printf( "content_size[%d], success[%d], fail[%d] \n",
		ret_stat.content_size, ret_stat.success_count, ret_stat.fail_count);
	}

	help_status3();
}

void help_status3_2()
{
	int		jobid;
	STATUSDETAIL_INFO  ret_stat;

	printf(" JOBID : ");
	gets( in_str );
	jobid = atoi( in_str );

	ret_stat = fn_x_statusd( jobid, &ret);

	if (ret == 0) { 
		printf( "Status_Detail Complete..... JOB ID : %d \n", job_id );

		printf( "msgid[%d], jobid[%d], count[%d] \n",
		ret_stat.msgid, ret_stat.jobid, ret_stat.result_count);
		printf( "Result Content = %s\n", ret_stat.result_content);
	}

	help_status3();
}
*/

/*   취소를 위한 스래드 함수   */

DWORD WINAPI help_cancel4(void *)
{
/*	// 선언 
	int ret_cancel, jobid_cancel;
	// SQL 관련 변수 
	UCHAR select_buf[80] = "";
	SDWORD jobid_ptr,	jobid_len = SQL_NTS;
//	char jobid[80];
	SDWORD seq_ptr,		seq_len = SQL_NTS;
	//char meche[4]="";

	SQLRETURN sql_res, _ret;
	SDWORD gid_ptr;
	SDWORD gid_len = SQL_NTS;
//	SDWORD ret_ptr;
//	SDWORD ret_len = SQL_NTS;

	HSTMT		hstmt_gc;
	char *meche="";
	SDWORD meche_len=SQL_NTS;

	Sleep(1000);

	while(1)
	{
		//Sleep(100);

		EnterCriticalSection( &cs_db );

//printf("[CANCEL] SELECT->\n");
					
		meche=(char *)malloc(sizeof(char)*2);
		
		hstmt_gc = 0;
		SQLAllocStmt(hdbc,&hstmt_gc);
		{
			UCHAR select[80]="SELECT gid , meche FROM cancel_gid where meche='99'";
			//UCHAR select[55]="SELECT gid , meche FROM goodshot.cancel_gid";

			SQLPrepare(hstmt_gc, select, SQL_NTS);

			SQLBindCol(hstmt_gc, 1, SQL_C_LONG,(PTR) &gid_ptr,
					 (SDWORD)sizeof(gid_ptr), &gid_len);
			
			SQLBindCol(hstmt_gc, 2, SQL_C_CHAR,(PTR) meche,
					 (SDWORD)sizeof(meche), &meche_len);

			sql_res=SQLExecute(hstmt_gc);
			_ret=SQLFetch(hstmt_gc);

			// 데이타가 없으므로...
			if ( _ret == SQL_NO_DATA_FOUND || _ret == 100 ) {
				SQLFreeStmt(hstmt_gc,SQL_DROP);
				// SQL 동기화 
				free(meche);
				LeaveCriticalSection( &cs_db );
				//printf("[CANCEL] SELECT<-\n");
				Sleep(20);
				//break;
				continue;	
			}

			_ret=SQLFreeStmt(hstmt_gc ,SQL_DROP);	
		} // DB Block
		

		LeaveCriticalSection( &cs_db );
	//	printf("취소 시작 ... \n");
		//getchar();
		// gid 질의 
		// SELECT !!
		EnterCriticalSection( &cs_db );
	//	printf("DB Enter !! \n");

		hstmt_c=0;
		sql_res = SQLAllocStmt(hdbc, &hstmt_c);
		{
			//UCHAR select_gid[]="select gid from goodshot.message2 where meche='9'";
			UCHAR select_gid[]="select gid from cancel_gid where meche='99'";
			
			sql_res = SQLPrepare(hstmt_c, (UCHAR *)select_gid, SQL_NTS);

			SQLBindCol( hstmt_c,1, SQL_C_LONG, (PTR) &gid_ptr, 
					 (SDWORD)sizeof(gid_ptr), &gid_len );

			_ret = SQLExecute(hstmt_c);
			sql_res = SQLFetch( hstmt_c );

			_ret =SQLFreeStmt(hstmt_c, SQL_DROP) ;
		}	

		if (sql_res == SQL_NO_DATA_FOUND ) {
			printf("취소할 그룹에 대한 레코드 없음...\n");
			//getchar();
			LeaveCriticalSection( &cs_db );
			//printf("취소 종료 합니다...\n");
			continue;
		}
	
		// DELETE - message2
		hstmt_c=0;
		//sprintf((char *)select_buf,"delete from goodshot.message2 where gid=%d and meche='9'", gid_ptr);
		sprintf((char *)select_buf,"delete from cancel_gid where gid=%d and meche='99'", gid_ptr);
		printf("[Query] %s\n", select_buf);
		//getchar();
		_ret = SQLAllocStmt( hdbc, &hstmt_c );
		_ret = SQLPrepare(hstmt_c, select_buf, SQL_NTS);
		// SQL 문실행
		sql_res = SQLExecute(hstmt_c);
		SQLFetch( hstmt_c );
		_ret =SQLFreeStmt(hstmt_c, SQL_DROP) ;

		printf("[CANCEL] : cancel_gid 삭제후 !!\n");
		LeaveCriticalSection( &cs_db );


		printf("[CANCEL GID] %d, %d\n", gid_ptr, gid_len);
		//getchar();


		// gid 에 해당 하는 jod 를 취소 하기 : 한번 전송시 마다 동기화 해제 할것 
			
		while ( 1 ) {
		//	getchar();
			jobid_ptr	= NULL;
			seq_ptr		= NULL;
			//ret_ptr	= NULL;

			//meche		= ;

			printf("help_calcel4() Ent[취소(While)]<-\n");
			EnterCriticalSection( &cs_db );
			
			// ------------------- 셀랙트 --------------------------
			// SQL 초기화
			hstmt_c=0;
			sprintf((char *)select_buf,"select gid, jobid, seq, meche  from message_bak2 where gid=%d", gid_ptr);
			printf("SELECT : %s\n", select_buf);
			_ret = SQLAllocStmt( hdbc, &hstmt_c );
			_ret = SQLPrepare(hstmt_c, select_buf, SQL_NTS);
				
			// 변수 바인딩..
			SQLBindCol( hstmt_c,1, SQL_C_LONG, (PTR) &gid_ptr, 
					 (SDWORD)sizeof(jobid_ptr), &gid_len );
			SQLBindCol( hstmt_c,2, SQL_C_LONG, (PTR) &jobid_ptr, 
					 (SDWORD)sizeof(jobid_ptr), &jobid_len );
			SQLBindCol( hstmt_c,3, SQL_C_LONG, (PTR) &seq_ptr, 
					 (SDWORD)sizeof(jobid_ptr), &seq_len );
			SQLBindCol(hstmt_c,4,SQL_C_CHAR,(PTR) meche,
					(SDWORD)sizeof(char)*2, &meche_len);
				
			// SQL 문실행
			_ret = SQLExecute(hstmt_c);
			sql_res = SQLFetch( hstmt_c );
			_ret = SQLFreeStmt(hstmt_c, SQL_DROP) ;


		//  ***  관련 데이타가 없으므로 종료..  *** 
			if (sql_res == SQL_NO_DATA_FOUND || _ret == SQL_NO_DATA_FOUND) {
				printf("해당 데이타가 없어 취소를 종료 합니다.\n");
				LeaveCriticalSection( &cs_db );
				break;
			}

			printf("sql_res %d, _ret %d\n", sql_res, _ret);
			//getchar();

			// ------------- Submit Cancel 
			// ------------- 예약을 취소 합니다.
			printf("[CANCEL 완료-전]!! jobid:  %d -> len: %d\n", jobid_ptr, jobid_cancel);

			jobid_cancel = fn_x_cancel( jobid_ptr, 
										0,
										&ret_cancel
										);

			printf("[CANCEL 완료-후]!! jobid :%d -> len: %d\n", jobid_ptr, jobid_cancel);

		
			// -------------- CANCEL Insert
			// ------------- 취소되 레크드를 cancel_data 에 보관
			hstmt_c=0;
 
			//printf("--- gid %d, seq %d, jobid %d, ret_code %d, meche %s\n", gid_ptr, seq_ptr, jobid_ptr, ret_ptr, meche);
			//sprintf((char *)select_buf,"insert into ezshot.cancel_data(gid,seq,jobid, meche) VALUES(%d, %d, %d, %s)", gid_ptr, seq_ptr, jobid_ptr, meche);
			//sprintf((char *)select_buf,"insert into goodshot.cancel_data(gid,seq,jobid, ret_code, meche) VALUES(%d, %d, %d, %d, %s)", gid_ptr, seq_ptr, jobid_ptr, ret_ptr, meche);
			sprintf((char *)select_buf,"insert into cancel_data(gid,seq,jobid,meche) VALUES(%d, %d, %d, %s)", gid_ptr, seq_ptr, jobid_ptr, meche);
			printf("INSERT : %s\n", select_buf);

			_ret = SQLAllocStmt( hdbc, &hstmt_c );
			_ret = SQLPrepare(hstmt_c, select_buf, SQL_NTS);
			// SQL 문실행 
			sql_res = SQLExecute(hstmt_c);
			//SQLFetch( hstmt );
			_ret =SQLFreeStmt(hstmt_c, SQL_DROP);


			// -------------- CANCEL Delete
			hstmt_c=0;

			//sprintf((char *)select_buf,"delete goodshot.message_bak2 where jobid=%d and gid=%d and seq=%d",jobid_ptr, gid_ptr, seq_ptr);
			sprintf((char *)select_buf,"delete message_bak2 where gid=%d and seq=%d", gid_ptr, seq_ptr);
			//sprintf((char *)select_buf,"delete goodshot.message_bak2 where jobid=%d" ,jobid_ptr);
			printf("DELETE : %s\n", select_buf);
			_ret = SQLAllocStmt( hdbc, &hstmt_c );
			_ret = SQLPrepare(hstmt_c, select_buf, SQL_NTS);
				
			// SQL 문실행
			sql_res = SQLExecute(hstmt_c);
			//SQLFetch( hstmt );
			_ret = SQLFreeStmt(hstmt_c, SQL_DROP) ;
				
			//printf("[CANCEL] - End while() \n");

			LeaveCriticalSection( &cs_db );
		} // while( 해당 하는 jobid 가 없을ㄷ때 까지!! )
	} // while(1)
*/
Sleep(1000);	
	return 1;
}
 
void help_report()
{
	help_basic();
}


char *get_now()
{
	char *ret;
	static char  str[20];

	SYSTEMTIME	systime;

	GetLocalTime( &systime );

	sprintf( str, "%.4d%.2d%.2d%.2d%.2d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute);

	ret = str;

	return ret;
}


char *get_date()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );

	sprintf( str, "%.4d%.2d%.2d%",
		systime.wYear,
		systime.wMonth,
		systime.wDay
		);

	ret = str;

	return ret;
}


char *get_time()
{

	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );

	sprintf( str, "%.2d%.2d%.2d%.2d",
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds
		);

	ret = str;

	return ret;
}


int isNight()
{

	SYSTEMTIME	systime;
	GetLocalTime( &systime );
	if((systime.wHour>=21) || systime.wHour<=7) return 0;
	else return 1;
}
int isNight2(char *now)
{
	if((atoi(now)>=21) || atoi(now)<=07) return 0;
	else return 1;
}

void throw_fucntion()
{

}

void help_uinfo()
{
/*	int			ret;	
	USERINFO	i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}

	printf( " \n\n ");

	i = fn_x_userinfo( &ret );

	printf( " USER INFO FTN [%d], RET [%d] \n", i, ret );

	help_basic();*/
}



void help_reportfile()
{
/*	char	*get_id;
	int		ret, loop;

	if ( bind_flag == 1 )
	{
		printf("벌써 BIND하셨습니다.\n");
		help_basic();
	}

	printf("\n\n");

	printf(" 이용자ID : ");
	gets( USER_NAME );
	printf(" 비밀번호 : ");
	gets( USER_PASSWORD );
	printf(" s : 전송\n" );
	printf(" x : 취소\n");
	printf(" 입력 : ");
	gets( in_str );

	for ( loop = 0 ; loop < 5 ; loop ++ )
	{
	if ( in_str[0] == 's' ) 
	{
		get_id = fn_x_bind(	0, USER_NAME, USER_PASSWORD, "xroshot.com", 
			1, lpfn_submit,
			1, lpfn_report,
			1, lpfn_cancel,
			1, lpfn_status,
			1, lpfn_statusd,
			&ret);

		if ( strncmp( get_id, "-1", 2 ) == 0 )
		{
			printf("Can't connect to server.... exit.\n");
		}

		switch ( ret )
		{
			case 0x00 :
				printf( "바인드 되었습니다. Session ID : %s \n", get_id );
				break;
			case 0x01 : 
				printf( "Result Error.\n" );
				help_basic();
				break;
			case 0x02 :
				printf( "Not a member Error.\n" );
				help_basic();
				break;
			case 0x03 :
				printf( "Length Mismatch Error.\n" );
				help_basic();
				break;
			case 0x04 :
				printf( "Attach Mismatch Error.\n" );
				help_basic();
				break;
			case 0x05 :
				printf( "Queue Full Error.\n" );
				help_basic();
				break;
			case 0x06 :
				printf( "Failure Error.\n" );
				help_basic();
				break;
			case 0x07 :
				printf( "Not Enough Money Error.\n" );
				help_basic();
				break;
			case 0x08 :
				printf( "No Program Error.\n" );
				help_basic();
				break;
			case 0x09 :
				printf( "Holding now.\n" );
				help_basic();
				break;
			case 0x10 :
				printf( "UnKnown Error.\n" );
				help_basic();
				break;
			case 0x11 :
				printf( "Password Mismatch Error.\n" );
				help_basic();
				break;
			case 0x12 :
				printf( "TTS Service Error.\n" );
				help_basic();
				break;
			case 0x13 :
				printf( "TTF Service Error.\n" );
				help_basic();
				break;
			case 0x14 :
				printf( "Already Logged in ...\n" );
				help_basic();
				break;
			case 0x15 :
				printf( "Already Bind Error.\n" );
				help_basic();
				break;
			case 0x16 :
				printf( "Resource Error.\n" );
				help_basic();
				break;
			case 0x17 :
				printf( "Midnight... Prohibited.\n" );
				help_basic();
				break;
			default	:
				printf( "Error occurred...\n" );
				help_basic();
				break;
		}
		bind_flag = 1;
		help_basic();
		Sleep(1000);
	}
	else if ( in_str[0] == 'x' ) help_basic();

	}*/
}


char *get_time2()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );
	char *a="00";
	char *b="00";

	sprintf( str, "%.2d%.2d%s",
		systime.wHour,
		systime.wMinute,
		b
		
		);

	ret = str;

	return ret;
}
/*

char *get_time4()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );
char *a="00";
char *b="00";
	sprintf( str, "%.2d%.2d",
		systime.wHour,
		systime.wMinute
		
		
		);

	ret = str;

	return ret;
}

char *get_timep1()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );
char *b="00";
	sprintf( str, "%.2d%.2d%s",
		systime.wHour,
		systime.wMinute+1,
		b
				
		);

	ret = str;

	return ret;
}
char *get_timep2()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );
char *b="00";
	sprintf( str, "%.2d%.2d%s",
		systime.wHour,
		systime.wMinute+2,
		b

		);

	ret = str;

	return ret;
}
char *get_timep3()
{
	static char  str[20];
	char *ret;
	SYSTEMTIME	systime;

	GetLocalTime( &systime );
char *b="00";
	sprintf( str, "%.2d%.2d%s",
		systime.wHour,
		systime.wMinute+3,
		b
		
		);

	ret = str;

	return ret;
}


*/

// 전송 함수

int	lpfn_submit( int ret_code, char *session_id, int msg_id, int job_id, int fee, int content_size )
{
	/*
	 * 이함수는 콜백 함수로 서버에서 들어 온 jobid 를 받아 큐에 저장한다.
	 * 저장된 jobid 는 스래드에 의해 데이타 베이스에 저장 ( 스래드 동기화 요구 !!! ) 
	*/

	//int count;
	int submit_set[2];
	submit_set[0]=msg_id;
	submit_set[1]=job_id;

	printf("[CALL BACK (submin)] : lpfn_submit job_id[%d] ret_code[%d] <> ", job_id, ret_code);

	// put(...) 함수는 스래드와 동기화를 위한 루틴이 포함되어 있다.
    //count = put(submit_set[0],submit_set[1]);
	put(submit_set[0],submit_set[1]);
	
	//printf("[put()] count : %d \n", count);

    return 1;
}

// 레포트

int	lpfn_report(
char	*session_id,
int		job_id,
int		subjob_id,
char	*dest_addr,
int		ret_code,
char	*start_time,
char	*end_time,
int		duration,
int		pages,
int		fee,
int		reply_dtmf,
char    *replied_file)
{
//	Sleep(50);

	int report_set[6];
	char *tel=dest_addr;
	
	report_set[0]=job_id;
	report_set[1]=ret_code;
	report_set[2]=duration;
	report_set[3]=pages;
	report_set[4]=fee;
	report_set[5]=reply_dtmf;


	totalReportCount++;

	printf("[CALL BACK (repot)] : lpfn_report job_id[%d] ret_code[%d] <>\n ", job_id, ret_code);

	char buff[1000];
	sprintf(buff,"%d\$%s\$%d\$%d\$%d\$%d\$%d",report_set[0],tel,report_set[1],report_set[2],report_set[3],report_set[4],report_set[5]);
	buff[strlen(buff)]=0;
	send(hSocket2,buff,sizeof(buff),0);
/*	printf("[put_2()] count : XX \n");
	printf("[REPORT_CB] : session_id [%s] \n"
			"\t job id [%d:%d] \n"
			"\t dest_addr [%s] \n"
			"\t ret_code  [%d] \n"
			"\t start time | end time [%s|%s] \n"
			"\t duration, pages [%d:%d] \n"
			"\t fee [%d] \n"
			"\t reply_dtmf [%d] \n",
			session_id,
			job_id,
			subjob_id,
			dest_addr,
			ret_code,
			start_time,
			end_time,
			duration,
			pages,
			fee,
			reply_dtmf );
*/
	return 1;
}


int	lpfn_cancel( int ret_code, char *session_id )
{
	printf("* ***** [CANCEL CB] *** <COUNTER : %-10d> ************ * \n", ++CB_COUNTER);
	printf("* [OK!! CANCEL^^// ] ret_code : %d, seession_id %s,  \n", ret_code, session_id);
	printf("* ********************************************************* *\n");

	return 1;
}


// *************** Queue ***********************
// 초기화 
void init_queue(void)
{
	// 원형큐의 초기화 : F 와 R 이 초기 위치인 -1 이며, 아무런 데이타를 
	// 가지지 않은 상태 
	F = -1;
	R = -1;
	F_2 = -1;
	R_2 = -1;
}


// 큐에 자료 입력
int put(int s,int j)
{
	R = (R+1) % MAX_QUE;
	printf("[RUN_CALL_BACK] -> PUT[1]\n");


	if (F == R) 			// 데이타가 큐 에 꽉찬 상태 , 스래드와 동기화 예정
		fprintf(stderr, "put() : Q is Full.. [%d][%d] \n", F, R);

					// Rear 포이터에 테이타 저장
	printf("[Q에 저장]\n");
	Q[R].seq   = s;
	Q[R].jobid = j;
	++k;				// 입력 자료의 갯수

	if (q_flag == 1) {
		// 세마포어(1) unlock
		LeaveCriticalSection( &cs );
		q_flag = 0;
		printf("[Q_1 : 자료가 다시 들어 갑니다.\n");
	}	
	
	// 입력 된 자료의 갯수를 리턴 한다... 
	// 입력 자료의 최대값이 되면 데이타 베이스에 저장.
	return k;
}



// 큐의 자료를 데이타 베이스에 작성 
//int get(void) 
DWORD WINAPI getThread(void *args)
{
	dnode t;		// 임시 데이타 
	int s[2];

	int cnt=10;

	fprintf(stdout, "Thread BLOCK!!\n"); 
	EnterCriticalSection( &cs );	// 최초 대기 상태
	Sleep(100);
	fprintf(stdout, "Thread First - Start!! \n");
		
	while (1) {
		if (F == R) {			// 데이타가 없다 
			q_flag = 1;			// put() 함수에게 lock 상태임을 설정 한다.
			// 세미포어(1) lock
			EnterCriticalSection( &cs );
			//fprintf(stdout, "\nThread Locking....\n");
			// 세마포어(1) lock , 두번 lock 연산을 해서 대기 상태로 된다.
			EnterCriticalSection( &cs );
			Sleep(100);
			continue;
            //fprintf(stderr, "get() : Q is Empty \n");
		} else {
			F = (F + 1) % MAX_QUE;
			t = Q[F];
		}
		

		hstmt1=0;
		printf("t->seq   =%d \n", t.seq);
		printf("t->jobid =%d \n", t.jobid);
		//getchar(); 
		s[0] = t.seq;
		s[1] = t.jobid;

		// 데이타 베이스 질의  
		// 쓰기전 딜레이를 준다.....
		 //Sleep(20);
		//Sleep(200);
		send(hSocket2,"submit",sizeof("submit"),0);
//		Sleep(50);
		printf("[submit CB] 질의 대기....\n");
		EnterCriticalSection( &cs_db );
		printf("[submit CB] 질의 시작....\n");

		_ret=SQLAllocStmt(hdbc,&hstmt1);

		UCHAR submit_bak[200];

		//sprintf((char *)submit_bak,"insert into submit_bak2(seq,jobid) values(%d,%d)",s[0],s[1]);
		sprintf((char *)submit_bak,"update message_bak2 set jobid=%d where seq=%d",s[1],s[0]);
		printf("submit_bak=%s\n",submit_bak);

		SQLPrepare(hstmt1,submit_bak,SQL_NTS); 
		_ret=SQLExecute(hstmt1);
		printf("submit callback query _ret=%d\n",_ret);
		if(_ret==-1) 
		{
			puts("jobid update error!!!!!!!");
			getchar();
		}

/*		
		while ( (_ret=SQLExecute(hstmt1) == -1)) {	// Error  이면 다시 데이타 베이스에 저장.
			fprintf(stderr, "SQL 질의 재실행[1][%d] !!\n", cnt);
			if( (--cnt) < 2 ) break;
			//submit_err(s[0], s[1]);
			Sleep(40);
		}
		cnt=10;
		*/
//printf("[스래드_1] SQLEXE[%d] : ", _ret);
db_error((UCHAR *)"[스래드_1] SQLEXE ", henv, hdbc, hstmt1);
		SQLFreeStmt(hstmt1,SQL_DROP);
		send(hSocket2,"leave",5,0);
		LeaveCriticalSection( &cs_db );
		
	}

	return 1;
}

// report call back Thread
/*
int	put_2(int job_id, char *tel, int ret_code, int talktime, int pages,int fee,int dtmf )
{
	printf("[RUN_CALL_BACK] -> PUT[2]\n");
	R_2 = (R_2+1) % MAX_QUE;

	if (F_2 == R_2)			// 데이타가 큐 에 꽉찬 상태 , 스래드와 동기화 예정
		fprintf(stderr, "put_2() : Q_2 is Full.. [%d][%d] \n", F_2, R_2);


	printf("[Q_2에 저장]\n");
		
	// Rear 포이터에 테이타 저장
	Q_2[ R_2 ].job_id	= job_id;
	Q_2[ R_2 ].ret_code	= ret_code;
	Q_2[ R_2 ].talktime	= talktime;
	Q_2[ R_2 ].pages	= pages;	
	Q_2[ R_2 ].fee		= fee;
	Q_2[ R_2 ].dtmf		= dtmf;
	Q_2[ R_2 ].tel		= tel;

	++k_2;				// 입력 자료의 갯수

	if (q_flag_2 == 1) {
		// 세마포어(1) unlock
		LeaveCriticalSection( &cs_2 );
		Q_2[ R_2 ].job_id	= job_id;
		Q_2[ R_2 ].ret_code	= ret_code;
		Q_2[ R_2 ].talktime	= talktime;
		Q_2[ R_2 ].pages	= pages;	
		Q_2[ R_2 ].fee		= fee;
		Q_2[ R_2 ].dtmf		= dtmf;
		Q_2[ R_2 ].tel		= tel;
		printf("[Q_2 : 자료가 다시 들어 갑니다.\n");
		printf("%d, %d, %d, %d, %d, %d, %s\n",	Q_2[ R_2 ].job_id, Q_2[ R_2 ].ret_code,
				Q_2[ R_2 ].talktime, Q_2[ R_2 ].pages, 	Q_2[ R_2 ].fee, Q_2[ R_2 ].dtmf, 
				Q_2[ R_2 ].tel);
		
		q_flag_2 = 0;
	}
	
	// 입력 된 자료의 갯수를 리턴 한다... 
	// 입력 자료의 최대값이 되면 데이타 베이스에 저장.
	return k_2;
}
*/

/*
DWORD WINAPI getThread_2(void *args)
{
	dnode_2 *t;
	int s[6];
	char *tel;

	SQLRETURN res;  
		
	UCHAR report_bak[600]="";

	int cnt=10;


	//Sleep(50); 
	fprintf(stdout, "Thread BLOCK[2]!!\n");
	EnterCriticalSection( &cs_2 );	// 최초 대기 상태
	Sleep(50);
	fprintf(stdout, "Thread First[2] - Start!! \n");
		
	while (1) {
		if (F_2 == R_2) {			// 데이타가 없다 
			q_flag_2 = 1;			// put() 함수에게 lock 상태임을 설정 한다.
			// 세미포어(1) lock
			EnterCriticalSection( &cs_2 );
		//	fprintf(stdout, "\nnThread[2] Locking.... \n");
			// 세마포어(1) lock , 두번 lock 연산을 해서 대기 상태로 된다.
			EnterCriticalSection( &cs_2 );
			Sleep(100);
			continue;
            //fprintf(stderr, "get() : Q is Empty \n");
		} else {
			F_2 = (F_2 + 1) % MAX_QUE;
			t = &Q_2[F_2];
		}
		

		s[0] = t->job_id;
		s[1] = t->ret_code;
		s[2] = t->talktime;
		s[3] = t->pages;
		s[4] = t->fee;
		s[5] = t->dtmf;
		tel  = t->tel;
	
		// 데이타 베이스 질의  
		// sleep test
		//Sleep(20);
	
		printf("[report CB] 질의 대기중...\n");
		EnterCriticalSection( &cs_db );
		printf("[report CB] 질의 시작\n");

		th_1=0;
		res=SQLAllocStmt(hdbc,&th_1);

		sprintf((char *)report_bak, "update message_bak2 set ret_code=%d,talktime=%d,page=%d,fee=%d where jobid=%d",s[1],s[2],s[3],s[4],s[0]);
		
		res=SQLPrepare(th_1, report_bak, SQL_NTS);
		 
		// SQL 쿼리
		fprintf(stderr, "질의를 합니다.\n");
		//res=SQLExecute(th_1);
		while ( (_ret=SQLExecute(th_1) == -1)) {	// Error  이면 다시 데이타 베이스에 저장.
			fprintf(stderr, "SQL 질의 재실행[2][%d] !!\n", cnt);
			if( (--cnt) < 2 ) break;
		
			//submit_err(s[0], s[1]);
			Sleep(40);
		}
		cnt=10;

		db_error((UCHAR *)"[스래드_2] SQLEXE ", henv, hdbc, hstmt1);

		fprintf(stderr, "[레포트 질의 : 결과]\n");
		fprintf(stderr, "[job_id : %d]  [ret_code : %d] [talktime : %d] [pages : %d] [fee : %d] [dtmf : %d] \n[tel : %s];",
						s[0],s[1],s[2],s[3],s[4],s[5], tel);
		SQLFreeStmt(th_1, SQL_DROP);
	
		LeaveCriticalSection( &cs_db);
		printf("동기화 해제 \n");
	}

	return 1;
}
*/
int isNhic(void)
{
	int ukey=0;
	SDWORD ukey_len=SQL_NTS;
	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR select[55]="SELECT u_key FROM member";
		SQLPrepare(hstmt,select,SQL_NTS);
		SQLBindCol(hstmt,1,SQL_C_LONG,(PTR) ukey,
				  (SDWORD)sizeof(ukey),&ukey_len);
		}
		SQLExecute(hstmt);
		SQLFetch(hstmt);
		if(ukey==1) return 0;
		else		return 1;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

/* 취소할 레코드를 추출 한다.- 사용 하지 않음 */
/*
DWORD WINAPI getCancel(void *)
{
	SQLRETURN res_sql;
	HSTMT		hstmt_gc;
	char *meche="";
	SDWORD meche_len=SQL_NTS;
	SDWORD gid_ptr=0;
	SDWORD gid_len=SQL_NTS;

	EnterCriticalSection( &cs_c );
			
	while(1)
	{
		Sleep(1000);

		EnterCriticalSection( &cs_db );

printf("[CANCEL] SELECT->\n");
					
		meche=(char *)malloc(sizeof(char)*2);
		
		hstmt_gc = 0;
		SQLAllocStmt(hdbc,&hstmt_gc);
		{
			UCHAR select[55]="SELECT gid , meche FROM goodshot.cancel_gid where meche='9'";

			SQLPrepare(hstmt_gc, select, SQL_NTS);

			SQLBindCol(hstmt_gc, 1, SQL_C_LONG,(PTR) &gid_ptr,
					 (SDWORD)sizeof(gid_ptr), &gid_len);
			
			SQLBindCol(hstmt_gc, 2, SQL_C_CHAR,(PTR) meche,
					 (SDWORD)sizeof(meche), &meche_len);
				
			//while((res_sql=SQLExecute(hstmt)) == -1) {  
			//	printf("전송 자료 재질의 ...[%s] \n", select);
			//	Sleep(500);
			//}
			res_sql=SQLExecute(hstmt_gc);
			_ret=SQLFetch(hstmt_gc);

			// 데이타가 없으므로 스리핑....
			//if (_ret == 100) {
			if ( _ret == SQL_NO_DATA_FOUND || _ret == 100 ) {
				SQLFreeStmt(hstmt_gc,SQL_DROP);
				// SQL 동기화 
				LeaveCriticalSection( &cs_db );
				free(meche);
				printf("[CANCEL] SELECT<-\n");
				Sleep(1000);
				//break;
				continue;	
			}

//printf("[CANCEL] Searching... !!\n");
			_ret=SQLFreeStmt(hstmt_gc ,SQL_DROP);	
			LeaveCriticalSection( &cs_db );

			printf("call help_cancel4\n");

			// 스래드에게 작업 지시 
			LeaveCriticalSection( &cs_c ); 

			printf("[취소 대기중...]\n");
			EnterCriticalSection( &cs_c );

			printf("[취소 완  료...]\n");
			EnterCriticalSection( &cs_c );
		
			printf("Call help_cancel LATER !!\n");
			
		}// DB block
	} // while(1)
	return 1;
}
*/