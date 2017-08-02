// console_test2.cpp : Defines the entry point for the console application.
//
   

//동보 함수의 submit함수 인자 확인!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "header.h"
#include "header_s.h"
#include <stdio.h>
#include <string.h>

#include <process.h>		// Thread 관련
#include <windows.h>
#include <stdlib.h>

#if ( ( defined(WIN32) ) || ( defined(_WIN32) ) )
#include <windows.h>
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
#define JOBTYPE_SMS                 0x01        /* 문자 메시지 */
#define JOBTYPE_VOICE               0x02        /* 음성 메시지 */
#define JOBTYPE_FAX                 0x03        /* 팩스 메시지 */
#define JOBTYPE_EMAIL               0x04        /* 메일 메시지 */
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
int (*X_REPORT_CALLBACK_V1)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*) ;
int (*X_REPORT_CALLBACK_V2)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*, int) ;
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

/*
//---------------------
void help_basic();
void help_bind1();
void help_unbind();
void help_submit2();
void help_status3();
void help_cancel4();
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
/********** multistage *************/
//void help_submit2_2_9();
//void help_submit2_2_10();
/********** multistage *************/

/********** 머릿말.맺음말 분리 ************/
//void help_submit2_2_11();
//void help_submit2_2_12();
/********** 머릿말.맺음말 분리 *************/

/********** 업로드 파일 ************/
//void help_submit2_2_13();
//void help_submit2_2_14();
/********** 업로드 파일 ************/

/********** 반복 메시지 테스트  ************/
//void help_submit2_2_15();
/********** 반복 메시지 테스트  ************/

//void help_submit2_2_20();



//void help_submit2_3_1();
//void help_submit2_3_2();

/********** 업로드 파일 ************/
//void help_submit2_3_3();
//void help_submit2_3_4();
/********** 업로드 파일 ************/
/*
void help_submit2_4_1();
void help_submit2_4_2();
void help_submit2_4_3();
void help_submit2_4_4();
void help_submit2_4_5();
void help_submit2_4_6();
void help_submit2_4_7();
void help_submit2_4_8();
*/
/********** 업로드 파일 ************/
//void help_submit2_4_9();
//void help_submit2_4_10();
/********** 업로드 파일 ************/

/*
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
*/
  

//---------------------
void help_basic();
void help_bind1();
void help_unbind();
void help_submit2();
void help_status3();
void help_report();

void help_submit2_1();
void help_submit2_2();
void help_submit2_3();
void help_submit2_4();
void help_submit2_5();
void help_submit2_6();
void help_submit2_7();
void help_submit2_8();

void help_submit2_1_1(int, char *, char *, char *,char *,char *,char *);
void help_submit2_1_2(int, char *, char *, char *,char *,char *,char *);	

void help_submit2_2_1(int , char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_2_2(int , char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_2_3();
void help_submit2_2_4();
void help_submit2_2_5(int, char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_2_6(int, char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_2_7();
void help_submit2_2_8();
/********** multistage *************/
void help_submit2_2_9(int , char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_2_10(int , char *, char *, char *, char *, int, int, char *, char *, char *);
/********** multistage *************/

/********** 머릿말.맺음말 분리 ************/
void help_submit2_2_11(int , char *, char *, char *, char *, char *, int , int , char *, char *, char *,char *, int);
void help_submit2_2_12(int , char *, char *, char *, char *, char *, int , int , char *, char *, char *,char *, int);
/********** 머릿말.맺음말 분리 *************/

/********** 업로드 파일 ************/
void help_submit2_2_13();
void help_submit2_2_14();
/********** 업로드 파일 ************/

/********** 반복 메시지 테스트  ************/
void help_submit2_2_15();
/********** 반복 메시지 테스트  ************/

void help_submit2_3_1(int, char *, char *, char *, char *, char *);
void help_submit2_3_2(int, char *, char *, char *, char *, char *);

/********** 업로드 파일 ************/
void help_submit2_3_3();
void help_submit2_3_4(int, char *, char *, char *, char *, char *);
/********** 업로드 파일 ************/

void help_submit2_4_1(int, char *, char *, char *,char *, char *, int, int, char *,char *,char *);
void help_submit2_4_2(int, char *, char *, char *,char *, char *, int, int, char *,char *,char *);
void help_submit2_4_3();
void help_submit2_4_4();
void help_submit2_4_5(int, char *, char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_4_6(int, char *, char *, char *, char *, char *, int, int, char *, char *, char *);
void help_submit2_4_7();
void help_submit2_4_8();

/********** 업로드 파일 ************/
void help_submit2_4_9();
void help_submit2_4_10();
/********** 업로드 파일 ************/


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


char	in_str[80];
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

char    USERID[100], USERPW[100];
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

char	inputSubs[10000][12];	
int		inputSubsCount = 500;
int		curSubsCount = 0;

char	*get_next();
void	*test_fn();
int		totalReportCount = 0;
int		totalSendCount = 0;

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
	int		(*X_SUBMIT_CALLBACK)(int, char *, int, int, int, int),
	int		report_ack,
	int 	(*X_REPORT_CALLBACK_V1)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*),
	int		cancel_ack,
	int		(*X_CANCEL_CALLBACK)(int, char *),
	int		status_ack,
	int	(*X_STATUS_CALLBACK)(int, char *, int, int),
	int		statusd_ack,
	int (*X_STATUSD_CALLBACK)(int, char *, int, int),
	int		*ret);



int fn_x_bind_v2( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	int		(*X_SUBMIT_CALLBACK)(int, char *, int, int, int, int),
	int		report_ack,
	int (*X_REPORT_CALLBACK_V2)(char *, int, int, char *, int, char *, char*, int, int, int, int, char*, int),
	int		cancel_ack,
	int		(*X_CANCEL_CALLBACK)(int, char *),
	int		status_ack,
	int	(*X_STATUS_CALLBACK)(int, char *, int, int),
	int		statusd_ack,
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




int fn_x_upload(char* file_name, int unique_num, char* upload_file_name) ;

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




int	lpfn_submit( int ret_code, char *session_id, int msg_id, int job_id, int fee, int content_size );
/*{
	printf("[SUBMIT_CB] : ret_code | %d, msg_id | %d, job_id | %d\n", ret_code, msg_id,job_id );
	return 1;
}
*/

int	lpfn_report_v1(
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
char*	replied_file)
{
	totalReportCount ++;

	printf("[REPORT_CB] : total[%d] session_id [%s] \n"
			"\t job id [%d:%d] \n"
			"\t dest_addr [%s] \n"
			"\t ret_code  [%d] \n"
			"\t start time | end time [%s|%s] \n"
			"\t duration, pages [%d:%d] \n"
			"\t fee [%d] \n"
			"\t reply_dtmf [%d] \n"
			"\t replied_file [%s] \n",
			totalReportCount,
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
			reply_dtmf,
			replied_file );

	return 1;
}


int	lpfn_report_v2(
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
char*	replied_file,
int		msgid);
/*{
	totalReportCount ++;

	printf("[REPORT_CB] : total[%d] session_id [%s] \n"
			"\t job id [%d:%d] msgid[%d] \n"
			"\t dest_addr [%s] \n"
			"\t ret_code  [%d] \n"
			"\t start time | end time [%s|%s] \n"
			"\t duration, pages [%d:%d] \n"
			"\t fee [%d] \n"
			"\t reply_dtmf [%d] \n"
			"\t replied_file [%s] \n",
			totalReportCount,
			session_id,
			job_id,
			subjob_id,
			msgid,
			dest_addr,
			ret_code,
			start_time,
			end_time,
			duration,
			pages,
			fee, 
			reply_dtmf,
			replied_file );

	return 1;
}
*/

int	lpfn_cancel( int ret_code, char *session_id )
{
	printf("[CANCEL_CB] : ret_code | %d\n", ret_code  );
	return 1;
}



int	lpfn_status( int ret_code, char *session_id, int msg_id, int job_id )
{
	printf("[STATUS_CB] : ret_code | %d, msg_id | %d, job_id | %d \n", ret_code, msg_id, job_id );
	return 1;
}



int	lpfn_statusd( int ret_code, char *session_id, int msg_id, int job_id );
/*{
	printf("[STATUSD_CB] : ret_code | %d, msg_id | %d, job_id | %d \n", ret_code, msg_id, job_id );
	return 1;
}*/

#define FILE_VOICE_WAV		"d:\\sendwav\\"		// 음성 파일 경로
#define FILE_FAX     		"d:\\sendfax\\"		// 팩스 파일 경로
#define FILE_LIST           "d:\\sendlist\\"    // 수신자 리스트 파일 경로

// 쿼리문 버퍼 사이즈
#define BUFF_SIZE	128
#define INSERT_SIZE	2048
#define DELETE_SIZE	128
#define UPDATE_SIZE 128
#define UPDATE2_SIZE 256


char *get_date();
char *get_time();
char *get_time2();
char *get_time4();
void exception();
void status(int);
void cancel(int);
char *file_processing(int,char *,char *);
char *file_processing2(int,char *,char *);

/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;
void SQL_EXCEPTION( int _ret );
int db_error(UCHAR *msg, SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);

int job_id;

/* *** 스래드 함수 *** */
HANDLE	hThread, hThread_2, hThread_c, hThread_bu;
DWORD	hID, hID_2, hID_c, hID_bu;
CRITICAL_SECTION  cs, cs_2, cs_bu;							// 스래드 동기화 변수
CRITICAL_SECTION  cs_db, cs_c;

//DWORD WINAPI getThread(void *);
DWORD WINAPI getThread_2(void *);

DWORD WINAPI help_cancel4(void *);		// 취소 기능
DWORD WINAPI build(void *);


/* *** 큐 정의 *** */
void init_queue(void);
//int put(int,int,int);
int	put_2(int,char *,int,int,int,int,int,char *,char *,char *,int);

//int get();

//#define MAX_QUE 80000
#define		MAX_QUE 163840		// 한국통신 서버의 큐-버퍼는 8 만개이다.
/*
struct _dnode
{
	int seq;
	int jobid;
	int ret_code;
//	struct _dnode *prev;
//	struct _dnode *next;
};
typedef struct _dnode dnode;
*/
struct _dnode_2
{
	// 두번째 큐 자료형
	int job_id;
	int msgid;
	char tel[15]; 
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

//dnode	Q[MAX_QUE];
//dnode_2	Q_2[MAX_QUE];
dnode_2 *head, *tail;
	
//int F, R;				// Front, Rear : 두개의 큐 사용 
int F_2, R_2;
//int q_flag = 1;			// 세마포어가 실행시 블록 상태로 시작
int q_flag_2 = 1;
int k, k_2;				// Q counter;
unsigned long CB_COUNTER = 0; // Cancel CB counter


//MSG field variable
int   gid=0;
SDWORD gid_len=SQL_NTS;

char message1[82]="";
SDWORD message1_len=SQL_NTS;
char message2[1002]="";
SDWORD message2_len=SQL_NTS;
char message3[82]="";
SDWORD message3_len=SQL_NTS;
char msg_sdate[15]="";
SDWORD msg_sdate_len=SQL_NTS;
char msg_edate[15]="";
SDWORD msg_edate_len=SQL_NTS;
char attach_file[30]="";
SDWORD attach_file_len=SQL_NTS;
int	  reply_type=0;
SDWORD reply_type_len=SQL_NTS;
int   reply_count=0;
SDWORD reply_count_len=SQL_NTS;
char cs_number[12]="";
SDWORD cs_number_len=SQL_NTS;
int   msg_type=0;
SDWORD msg_type_len=SQL_NTS;
char subject[50]="";
SDWORD subject_len=SQL_NTS;
char reply_tel[12]="";
SDWORD reply_tel_len=SQL_NTS;
int job_status;
SDWORD job_status_len=SQL_NTS;
int ment_play=0;
SDWORD ment_play_len=SQL_NTS;
int temp=0;


int qCount;

#define PRINT	1
#define TEST	0
#define EMUL	1
#define SUBMIT4

struct _table{
	char msg[20];
	char msg_result[20];
	char db_name[20];
	char db_id[20];
	char db_passwd[20];
	char xroshot_id[20];
	char xroshot_passwd[20];
};

struct _table table;

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

int main()
{
	int		retnum;
	int		ret;
	
	fn_x_set_conf_file("D:\\conf\\ver2\\xroshot.conf") ;
	// 바인드 는 Call back 함수 등록 한다. 
	
	retnum = fn_x_bind_v2(	0,"sohi9","09421", "xroshot.com", 
				1, lpfn_submit,
				1, lpfn_report_v2,
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
	bind_flag = 1;
	// 큐 초기화 
	init_queue();
	
	// 초기화 - MS SQL
	henv = hdbc = hstmt = 0;  

	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
//	_ret=SQLConnect(hdbc,(UCHAR *)table.db_name,SQL_NTS,(UCHAR *)table.db_id,SQL_NTS,(UCHAR *)table.db_passwd,SQL_NTS);
	_ret=SQLConnect(hdbc,(UCHAR *)"KARMA",SQL_NTS,(UCHAR *)"karma",SQL_NTS,(UCHAR *)"karmahaja8282",SQL_NTS);

	// SQL Connect Exception's
	SQL_EXCEPTION( _ret );

//help_submit2_1_2(gid,message1,msg_sdate,msg_edate,attach_file,subject,reply_tel);
	// 스래드 동기화 선언 : cs 변수는 동기화 ID 이다.
//	InitializeCriticalSection( &cs   );
	InitializeCriticalSection( &cs_2 );
	InitializeCriticalSection( &cs_db);
//	InitializeCriticalSection( &cs_bu);
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

//	EnterCriticalSection( &cs_bu );
//	hThread_db= (HANDLE)CreateThread(NULL, 0, build, (void *)NULL, 0, (unsigned long *)&hID_bu);
//	Sleep(100);
//	EnterCriticalSection( &cs );	// 최초 대기 상태
	EnterCriticalSection( &cs_2 );
//	EnterCriticalSection( &cs_c );


	
//	hThread	  = (HANDLE)CreateThread(NULL, 0, getThread,    (void *)NULL, 0, (unsigned long *)&hID  );
	hThread_2 = (HANDLE)CreateThread(NULL, 0, getThread_2,  (void *)NULL, 0, (unsigned long *)&hID_2);
//	hThread_gc = (HANDLE)CreateThread(NULL, 0, getCancel, (void *)NULL, 0, (unsigned long *)&hID_gc);
//	hThread_c = (HANDLE)CreateThread(NULL, 0, help_cancel4, (void *)NULL, 0, (unsigned long *)&hID_c);   //cancel thread
#ifdef TEST 1
	DeleteCriticalSection( &cs_bu );	
#endif
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

	return 0;
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








void help_basic()
{

	SQLRETURN res_sql;
	
	while(1)
	{
	//printf("help_basic() Ent[help_basic]->\n");		// SQL 동기화
	EnterCriticalSection( &cs_db );
	
	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
		{
		UCHAR select[650]="SELECT GID,MESSAGE1,MESSAGE2,MSG_SDATE,MSG_EDATE,ATTACH_FILE,REPLY_TYPE,REPLY_COUNT,CS_NUMBER,MSG_TYPE,SUBJECT,REPLY_TEL,JOB_STATUS,MESSAGE3,MENT_PLAY FROM VIEW_MSG WHERE JOB_STATUS=11 or JOB_STATUS=21 or JOB_STATUS=22 or JOB_STATUS=23 or JOB_STATUS=31 ORDER BY MSG_SDATE";
		SQLPrepare(hstmt,select,SQL_NTS);
		SQLBindCol(hstmt,1,SQL_C_LONG,(PTR) &gid,(SDWORD)sizeof(gid), &gid_len);
		SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) message1,(SDWORD)sizeof(message1),&message1_len);
		SQLBindCol(hstmt,3,SQL_C_CHAR,(PTR) message2,(SDWORD)sizeof(message2),&message2_len);
		SQLBindCol(hstmt,4,SQL_C_CHAR,(PTR) msg_sdate,(SDWORD)sizeof(msg_sdate),&msg_sdate_len);
		SQLBindCol(hstmt,5,SQL_C_CHAR,(PTR) msg_edate,(SDWORD)sizeof(msg_edate),&msg_edate_len);
		SQLBindCol(hstmt,6,SQL_C_CHAR,(PTR) attach_file,(SDWORD)sizeof(attach_file),&attach_file_len);
		SQLBindCol(hstmt,7,SQL_C_LONG,(PTR) &reply_type,(SDWORD)sizeof(reply_type), &reply_type_len);
		SQLBindCol(hstmt,8,SQL_C_LONG,(PTR) &reply_count,(SDWORD)sizeof(reply_count), &reply_count_len);
		SQLBindCol(hstmt,9,SQL_C_CHAR,(PTR) cs_number,(SDWORD)sizeof(cs_number),&cs_number_len);
		SQLBindCol(hstmt,10,SQL_C_LONG,(PTR) &msg_type,(SDWORD)sizeof(msg_type), &msg_type_len);
		SQLBindCol(hstmt,11,SQL_C_CHAR,(PTR) subject,(SDWORD)sizeof(subject),&subject_len);
		SQLBindCol(hstmt,12,SQL_C_CHAR,(PTR) reply_tel,(SDWORD)sizeof(reply_tel),&reply_tel_len);
		SQLBindCol(hstmt,13,SQL_C_LONG,(PTR) &job_status,(SDWORD)sizeof(job_status), &job_status_len);	
		SQLBindCol(hstmt,14,SQL_C_CHAR,(PTR) message3,(SDWORD)sizeof(message3),&message3_len);
		SQLBindCol(hstmt,15,SQL_C_LONG,(PTR) &ment_play,(SDWORD)sizeof(ment_play), &ment_play_len);	
		res_sql=SQLExecute(hstmt);
		_ret=SQLFetch(hstmt);
	
	/*	while((res_sql=SQLExecute(hstmt)) == -1) {  
			printf("전송 자료 재질의 ...[%s] \n", select);
			Sleep(500);
		}*/
	
		if (_ret == 100){
			SQLFreeStmt(hstmt,SQL_DROP);
			Sleep(20);
		//	printf("help_basic() Lev[help_basic]<-\n");	// 수정 금지 !!!
			LeaveCriticalSection( &cs_db );
			continue;	
		}
		if (_ret == 0 || _ret== 1 ){
			if(job_status==31)
			{
				puts("call status()\n");
				_ret=SQLFreeStmt(hstmt,SQL_DROP);
				status(gid);
				continue;
			}else{
		
				switch(msg_type)
				{
				//문자 동보
				case 101 :
					printf("call help_submit2_1_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_1_1(gid,message1,msg_sdate,msg_edate,attach_file,subject,reply_tel);
				//문자 파일 
				case 102 :
					printf("call help_submit2_1_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_1_2(gid,message1,msg_sdate,msg_edate,attach_file,subject,reply_tel);
					break;
				//음성 파일 첨부 동보
				case 201 :
					printf("call help_submit2_2_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_1(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//음성 파일 첨부 파일
				case 202 :
					printf("call help_submit2_2_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_2(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//음성 TTS 변환 동보
				case 205 :
					printf("call help_submit2_2_5\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_5(gid,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//음성 TTS 변환 파일 
				case 206 :
					printf("call help_submit2_2_6\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_6(gid,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//다단계 설문조사 동보
				case 209 :
					printf("call help_submit2_2_9\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_9(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//다단계 설문조사 파일
				case 210 :
					printf("call help_submit2_2_10\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_10(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//머릿말 맺음말 분리 동보
				case 211 :
					printf("call help_submit2_2_11\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_11(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel,message3,ment_play);
					break;
				//머릿말 맺음말 분리 동보
				case 212 :
					printf("call help_submit2_2_12\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_12(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel,message3,ment_play);
					break;

				//팩스 동보
				case 301 :
					printf("call help_submit2_3_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_1(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;
				//팩스 파일
				case 302 :
					printf("call help_submit2_3_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_2(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;

				//팩스 즉시(upload) ---> 사용안함
				case 304 :
					printf("call help_submit2_3_4\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_4(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;

				//문자 플러스 파일첨부 동보
				case 401 :
					printf("call help_submit2_4_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_1(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//문자 플러스 파일첨부 파일
				case 402 :
					printf("call help_submit2_4_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_2(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//문자 플러스 동보
				case 405 :
					printf("call help_submit2_4_6\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_6(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				//문자 플러스 파일
				case 406 :
					printf("call help_submit2_4_6\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_6(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//문자 예약				
				case 501 :
					printf("call help_submit2_1_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_1_1(gid,message1,msg_sdate,msg_edate,attach_file,subject,reply_tel);
					break;
				case 502 :
					printf("call help_submit2_1_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_1_2(gid,message1,msg_sdate,msg_edate,attach_file,subject,reply_tel);
					break;

				//음성 파일 첨부 예약
				case 601 :
					printf("call help_submit2_2_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_2(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				case 602 :
					printf("call help_submit2_2_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_2(gid,message1,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//음성 TTS변환 예약
				case 606 :
					printf("call help_submit2_2_6\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_2_6(gid,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//팩스 예약
				case 701 :
					printf("call help_submit2_3_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_1(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;
				case 702 :
					printf("call help_submit2_3_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_2(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;

				//팩스 예약(upload) ---> 사용안함
				case 704 :
					printf("call help_submit2_3_4\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_3_4(gid,message1,msg_sdate,msg_edate,attach_file,subject);
					break;

				//문자 플러스 파일첨부 예약
				case 801 :
					printf("call help_submit2_4_1\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_1(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				case 802 :
					printf("call help_submit2_4_2\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_2(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//문자 플러스 예약
				case 805 :
					printf("call help_submit2_4_5\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_5(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;
				case 806 :
					printf("call help_submit2_4_6\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					help_submit2_4_6(gid,message1,message2,msg_sdate,msg_edate,attach_file,reply_type,reply_count,cs_number,subject,reply_tel);
					break;

				//예약 취소 
				case 99	:
					printf("call cancel()\n");
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					cancel(gid);
					break;

				default :
					//	printf("Data ERR !!! \n\n");
					//	getchar();
					_ret=SQLFreeStmt(hstmt,SQL_DROP);
					break;
				} // switch
			}//else
		} // if()

	}// DB block
//		printf("help_basic() Lev[while]<-\n"); // 수정 금지  
//		LeaveCriticalSection( &cs_db );
	} 


}



void help_bind1()
{
	int		retnum;
	int		ret, loop;
	int		bind_vsn;
	char	temp[10] ;

	if ( bind_flag == 1 )
	{
		printf("벌써 BIND하셨습니다.\n");
		help_basic();
	}

	printf("\n\n");

	printf(" 이용자ID : ");
	gets( USERID );
	printf(" 비밀번호 : ");
	gets( USERPW );
	printf(" s : 전송\n" );
	printf(" x : 취소\n");
	printf(" 입력 : ");
	gets( in_str );

SELECT_BIND_VSN:
	printf("\n\n");
	printf("BIND 버젼 (1 or 2) : ");
	gets( temp );
	bind_vsn = atoi( temp );
	if(bind_vsn != 1 && bind_vsn != 2)
	{
		printf("BIND VERSION은 1 또는 2중에 선택 가능합니다.");
		goto SELECT_BIND_VSN ;
	}

	for ( loop = 0 ; loop < 5 ; loop ++ )
	{
	if ( in_str[0] == 's' ) 
	{
		fn_x_set_conf_file("D:\\conf\\xroshot.conf") ;

		if(bind_vsn == 1)
		{
			retnum = fn_x_bind(	0, USERID, USERPW, "xroshot.com", 
				1, lpfn_submit,
				1, lpfn_report_v1,
				1, lpfn_cancel,
				1, lpfn_status,
				1, lpfn_statusd,
				&ret);
		}
		else if(bind_vsn == 2)
		{
			retnum = fn_x_bind_v2(	0, USERID, USERPW, "xroshot.com", 
				1, lpfn_submit,
				1, lpfn_report_v2,
				1, lpfn_cancel,
				1, lpfn_status,
				1, lpfn_statusd,
				&ret);
		}

		if ( retnum != X_RET_FUNC_SUCCESS )
		{
			printf("Can't connect to server.... exit.\n");
		}
		switch ( ret )
		{
			case 0x00 :
				printf( "바인드 되었습니다. \n" );
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
#if ( defined(WIN32) || defined(_WIN32) )
	Sleep(1000);
#else
	sleep(1);
#endif

	}
	else if ( in_str[0] == 'x' ) help_basic();
	else help_bind1();
	}
}


void help_unbind()
{

		printf("UNBIND------------------ \n");

		fn_x_unbind( USERID, USERPW );
		printf("UNBIND되었습니다. \n");
		bind_flag = 0;


}

/*
void help_submit2()
{
	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}
	printf(" \n\n");
	printf(" 2-1. 문자메시지 즉시전송  \n");
	printf(" 2-2. 음성메시지 즉시전송  \n");
	printf(" 2-3. 팩스메시지 즉시전송  \n");
	printf(" 2-4. 문자플러스 즉시전송  \n");
	printf(" 2-5. 문자메시지 예약전송  \n");
	printf(" 2-6. 음성메시지 예약전송  \n");
	printf(" 2-7. 팩스메시지 예약전송  \n");
	printf(" 2-8. 문자플러스 예약전송  \n");
	printf(" x. 이전화면  \n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-1", 3 ) == 0 ) help_submit2_1();
	else if ( strncmp( in_str, "2-2", 3 ) == 0 ) help_submit2_2();
	else if ( strncmp( in_str, "2-3", 3 ) == 0 ) help_submit2_3();
	else if ( strncmp( in_str, "2-4", 3 ) == 0 ) help_submit2_4();
	else if ( strncmp( in_str, "2-5", 3 ) == 0 ) help_submit2_5();
	else if ( strncmp( in_str, "2-6", 3 ) == 0 ) help_submit2_6();
	else if ( strncmp( in_str, "2-7", 3 ) == 0 ) help_submit2_7();
	else if ( strncmp( in_str, "2-8", 3 ) == 0 ) help_submit2_8();
	else if ( in_str[0] == 'x' ) help_basic();
	else help_submit2();
}
*/

/*
void 
help_submit2_1()
{
	printf("\n\n");
	printf(" 2-1-1. 문자메시지 (동보 가능)\n");
	printf(" 2-1-2. 문자메시지 (파일)전송  \n");
	printf(" x. 이전화면  \n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-1-1", 5 ) == 0 ) help_submit2_1_1();
	else if ( strncmp( in_str, "2-1-2", 5 ) == 0 ) help_submit2_1_2();
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_1();
	
}
*/


void help_submit2_1_1(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, char *subject, char *reply_tel)
{
FILE *list;
int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

//데이터의 공백제거 
for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}
	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(reply_tel," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.
#if defined(SUBMIT4)
		fn_x_submit_4 (
			gid,
			JOBTYPE_SMS,
			SMS_TEMP_TEMP,
			subject,
			0,
			msg_sdate,
			msg_edate,
			reply_tel,
			"http^//www.xroshot.com",
			DEST_TEMP_TYPE,
			0,
			"",
			j,
			send_list,
			message1,
			"",
			0,
			"",
			"",//서버에 존재하는 파일이름
			0,	
			0,
			0,   /* Expire Time */
			"", /* Return email address */
			0,   /*  0 : No ment, 1 : Ment */
			0,  /*  0 : default ( Select voice to hear ) */
			0,
			"",//cs_number
			"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
			"",		/* 맺음말 메시지, 파일만 사용 가능 */
			3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
			"251186",		/* 사원 번호 */
			&ret);
#else

		ret_flag=fn_x_submit( gid,
			JOBTYPE_SMS,
			SMS_TEMP_TEMP,
			subject,
			0,
			msg_sdate,
			msg_edate,
			reply_tel,
			"http^//www.xroshot.com",
			DEST_TEMP_TYPE,
			0,
			"",
			j,
			send_list,
			message1,
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
#endif
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
  		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
		printf("문자 메시지 동보 : [%s] 전송완료\n",subject);
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	}
	LeaveCriticalSection(&cs_db);
	return ;
}

/*
void 
help_submit2_1_2()
{
	char	msg_num[20], title[20], repnum[20], msg[100], filename[10000];
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

	ret_flag=fn_x_submit(	msgnum, 
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
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_1();
}
*/
//문자 즉시
void help_submit2_1_2(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, char *subject, char *reply_tel)
{
	UCHAR update_data[DELETE_SIZE]="";
	int i;
	char *send="";
	char send_list[30];
	send=file_processing(gid,attach_file,subject);
	strcpy(send_list,send);

//데이터의 공백제거 
for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
strtok(send_list," ");

#if defined(SUBMIT4)

fn_x_submit_4 (
		gid,
		JOBTYPE_SMS, 
		SMS_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"http^//www.xroshot.com", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		message1, 
		"",
		0,
		"",
		"",//서버에 존재하는 파일이름
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
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else

	ret_flag=fn_x_submit(	
		gid, 
		JOBTYPE_SMS, 
		SMS_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"http^//www.xroshot.com", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		message1, 
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
#endif	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
  	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("문자 메시지 :[%s] 전송완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );

  return;
}

/*
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
	printf(" 2-2-9. 음성메시지 다단계파일(mstg) (동보 가능) \n");
	printf(" 2-2-10. 음성메시지 다단계파일(mstg) (파일 전송) \n");
	printf(" 2-2-11. 음성메시지 머릿말분리 (동보 가능) \n");
	printf(" 2-2-12. 음성메시지 머릿말분리 (파일 전송) \n");
	printf(" 2-2-13. 음성메시지 업로드한 파일 (동보 가능) \n");
	printf(" 2-2-14. 음성메시지 업로드한 파일 (파일 전송) \n");
	printf(" 2-2-15. 음성메시지 반복 테스트 (동보가능) \n");
	


	printf(" x. 이전화면  \n");
	gets( in_str );
	if ( strncmp( in_str, "2-2-10", 6 ) == 0 ) help_submit2_2_10();
	else if ( strncmp( in_str, "2-2-11", 6 ) == 0 ) help_submit2_2_11();
	else if ( strncmp( in_str, "2-2-12", 6 ) == 0 ) help_submit2_2_12();
	else if ( strncmp( in_str, "2-2-13", 6 ) == 0 ) help_submit2_2_13();
	else if ( strncmp( in_str, "2-2-14", 6 ) == 0 ) help_submit2_2_14();
	else if ( strncmp( in_str, "2-2-15", 6 ) == 0 ) help_submit2_2_15();
	else if ( strncmp( in_str, "2-2-20", 6 ) == 0 ) help_submit2_2_20();
	else if ( strncmp( in_str, "2-2-1", 5 ) == 0 ) help_submit2_2_1();
	else if ( strncmp( in_str, "2-2-2", 5 ) == 0 ) help_submit2_2_2();
	else if ( strncmp( in_str, "2-2-3", 5 ) == 0 ) help_submit2_2_3();
	else if ( strncmp( in_str, "2-2-4", 5 ) == 0 ) help_submit2_2_4();
	else if ( strncmp( in_str, "2-2-5", 5 ) == 0 ) help_submit2_2_5();
	else if ( strncmp( in_str, "2-2-6", 5 ) == 0 ) help_submit2_2_6();
	else if ( strncmp( in_str, "2-2-7", 5 ) == 0 ) help_submit2_2_7();
	else if ( strncmp( in_str, "2-2-8", 5 ) == 0 ) help_submit2_2_8();
	else if ( strncmp( in_str, "2-2-9", 5 ) == 0 ) help_submit2_2_9();
	
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_2();
	
}
*/

/*
void 
help_submit2_2_1()
{
	char	msg_num[20], title[20], repnum[20], pLoop[10],
			file[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, loop, i = 0;

	int		call_dtmf = 0;
	char	call_num[20] = "";


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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );
	

	qnum = atoi( in_str );
	printf(" 첨부파일 : ");
	gets( file );
	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	printf("\n\n");
	printf("반복 횟수 : ");
	gets( pLoop );
	loop = atoi( pLoop );


	for(i=0; i<loop; i++)
	{
		tmp = strtok( list1, "|" );
		while ( tmp != NULL )
		{
			i ++;
			tmp = strtok( NULL, "|" );
		}
		
		printf(" file: %s", file);
		
		//	DebugLog((DEB_TRACE, "[SUBMIT_FUNCTION] called by B2B Client !!!!!!!!!!!!! \n")) ;
		ret_flag = fn_x_submit_2( msgnum, 
			JOBTYPE_VOICE, 
			VOICE_FILE_TEMP, 
			title, 
			0, 
			get_now(),
			get_now(),
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
			call_dtmf,
			call_num,
			&ret
			);
		printf(" file: %s", file);
		msgnum++ ;
		
		if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
		else printf( " 성공적으로 수행되었습니다. \n");
	}

	help_submit2_2();
}
*/
//음성 첨부 동보 
void help_submit2_2_1(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{
	
FILE *list;
//int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

char wave_file[30]="";
sprintf(wave_file,"d:\\sendfile\\%s",message1);

//데이터의 공백제거 
strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
strtok(cs_number," ");
strtok(wave_file," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) fprintf(stderr,"file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.
#if defined(SUBMIT4)
	fn_x_submit_4 (
			gid,
			JOBTYPE_VOICE, 
			VOICE_FILE_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			reply_tel,  
			"", 
			DEST_TEMP_TYPE, 
			j,
			send_list, 
			0, 
			"", 
			"", 
			"", 
			0, 
			wave_file,
			"",
			reply_type,
			reply_count,
			0,   /* Expire Time */
			"", /* Return email address */
			0,   /*  0 : No ment, 1 : Ment */
			0,  /*  0 : default ( Select voice to hear ) */
			0,
			cs_number,
			"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
			"",		/* 맺음말 메시지, 파일만 사용 가능 */
			3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
			"251186",		/* 사원 번호 */
			&ret);
#else

		ret_flag = fn_x_submit_2( gid, 
			JOBTYPE_VOICE, 
			VOICE_FILE_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			reply_tel,  
			"", 
			DEST_TEMP_TYPE, 
			j,
			send_list, 
			0, 
			"", 
			"", 
			"", 
			0, 
			wave_file,
			"",
			reply_type,
			reply_count, 
			0,
			"",
			0,
			0,
			0,
			cs_number,
			&ret
		);
#endif	
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
  	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("음성 첨부 메시지 : [%s] 전송완료\n",subject);
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	}
	LeaveCriticalSection( &cs_db );
	return;
	
}



//음성 첨부 
void help_submit2_2_2(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{

	UCHAR update_data[DELETE_SIZE]="";
	char *send="";
	char send_list[30];
	send=file_processing(gid,attach_file,subject);
	strcpy(send_list,send);
	char wave_file[30]="";
	sprintf(wave_file,"d:\\sendfile\\%s",message1);

//데이터의 공백제거 

strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
strtok(cs_number," ");
strtok(wave_file," ");

#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_VOICE, 
		VOICE_FILE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		"", 
		"", 
		0, 
		wave_file,
		"",
		reply_type,
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( gid, 
		JOBTYPE_VOICE, 
		VOICE_FILE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		"", 
		"", 
		0, 
		wave_file,
		"",
		reply_type,
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);

#endif
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
  
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("음성 첨부 메시지 : [%s] 전송완료\n",subject);
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );
  return;
}

/*

void 
help_submit2_2_2()
{
	char	msg_num[20], title[20], repnum[20], 
			infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	int	call_dtmf = 0;
	char	call_num[20] = "";


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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_2();
}



void 
help_submit2_2_3()
{
	char	msg_num[20], title[20], repnum[20], 
			file[100], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag  );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_2();
}



void 
help_submit2_2_4()
{
	char	msg_num[20], title[20], repnum[20], 
			infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	ret_flag=fn_x_submit_2(	msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_2();
}
*/
/*
void 
help_submit2_2_5(int gid,char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{

FILE *list;
int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
char *send_list_temp;
char list_buff[50];

//데이터의 공백제거 
for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}
	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(reply_tel," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	list=fopen(send_file,"w");

	while(1)
	{
	
		for(j=0;j=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
			send_list[strlen(send_list)-1]=0; //맨 마지막에 |를 없앤다.
		}

	ret_flag=fn_x_submit_4(	
	gid, 
	JOBTYPE_VOICE, 
	VOICE_TEMP_TEMP, 
	subject, 
	0, 
	msg_sdate,
	msg_edate,
	reply_tel,  
	"", 
	DEST_TEMP_TYPE,		// customer type
	0,					// file count
	"",					// file name
	j,					// addr count 
	send_list,
	"", 				// sms
	message2, 				// tts
	0, 					// content file count
	"",
	"",
	reply_type, 
	reply_count, 
	0,
	"",
	0, //ment_play
	0,
	call_dtmf,
	call_num,
	"",
	"",
	1,
	"12345",
	&ret
	);

	printf( " TTS : %s \n", msg);

	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}

*/
/*
void 
help_submit2_2_6()
{
	char	msg_num[20], title[20], repnum[20], 
			msg[10000], ufile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자화일: ");
	gets( ufile );


	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}
*/

void help_submit2_2_5(int gid,char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{

FILE *list;
int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

//데이터의 공백제거 
for (i=1002; i >= 0; --i) {
		if (message2[i] != ' ') {
			message2[i+1] = '\0';
			break;
		}
	}
strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	list=fopen(send_file,"w");

	while(1)
	{
	
		for(j=0;j=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
			send_list[strlen(send_list)-1]=0; //맨 마지막에 |를 없앤다.

	ret_flag=fn_x_submit_4(	
		gid, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
		subject, 
		0,  
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// file count
		"",					// file name
		j,					// addr count 
		send_list,
		"", 				// sms
		message2, 				// tts
		0, 					// content file count
		"",
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0, //ment_play
		0,
		0,
		cs_number,
		"",
		"",
		1,
		"251186",
		&ret
		);
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
		printf("\t음성 메시지 : [%s] 전송완료\n",subject);
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	}//end while
	LeaveCriticalSection(&cs_db);
	return;

}


//음성 즉시 
void help_submit2_2_6(int gid,char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{

UCHAR update_data[DELETE_SIZE]="";
int i;
char *send="";
char send_list[30];

send=file_processing(gid,attach_file,subject);
//EnterCriticalSection(&cs_db);
strcpy(send_list,send);


for (i=1002; i >= 0; --i) {
		if (message2[i] != ' ') {
			message2[i+1] = '\0';
			break;
		}
	}
strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");

#if defined(SUBMIT4)
puts("ver.4");
fn_x_submit_4 (
	gid,
JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,//get_now()
		msg_edate,//get_now()
		reply_tel,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		send_list,			// file name
		0,					// addr count 
		"",      //tel_db_ptr
		"", 				// sms
		message2, 			// tts
		0, 					// content file count
		"",
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	job_id=fn_x_submit_2(
		gid, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,//get_now()
		msg_edate,//get_now()
		reply_tel,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// file count
		send_list,			// file name
		0,					// addr count 
		"",      //tel_db_ptr
		"", 				// sms
		message2, 			// tts
		0, 					// content file count
		"",
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);
#endif


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
  	
//free(send_list);


	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("\t음성 메시지 : [%s] 전송완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

LeaveCriticalSection( &cs_db );

  
  return;
}

/*
void 
help_submit2_2_7()
{
	char	msg_num[20], title[20], repnum[20], 
			list[10000], list1[10000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf= 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}


void 
help_submit2_2_8()
{
	char	msg_num[20], title[20], repnum[20], 
			ufile[100],  infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}


////////////////////////////
void 
help_submit2_2_9()
{
	char	msg_num[20], title[20], repnum[20], 
			list[10000], list1[10000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf= 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_TEMP, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}
*/

void 
help_submit2_2_9(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{
	FILE *list;
//int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

char wave_file[30]="";
sprintf(wave_file,"d:\\sendfile\\%s",message1);

//데이터의 공백제거 
strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
strtok(cs_number," ");
strtok(wave_file," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.
#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",				// file name
		j,					// addr count 
		send_list,
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		wave_file,
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( gid, 
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",				// file name
		j,					// addr count 
		send_list,
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		wave_file,
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);
#endif	
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
		puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
		printf("\t다단계 설문 조사 메시지 : [%s] 전송완료\n",subject);
		puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	}
	LeaveCriticalSection( &cs_db );
	return;
}



/*
void 
help_submit2_2_10()
{
	char	msg_num[20], title[20], repnum[20], 
			ufile[100],  infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

	printf("\n\n 2_2_10 !!!!! ");
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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_FILE, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}
*/


//다단계 설문조사 
void help_submit2_2_10(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{
	
	UCHAR update_data[DELETE_SIZE]="";
	char *send="";
	char send_list[30];
	send=file_processing(gid,attach_file,subject);
	strcpy(send_list,send);
	char wave_file[30]="";
	sprintf(wave_file,"d:\\sendfile\\%s",message1);

//데이터의 공백제거 

strtok(send_list," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
strtok(reply_tel," ");
strtok(cs_number," ");
strtok(wave_file," ");

#if defined(SUBMIT4)
	fn_x_submit_4 (
		gid,
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		send_list,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		wave_file,
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( 
		gid, 
		JOBTYPE_VOICE, 
		VOICE_MULTISTAGE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		send_list,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		wave_file,
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);
#endif	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
  
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("\t다단계 설문 조사 메시지 : [%s] 전송완료\n",subject);
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );
  return;
}


////////////////////////////
/*
void 
help_submit2_2_11()
{
	char	msg_num[20], title[20], repnum[20], temp[10], 
			header_msg[70], tail_msg[64], msg[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0, re_listen_count, ment_play ;
	

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf("  멘트타입 : " );
	gets( temp );
	ment_play = atoi(temp);

	printf("  재청취 횟수 : " );
	gets( temp );
	re_listen_count = atoi(temp);

	printf(" 머릿말 : ");
	gets( header_msg );

	printf(" TTS메시지  : ");
	gets( msg );

	printf(" 맺음말 : ") ;
	gets( tail_msg );

	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_3(	msgnum, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
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
		list,
		"", 				// sms
		msg, 				// tts
		0, 					// content file count
		"",
		"",
		reptype, 
		qnum, 
		0,
		"",
		ment_play,
		0,
		call_dtmf,
		call_num,
		header_msg,
		tail_msg,
		re_listen_count,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}



void 
help_submit2_2_12()
{
	char	msg_num[20], title[20], repnum[20], temp[10], ufile[100], 
			header_msg[70], tail_msg[64], msg[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0, re_listen_count, ment_play ;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );


	printf("  멘트타입 : " );
	gets( temp );
	ment_play = atoi(temp);

	printf("  재청취 횟수 : " );
	gets( temp );
	re_listen_count = atoi(temp);

	printf(" 머릿말 : ");
	gets( header_msg );

	printf(" TTS메시지  : ");
	gets( msg );

	printf(" 맺음말 : ") ;
	gets( tail_msg );

	printf(" 수신자화일: ");
	gets( ufile );


	ret_flag = fn_x_submit_3( msgnum, 
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
		ment_play,
		0,
		call_dtmf,
		call_num,
		header_msg,
		tail_msg,
		re_listen_count,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}
*/
void help_submit2_2_11(int gid, char *message1, char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel, char *mssage3, int ment_play)
{
FILE *list;
//int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];
char wave_file[40]="";
char wave_file2[40]="";

sprintf(wave_file,"/DATA4/b2b/b2b_voice/sohi9/%s",message1);
sprintf(wave_file2,"/DATA4/b2b/b2b_voice/sohi9/%s",message3);


//데이터의 공백제거 
	strtok(send_list," ");
	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(reply_tel," ");
	strtok(cs_number," ");
	strtok(wave_file," ");
	strtok(wave_file2," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.

#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE,			// customer type
		0,						// file count
		"",					// file name
		j,						// addr count 
		send_list, 					// dest addr
		"", 					// sms
		message2,					// tts
		0, 						// content file count
		"",						// content file name
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		ment_play,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		wave_file,	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		wave_file2,		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag=fn_x_submit_3(	gid, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE,			// customer type
		0,						// file count
		"",					// file name
		j,						// addr count 
		send_list, 					// dest addr
		"", 					// sms
		message2,					// tts
		0, 						// content file count
		"",						// content file name
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		ment_play,
		0,
		0,
		cs_number,
		wave_file,
		wave_file2,
		3,
		&ret
	);
#endif
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
		printf("머릿말 분리 : [%s] 전송완료\n",subject);
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	}
	LeaveCriticalSection(&cs_db);
	return;
}


//머릿말 분리 
void help_submit2_2_12(int gid, char *message1, char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel, char *mssage3, int ment_play)
{
	
	UCHAR update_data[DELETE_SIZE]="";
	char *send="";
	char send_list[30];
	send=file_processing(gid,attach_file,subject);
	strcpy(send_list,send);
	char wave_file[40]="";
	char wave_file2[40]="";

/*
	if(strncmp(message1,"문자",4)==0)
	{
			sprintf(wave_file,"$TEXT$ /DATA4/b2b/b2b_voice/sohi9/%s",message1);
	}else{
			sprintf(wave_file,"/DATA4/b2b/b2b_voice/sohi9/%s",message1);
		}
*/
	sprintf(wave_file,"/DATA4/b2b/b2b_voice/sohi9/%s",message1);
	sprintf(wave_file2,"/DATA4/b2b/b2b_voice/sohi9/%s",message3);


//데이터의 공백제거 
	strtok(send_list," ");
	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(reply_tel," ");
	strtok(cs_number," ");
	strtok(wave_file," ");
	strtok(wave_file2," ");

#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		send_list,					// file name
		0,						// addr count 
		"", 					// dest addr
		"", 					// sms
		message2,					// tts
		0, 						// content file count
		"",						// content file name
		"",
		reply_type, 
		reply_count,
		0,   /* Expire Time */
		"", /* Return email address */
		ment_play,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		wave_file,	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		wave_file2,		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_3( 
		gid, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		send_list,					// file name
		0,						// addr count 
		"", 					// dest addr
		"", 					// sms
		message2,					// tts
		0, 						// content file count
		"",						// content file name
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		ment_play,
		0,
		0,
		cs_number,
		wave_file,
		wave_file2,
		3,
		&ret
	);
#endif	
	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("머릿말 분리 : [%s] 전송완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	LeaveCriticalSection(&cs_db);
	return;
}


///////////////////////////////////////
/*
void 
help_submit2_2_13()
{
	char	msg_num[20], title[20], repnum[20], 
			list[10000], list1[10000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf= 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_UPLOAD_TEMP, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}


void 
help_submit2_2_14()
{
	char	msg_num[20], title[20], repnum[20], 
			ufile[100],  infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_UPLOAD_FILE, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}

void 
help_submit2_2_15()
{
	char	msg_num[20], title[20], repnum[20],
			msg[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";
	int count ;

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	printf(" 반복수 : ");
	gets( in_str );
	count = atoi( in_str );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	for(i=0; i<count; i++)
	{
		ret_flag=fn_x_submit_2(	msgnum, 
			JOBTYPE_VOICE, 
			VOICE_TEMP_TEMP, 
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
			list,
			"", 				// sms
			msg, 				// tts
			0, 					// content file count
			"",
			"",
			reptype, 
			qnum, 
			0,
			"",
			0,
			0,
			call_dtmf,
			call_num,
			&ret
		);
		totalSendCount++ ;
		msgnum++ ;
		if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
 		else printf( " 성공적으로 수행되었습니다.  count[%d] \n", totalSendCount);
	}

	help_submit2_2();
}


void 
help_submit2_2_20()
{
	char	msg_num[20], title[20], repnum[20], kt_office[12],
			msg[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자리스트: ");
	gets(list);
	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_3(	msgnum, 
		JOBTYPE_VOICE, 
		//VOICE_TEMP_TEMP, 
		//VOICE_FILE_TEMP,
		VOICE_UPLOAD_TEMP,
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
		list,
		"", 				// sms
		"", 				// tts
		0, 					// content file count
		"",
		"/UMS/data/txq/pcm/bizclnt_20040622131206.vox",					//server
		reptype, 
		qnum, 
		0,
		"",
		0, //ment_play
		0,
		call_dtmf,
		call_num,
		"" , //"$TEXT$",
		"",
		1,
		&ret
	);

	printf( " TTS : %s \n", msg);

	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d]\n", ret_flag );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}
*/

//////////////////////////////////////////////////////////
	/*
void 
help_submit2_3()
{

	printf(" \n\n");
	printf(" 2-3-1. 팩스메시지 (동보 가능) \n");
	printf(" 2-3-2. 팩스메시지 (파일 전송) \n");
	printf(" 2-3-3. 업로드 팩스메시지 (동보 가능) \n");
	printf(" 2-3-4. 업로드 팩스메시지 (파일 전송) \n");


	printf(" x. 이전화면\n");
	printf(" 입력 : ");
	gets( in_str );
	if ( strncmp( in_str, "2-3-1", 5 ) == 0 ) help_submit2_3_1();
	else if ( strncmp( in_str, "2-3-2", 5 ) == 0 ) help_submit2_3_2();
	else if ( strncmp( in_str, "2-3-3", 5 ) == 0 ) help_submit2_3_3();
	else if ( strncmp( in_str, "2-3-4", 5 ) == 0 ) help_submit2_3_4();
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_3();
	
}



void 
help_submit2_3_1()
{
	char	msg_num[20], title[20], 
			list[10000], list1[10000], infile[100], server_file[100], *tmp;
	int		msgnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf( "첨부 화일 : " );
	gets(infile);

	printf( "서버 존재 화일 : " );
	gets(server_file);

	printf(" 수신자리스트: ");
	gets( list );

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_FAX, 
		FAX_TEXT_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		"",  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",					// file name
		1,					// addr count 
		list, 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		infile,
		server_file,
		0, 
		0, 
		0,
		"",
		0,
		0,
		0,
		"",
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_3();
}


void 
help_submit2_3_2()
{
	char	msg_num[20], title[20], ufile[100],
			infile[100];
	int		msgnum,  qnum, i = 0;


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

	ret_flag=fn_x_submit(	msgnum, 
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
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_3();
}
*/

void 
help_submit2_3_1(int gid,char *message1,char *msg_sdate,char *msg_edate,char *attach_file,char *subject)
{
FILE *list;
//int i;
int j;

char fax_file[40]="";
sprintf(fax_file,"d:\\sendfile\\%s",message1);

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

//데이터의 공백제거 
strtok(send_list," ");	
strtok(fax_file," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");
	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 

#if defined(SUBMIT4)
fn_x_submit_4 (
	gid,
	JOBTYPE_FAX, 
			FAX_TEXT_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			"",  
			"", 
			DEST_TEMP_TYPE,		// customer type
			0,					// customer file count
			"",				// file name
			j,					// addr count 
			send_list, 				// addr
			"", 				// sms
			"", 				// tts
			1, 					// content file count
			fax_file,
			"",
			0, 
			0, 
			0,   /* Expire Time */
			"", /* Return email address */
			0,   /*  0 : No ment, 1 : Ment */
			0,  /*  0 : default ( Select voice to hear ) */
			0,//dtmf_number
			"",
			"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
			"",		/* 맺음말 메시지, 파일만 사용 가능 */
			0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
			"251186",		/* 사원 번호 */
			&ret);
#else
	ret_flag=fn_x_submit_2(	gid, 
			JOBTYPE_FAX, 
			FAX_TEXT_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			"",  
			"", 
			DEST_TEMP_TYPE,		// customer type
			0,					// customer file count
			"",				// file name
			j,					// addr count 
			send_list, 				// addr
			"", 				// sms
			"", 				// tts
			1, 					// content file count
			fax_file,
			"",
			0, 
			0, 
			0,
			"",
			0,
			0,
			0,
			"",
			&ret
	);
#endif
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("팩스 동보 메시지 : [%s] 전송 완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	}
	LeaveCriticalSection(&cs_db);
	return;

}


//팩스즉시 
void 
help_submit2_3_2(int gid, char *message1,char *msg_sdate, char *msg_edate, char *attach_file, char *subject)
{
char fax_file[40]="";
sprintf(fax_file,"d:\\sendfile\\%s",message1);
UCHAR update_data[DELETE_SIZE]="";
char *send="";
char send_list[30];
send=file_processing(gid,attach_file,subject);
strcpy(send_list,send);

strtok(send_list," ");	
strtok(fax_file," ");
strtok(msg_sdate," ");
strtok(msg_edate," ");

/*	ret_flag = fn_x_submit_2( 
		gid, 
		JOBTYPE_VOICE, 
		FAX_UPLOAD_FILE, 
		"subject", 
		0, 
		msg_sdate,
		msg_edate,
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		file_name2,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		fax_file,
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		0,
		"",
		&ret
	);

  */


#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_FAX, 
		FAX_TEXT_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		send_list,				// file name
		0,					// addr count 
		"", 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		fax_file,
		"",
		0, 
		0, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		"",
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		0,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag=fn_x_submit(	gid, 
		JOBTYPE_FAX, 
		FAX_TEXT_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		send_list,				// file name
		0,					// addr count 
		"", 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		fax_file,
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		&ret
	);

#endif
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("팩스 메시지 : [%s] 전송 완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );
	return;
}

/*
void 
help_submit2_3_3()
{

	char	msg_num[20], title[20], 
			list[10000], list1[10000], infile[100], server_file[100], *tmp;
	int		msgnum, i = 0;
	char	upload_file_name[1000] ;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );

	printf( "업로드  화일 : " );
	gets( infile );

	fn_x_upload(infile, msgnum, &upload_file_name) ;

	printf( "서버 존재 화일 : %s\n",  upload_file_name);


	printf(" 수신자리스트: ");
	gets( list );

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_FAX, 
		FAX_TEXT_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		"",  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",					// file name
		1,					// addr count 
		list, 				// addr
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		"",
		upload_file_name,
		0, 
		0, 
		0,
		"",
		0,
		0,
		0,
		"",
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_3();

}


void 
help_submit2_3_4()
{
	char	msg_num[20], title[20], repnum[20], 
			ufile[100],  infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		FAX_UPLOAD_FILE, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}*/

//팩스 업로드---->사용안함
void help_submit2_3_4(int gid, char *message1, char *msg_sdate, char *msg_edate, char *attach_file,char *subject)
{

	char upload_file_name[1000]="";
	sprintf(message1,"d:\\sendfile\\%s",message1);
	UCHAR insert_data[INSERT_SIZE]="";;
	UCHAR update_data[DELETE_SIZE]="";
	FILE *fp=0;
	FILE *fp2=0;
	char file_name[30]="";
	char file_name2[30]="";
	int i=0;
	int index=0;
	char fbuf[200]="";
	char fbuf_c[200]="";
	char fbuf_c2[200]="";
	char *name="";
	char *tell="";
	char *temp_field="";
	char muge[5][20]={""};
	char merge[120]="";
	char *p="";
	char noinfo[7]="noinfo";
	fn_x_upload(message1,gid,&upload_file_name[0]) ;
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=12 WHERE gid=%d",gid);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}


	puts("//-------------------------------------------------------------------------------//");
	printf("팩스 업로드 메시지 : [%s] 전송중\n",subject);
	puts("//-------------------------------------------------------------------------------//");


	strcpy(file_name,attach_file);
	sprintf(file_name,"d:\\sendfile\\%s",attach_file);
	strtok(file_name," ");
	
	if((fp=fopen(file_name,"rw"))==NULL) puts("file 1 open error");
	
	strtok(file_name,".");
	sprintf(file_name2,"%s_.txt",file_name);

	
	if((fp2=fopen(file_name2,"w"))==NULL) puts("file 2 open error");

  
	int	ret=0;
	int cnt=0;

	while(1)
	{
		


		fgets(fbuf,200,fp);
		if(!strncmp(fbuf,"\n",1)){
			continue;
		}
		strcpy(fbuf_c,fbuf);
		if(!strncmp(fbuf,"EOF",3)){
			fflush(fp);
			fflush(fp2);
			fclose(fp);
			fclose(fp2);
			break;
		}
		if(strstr(fbuf_c,"&")!=NULL) strtok(fbuf_c,"&");
		strcpy(fbuf_c2,fbuf_c);
		fputs(fbuf_c2,fp2);
		fputs("\n",fp2);
			if(strstr(fbuf_c,"&")!=NULL)
			{
				p=strtok(NULL,"&");
				temp_field=(char *)malloc(strlen(p)+2);
				strcpy(temp_field,p);
			}else{
				temp_field=(char *)malloc(strlen(noinfo)+2);
				strcpy(temp_field,noinfo);
			}

		
		p=strtok(fbuf_c2,"^");
		name=(char *)malloc(strlen(p)+2);
		strcpy(name,p);
		
		
		p=strtok(NULL,"^");
		tell=(char *)malloc(strlen(p)+2);
		strcpy(tell,p);

//팩스에는 머지가 없다.
/*		
	for(index=0; ;++index)
		{
			p=strtok(NULL,"^"); 
			if(p==NULL)
			{
				break;		
			}
			strcpy(muge[index],p);
			strcat(muge[index],"^");
			strcat(merge,muge[index]);
				
		}
*/

strtok(tell,"\n");
Sleep(20);
//Sleep(150);
	hstmt=0;
              
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into MSG_RESULT(GID,DEST_NAME,DEST_ADDR,TEMP_FIELD) values(%d,'%s','%s','%s')",gid,name,tell,temp_field); 
	printf("insert_data=%s\n",insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	printf("insert=%d\n",_ret);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	free(name);
	free(temp_field);
	free(tell);
	

//	LeaveCriticalSection( &cs_db );
	

} //end while()	
//데이터의 공백제거 
/*for (i=1002; i >= 0; --i) {
		if (message2[i] != ' ') {
			message2[i+1] = '\0';
			break;
		}
	}
*/

strtok(msg_sdate," ");
strtok(msg_edate," ");

	ret_flag = fn_x_submit_2( 
		gid, 
		JOBTYPE_VOICE, 
		FAX_UPLOAD_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		"",  
		"", 
		DEST_FILE_TYPE,		// customer type
		1,					// customer file count
		file_name2,				// file name
		0,					// addr count 
		"",
		"", 				// sms
		"", 				// tts
		1, 					// content file count
		upload_file_name,
		"",
		0, 
		0, 
		0,
		"",
		0,
		0,
		0,
		"",
		&ret
	);
	
	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("팩스 업로드 메시지 : [%s] 전송 완료\n",subject);
	puts("//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

  LeaveCriticalSection( &cs_db );

return;
}


//////////////////////////////////////////
/*
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
	printf(" 2-4-9. 문자플러스 업로드 파일 (동보 가능) \n");
	printf(" 2-4-10. 문자플러스 업로드 파일 (파일 전송) \n");
	printf(" x. 이전화면  \n");
	printf(" 선택하세요 : ");
	gets( in_str );
	
	if ( strncmp( in_str, "2-4-10", 6 ) == 0 ) help_submit2_4_10();
	else if ( strncmp( in_str, "2-4-1", 5 ) == 0 ) help_submit2_4_1();
	else if ( strncmp( in_str, "2-4-2", 5 ) == 0 ) help_submit2_4_2();
	else if ( strncmp( in_str, "2-4-3", 5 ) == 0 ) help_submit2_4_3();
	else if ( strncmp( in_str, "2-4-4", 5 ) == 0 ) help_submit2_4_4();
	else if ( strncmp( in_str, "2-4-5", 5 ) == 0 ) help_submit2_4_5();
	else if ( strncmp( in_str, "2-4-6", 5 ) == 0 ) help_submit2_4_6();
	else if ( strncmp( in_str, "2-4-7", 5 ) == 0 ) help_submit2_4_7();
	else if ( strncmp( in_str, "2-4-8", 5 ) == 0 ) help_submit2_4_8();
	else if ( strncmp( in_str, "2-4-9", 5 ) == 0 ) help_submit2_4_9();

	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_4();
	
}



void 
help_submit2_4_1()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. [%d][%d]\n", ret_flag, ret );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_4();
}

*/
/*
void 
help_submit2_4_2()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	int	call_dtmf = 0;
	char	call_num[20] = "";
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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_4();
}
*/

void 
help_submit2_4_1(int gid, char *message1, char *message2, char *msg_sdate,char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number,char *subject,char *reply_tel)
{

FILE *list;
int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];
	char wave_file[1005];

	sprintf(wave_file,"d:\\sendfile\\%s",message2);
	for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}

	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(cs_number," ");
	strtok(reply_tel," ");
	strtok(wave_file," ");
	strtok(send_list," ");


	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.
#if defined(SUBMIT4)
fn_x_submit_4 (
	gid,
	JOBTYPE_SMS, 
		SPV_FILE_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE, 
		0,
		"", 
		j, 
		send_list, 
		message1,
		"", 
		1, 
		wave_file, 
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( gid, 
		JOBTYPE_SMS, 
		SPV_FILE_TEMP, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_TEMP_TYPE, 
		0,
		"", 
		j, 
		send_list, 
		message1,
		"", 
		1, 
		wave_file, 
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret);
#endif
		memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}

		hstmt=0;
		_ret=SQLAllocStmt(hdbc,&hstmt);
		{
		sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
		printf("update_data=%s\n",update_data);
		_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   		_ret=SQLExecute(hstmt);
		_ret=SQLFreeStmt(hstmt,SQL_DROP);
		}
  		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
		printf("문자 플러스 파일첨부 동보 : [%s] 전송완료\n",subject);
		puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	}
	LeaveCriticalSection(&cs_db);
	return ;
}



//문자 플러스 파일 첨부
void help_submit2_4_2(int gid, char *message1, char *message2, char *msg_sdate,char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number,char *subject,char *reply_tel)
{

	UCHAR update_data[DELETE_SIZE]="";
	char *send="";
	char send_list[30];
	send=file_processing(gid,attach_file,subject);
	strcpy(send_list,send);
	char wave_file[1005];
	int i;
	sprintf(wave_file,"d:\\sendfile\\%s",message2);
	for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}

	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(cs_number," ");
	strtok(reply_tel," ");
	strtok(wave_file," ");
	strtok(send_list," ");
#if defined(SUBMIT4)
fn_x_submit_4 (
		gid,
		JOBTYPE_SMS, 
		SPV_FILE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		message1,
		"", 
		1, 
		wave_file, 
		"",
		reply_type, 
		reply_count, 
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( gid, 
		JOBTYPE_SMS, 
		SPV_FILE_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE, 
		1,
		send_list, 
		0, 
		"", 
		message1,
		"", 
		1, 
		wave_file, 
		"",
		reply_type, 
		reply_count, 
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);
#endif
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
	printf("_ret=%d\n",_ret);
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("문자 플러스 파일첨부 메시지 : [%s] 전송완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );
	
return;
}

/*
void 
help_submit2_4_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_4();
}



void 
help_submit2_4_4()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	int		msgnum, reptype, qnum;

	int	call_dtmf = 0;
	char	call_num[20] = "";
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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf(" 첨부파일: ");
	gets( infile );
	printf(" 수신자파일: ");
	gets( ufile );

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_4();
}



void 
help_submit2_4_5()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			msg[10000], list[10000], list1[10000], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 문자 메시지  : ");
	gets( smsmsg );
	printf(" TTS 메시지  : ");
	gets( msg );
	printf("수신자리스트 : ");
	gets( list );

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_SMS, 
		SPV_TEMP_TEMP, 
		title, 
		0, 
		get_now(),
		get_now(),
		repnum,  
		repurl, 
		DEST_TEMP_TYPE,		/* customer type */
	/*	0,					// file count
		"",					// file name
		i,					// addr count 
		list,
		smsmsg, 				// sms
		msg, 				// tts
		0, 					// content file count
		"",
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_4();
}

*/
/*
void 
help_submit2_4_6()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			msg[10000], ufile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf("  문자메시지  : ");
	gets( smsmsg );
	printf(" TTS메시지  : ");
	gets( msg );
	printf(" 수신자화일: ");
	gets( ufile );


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_4();
}
*/


void 
help_submit2_4_5(int gid, char *message1, char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{
FILE *list;
int i;
int j;

UCHAR update_data[DELETE_SIZE]="";
char *send="";
char *send_file;
char send_list[1000];
//char *send_list_temp;
char list_buff[50];

//데이터의 공백제거 
for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}

for (i=1002; i >= 0; --i) {
		if (message2[i] != ' ') {
			message2[i+1] = '\0';
			break;
		}
	}

	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(cs_number," ");
	strtok(reply_tel," ");
	strtok(send_list," ");

	
	send=file_processing2(gid,attach_file,subject);
	send_file=strcat("d:\\sendfile\\",send);
	if((list=fopen(send_file,"r"))==NULL) puts("file open error");

	while(1)
	{
	
		for(j=0;j<=20;j++)
		{
			fgets(list_buff,50,list);
			if(!strncmp(list_buff,"EOF",3))
			{
				break;
			}
			list_buff[strlen(list_buff)-1]=0; //\n 을 없앤다.
			strncat(send_list,list_buff,strlen(list_buff));
			strcat(send_list,"|");
		}
		send_list[strlen(send_list)-1]=0; 	 //맨 마지막에 |를 없앤다.


#if defined(SUBMIT4)
fn_x_submit_4 (
			gid,
			JOBTYPE_SMS, 
			SPV_TEMP_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			reply_tel,  
			"", 
			DEST_TEMP_TYPE,			// customer type
			0,						// file count
			"",					// file name
			j,						// addr count 
			send_list, 					// dest addr
			message1,					// sms
			message2,					// tts
			0, 						// content file count
			"",					// content file name
			"",
			reply_type,		//reply_type
			reply_count,
			0,   /* Expire Time */
			"", /* Return email address */
			0,   /*  0 : No ment, 1 : Ment */
			0,  /*  0 : default ( Select voice to hear ) */
			0,//dtmf_number
			cs_number,
			"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
			"",		/* 맺음말 메시지, 파일만 사용 가능 */
			3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
			"251186",		/* 사원 번호 */
			&ret);
#else
	ret_flag=fn_x_submit_2(	gid, 
			JOBTYPE_SMS, 
			SPV_TEMP_TEMP, 
			subject, 
			0, 
			msg_sdate,
			msg_edate,
			reply_tel,  
			"", 
			DEST_TEMP_TYPE,			// customer type
			0,						// file count
			"",					// file name
			j,						// addr count 
			send_list, 					// dest addr
			message1,					// sms
			message2,					// tts
			0, 						// content file count
			"",					// content file name
			"",
			reply_type,		//reply_type
			reply_count,    //reply_count
			0,
			"",
			0,
			0,
			0,
			cs_number,
			&ret
			);
#endif
	memset(send_list,0,sizeof(send_list));
		if(!strncmp(list_buff,"EOF",3))
		{
			break;
		}


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
	printf("_ret=%d\n",_ret);
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("문자플러스 메시지 : [%s] 전송 완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	}
	LeaveCriticalSection( &cs_db );
	return;
}

//문자 플러스 TTS 변환 
void help_submit2_4_6(int gid, char *message1, char *message2, char *msg_sdate, char *msg_edate, char *attach_file, int reply_type, int reply_count, char *cs_number, char *subject, char *reply_tel)
{
UCHAR update_data[DELETE_SIZE]="";
int i;
char *send="";
char send_list[30];
send=file_processing(gid,attach_file,subject);
strcpy(send_list,send);

for (i=82; i >= 0; --i) {
		if (message1[i] != ' ') {
			message1[i+1] = '\0';
			break;
		}
	}

for (i=1002; i >= 0; --i) {
		if (message2[i] != ' ') {
			message2[i+1] = '\0';
			break;
		}
	}

	strtok(msg_sdate," ");
	strtok(msg_edate," ");
	strtok(cs_number," ");
	strtok(reply_tel," ");
	strtok(send_list," ");
#if defined(SUBMIT4)
	fn_x_submit_4 (
		gid,
		JOBTYPE_SMS, 
		SPV_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		send_list,					// file name
		0,						// addr count 
		"", 					// dest addr
		message1,					// sms
		message2,					// tts
		0, 						// content file count
		"",					// content file name
		"",
		reply_type,		//reply_type
		reply_count,
		0,   /* Expire Time */
		"", /* Return email address */
		0,   /*  0 : No ment, 1 : Ment */
		0,  /*  0 : default ( Select voice to hear ) */
		0,//dtmf_number
		cs_number,
		"",	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
		"",		/* 맺음말 메시지, 파일만 사용 가능 */
		3,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
		"251186",		/* 사원 번호 */
		&ret);
#else
	ret_flag = fn_x_submit_2( 
		gid, 
		JOBTYPE_SMS, 
		SPV_TEMP_FILE, 
		subject, 
		0, 
		msg_sdate,
		msg_edate,
		reply_tel,  
		"", 
		DEST_FILE_TYPE,			// customer type
		1,						// file count
		send_list,					// file name
		0,						// addr count 
		"", 					// dest addr
		message1,					// sms
		message2,					// tts
		0, 						// content file count
		"",					// content file name
		"",
		reply_type,		//reply_type
		reply_count,    //reply_count
		0,
		"",
		0,
		0,
		0,
		cs_number,
		&ret
	);
	
#endif
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=13, RET_RETURN=%d, RET_SUBMIT=%d  WHERE gid=%d",ret,ret_flag,gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
	printf("_ret=%d\n",_ret);
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");
	printf("문자플러스 메시지 : [%s] 전송 완료\n",subject);
	puts("//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//");

	LeaveCriticalSection( &cs_db );
	return;


}

/*

void 
help_submit2_4_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			list[10000], list1[10000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_4();
}



void 
help_submit2_4_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			ufile[100], infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 문자 메시지: ");
	gets( smsmsg );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 수신자화일: ");
	gets( ufile );


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_4();
}

void 
help_submit2_4_9()
{
	char	msg_num[20], title[20], repnum[20], 
			list[10000], list1[10000], infile[100], *tmp;
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf= 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		SPV_UPLOAD_TEMP, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}


void 
help_submit2_4_10()
{
	char	msg_num[20], title[20], repnum[20], 
			ufile[100],  infile[100];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		SPV_UPLOAD_FILE, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_2();
}


/////////////////////////////

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
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_5();
}



void 
help_submit2_5_1()
{
	char	msg_num[10], subject[20], replynum[20], msg[100], list[10000];
	char	*tmp, list1[10000];
	char	re_start[20], re_end[20];
	int		msgnum, ret, cnt= 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( subject );
	printf(" 회신번호 : ");
	gets( replynum );
	printf(" 메시지 : ");
	gets( msg );
	printf(" 수신자리스트 :  ");
	gets( list );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen(re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_5_1();
	}


	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		cnt ++;
		tmp = strtok( NULL, "|" );
	}
	

	ret_flag=fn_x_submit( msgnum,
		JOBTYPE_SMS,
		SMS_TEMP_TEMP,
		subject,
		0,
		re_start,
		re_end,
		replynum,
		"http^//www.xroshot.com",
		DEST_TEMP_TYPE,
		0,
		"",
		cnt,
		list,
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
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_5();
}



void 
help_submit2_5_2()
{
	char	msg_num[20], title[20], repnum[20], msg[100], filename[10000];
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

	printf(" filename : %s ", filename );


	ret_flag = fn_x_submit( msgnum, 
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


	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

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
/*	else if ( strncmp( in_str, "2-6-3", 5 ) == 0 ) help_submit2_6_3();
	else if ( strncmp( in_str, "2-6-4", 5 ) == 0 ) help_submit2_6_4();
	else if ( strncmp( in_str, "2-6-5", 5 ) == 0 ) help_submit2_6_5();
	else if ( strncmp( in_str, "2-6-6", 5 ) == 0 ) help_submit2_6_6();
	else if ( strncmp( in_str, "2-6-7", 5 ) == 0 ) help_submit2_6_7();
	else if ( strncmp( in_str, "2-6-8", 5 ) == 0 ) help_submit2_6_8();
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_6();
}



void 
help_submit2_6_1()
{
	char	msg_num[20], title[20], repnum[20],
			file[100], list[10000], list1[10000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_6();
}



void 
help_submit2_6_2()
{
	char	msg_num[20], title[20], repnum[20], 
			infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;
	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_6();
}



void 
help_submit2_6_3()
{
	char	msg_num[20], title[20], repnum[20],
			file[100], list[10000], list1[10000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_6();
}



void 
help_submit2_6_4()
{
	char	msg_num[20], title[20], repnum[20], 
			infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;
	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag=fn_x_submit_2(	msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_6();
}


void 
help_submit2_6_5()
{
	char	msg_num[20], title[20], repnum[20],  
			msg[10000], list[10000], list1[10000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" TTS메시지  : ");
	gets( msg );
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
		help_submit2_6_5();
	}

	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_TEMP, 
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
		list,
		"", 				// sms
		msg, 				// tts
		1, 					// content file count
		infile,
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_6();
}



void 
help_submit2_6_6()
{
	char	msg_num[20], title[20], repnum[20],  
			msg[10000], ufile[100], infile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" TTS메시지  : ");
	gets( msg );
/*	printf(" 수신자화일: ");
	gets( ufile );
	printf( "첨부 화일 : " );
	gets(infile);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ))
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_6_6();
	}



	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_VOICE, 
		VOICE_TEMP_FILE, 
		title, 
		0, 
		re_start,
		re_end,
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_6();
}


void 
help_submit2_6_7()
{
	char	msg_num[20], title[20], repnum[20],
			list[10000], list1[10000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
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
		"",						// file nam'e
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_6();
}


void 
help_submit2_6_8()
{
	char	msg_num[20], title[20], repnum[20],
			ufile[100], infile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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



	ret_flag = fn_x_submit_2( msgnum, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
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
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_7();
}



void 
help_submit2_7_1()
{
	char	msg_num[20], title[20],  
			list[10000], list1[10000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, i = 0;


	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf( "첨부 화일 : " );
	gets(infile);
	printf(" 수신자리스트: ");
	gets( list );

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if (( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_7_1();
	}


	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit(	msgnum, 
		JOBTYPE_FAX, 
		FAX_TEXT_TEMP, 
		title, 
		0, 
		re_start,
		re_end,
		"",  
		"", 
		DEST_TEMP_TYPE,		// customer type
		0,					// customer file count
		"",					// file name
		2,					// addr count 
		list, 				// addr
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
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_7();
}



void 
help_submit2_7_2()
{
	char	msg_num[20], title[20], ufile[100],
			infile[100];
	char	re_start[20], re_end[20];
	int		msgnum, i = 0;


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


	ret_flag=fn_x_submit(	msgnum, 
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
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
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
	else if ( in_str[0] == 'x' ) help_submit2();
	else help_submit2_8();
}



void 
help_submit2_8_1()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[10000], list1[10000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
/*		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_8();
}



void 
help_submit2_8_2()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;
	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_8();
}



void 
help_submit2_8_3()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			file[100], list[10000], list1[10000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_8();
}



void 
help_submit2_8_4()
{
	char	msg_num[20], title[20], repnum[20], smsmsg[100],
			repurl[50], infile[100], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum;
	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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


	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");

	help_submit2_8();
}



void 
help_submit2_8_5()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[100],
			msg[10000], list[10000], list1[10000], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

	printf(" 문자 메시지  : ");
	gets( smsmsg );
	printf(" TTS 메시지  : ");
	gets( msg );
	printf( "수신자리스트 : " );
	gets( list);

	printf(" 예약 시작 시간 (YYYYMMDDHHMI) :  ");
	gets( re_start );
	printf(" 예약 종료 시간 (YYYYMMDDHHMI) :  ");
	gets( re_end );

	if ( ( strlen( re_start) != 12 ) || ( strlen( re_end) != 12 ) )
	{
		printf(" 날짜를 다시 입력하세요.. \n");
		help_submit2_8_5();
	}


	strcpy( list1, list );

	tmp = strtok( list1, "|" );
	while ( tmp != NULL )
	{
		i ++;
		tmp = strtok( NULL, "|" );
	}

	ret_flag=fn_x_submit_2(	msgnum, 
		JOBTYPE_SMS, 
		SPV_TEMP_TEMP, 
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
		list,
		smsmsg, 				// sms
		msg, 				// tts
		0, 					// content file count
		"",
		"",
		reptype, 
		qnum, 
		0,
		"",
		0,
		0,
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_8();
}



void 
help_submit2_8_6()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			msg[10000], ufile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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



	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_8();
}


void 
help_submit2_8_7()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			list[10000], list1[10000], infile[100], *tmp;
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

	printf("\n\n");
	printf(" 메시지ID : ");
	gets( msg_num );
	msgnum = atoi( msg_num );
	printf(" 제목 : ");
	gets( title );
	printf(" 회신번호 : ");
/*	gets( repnum );
	printf(" 회신URL : ");
	gets( repurl );
	printf(" 응답유형[0:답변요청없음, 1:설문조사, 2:답변녹음, 3:설문+답변\n");
	printf(" 입력 : " );
	gets( in_str );
	reptype = atoi(in_str);
	printf(" 설문항목수 : ");
	gets( in_str );
	qnum = atoi( in_str );

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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

	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret 
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_8();
}


void 
help_submit2_8_8()
{
	char	msg_num[20], title[20], repnum[20], repurl[50], smsmsg[10000],
			ufile[100], infile[100];
	char	re_start[20], re_end[20];
	int		msgnum, reptype, qnum, i = 0;

	int	call_dtmf = 0;
	char	call_num[20] = "";

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

	printf("  상담원 연결 : " );
	gets( call_num );
	call_dtmf = atoi(call_num);
	printf("  상담원 번호: " );
	gets( call_num );

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



	ret_flag = fn_x_submit_2( msgnum, 
		JOBTYPE_SMS, 
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
		call_dtmf,
		call_num,
		&ret
	);
	if ( ret_flag != X_RESULT_SUCCESS ) printf( "에러 발생입니다. \n" );
 	else printf( " 성공적으로 수행되었습니다. \n");
	help_submit2_8();
}
*/

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
	char	stime[20];
	STATUSINFO  ret_stat;

	printf(" Start Time (yyyymmdd) : ");
	gets( stime );

	ret_stat = fn_x_status( 
		stime,
		1,
		&ret
	);

	if (ret == 0) {
		printf( "Status Complete..... JOB ID : %d \n", ret_flag );

		printf( "msgid[%d], jobid[%d], subject[%s] \n",
		ret_stat.msgid, ret_stat.jobid, ret_stat.subject);
		printf( "stime[%s], etime[%s], reserved count[%d] \n",
		ret_stat.schedule_stime, ret_stat.schedule_etime, ret_stat.reserved_count);
		printf( "content_size[%d], success[%d], fail[%d] \n",
		ret_stat.content_size, ret_stat.success_count, ret_stat.fail_count);
	}

	help_status3();
}



void 
help_status3_2()
{
	int		jobid;
	STATUSDETAIL_INFO  ret_stat;

	printf(" JOBID : ");
	gets( in_str );
	jobid = atoi( in_str );

	ret_stat = fn_x_statusd( jobid, &ret);

	if (ret == 0) { 
		printf( "Status_Detail Complete..... JOB ID : %d \n", ret_flag );

		printf( "msgid[%d], jobid[%d], count[%d]\n",
		ret_stat.msgid, ret_stat.jobid, ret_stat.result_count);
		printf( "Result Content = %s\n", ret_stat.result_content);
	}

	help_status3();
}



void 
help_cancel4()
{
	int		jobid;

	printf(" JOBID : ");
	gets( in_str );
	jobid = atoi( in_str );

	ret_flag = fn_x_cancel( jobid, 
		0,
		&ret
	);
	printf( "Cancel Complete..... JOB ID : %d \n", ret_flag );
	help_basic();
}


//cancel ==> cancel할 메시지의 gid를 인자로 받는다.
void cancel(int gid)
{
	int jobid;
	SDWORD jobid_len=SQL_NTS;
	UCHAR select_[160];
	UCHAR delete_[128];
	UCHAR update_[200];
	HSTMT		hstmt_gc;
	int jobid_cancel;
	int ret_cancel;


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_,"UPDATE MSG SET JOB_STATUS=JOB_STATUS+4 WHERE gid=%d",gid);
	printf("update_data=%s\n",update_);
	_ret=SQLPrepare(hstmt,update_,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

puts("//============================================================//");
printf("예약 취소 시작 -> [gid=%d]\n",gid);
puts("//============================================================//");

	hstmt_gc = 0;
	SQLAllocStmt(hdbc,&hstmt_gc);
	{
	sprintf((char *)select_,"select distinct jobid from msg_result where gid=%d",gid);
	SQLPrepare(hstmt_gc, select_, SQL_NTS);
	SQLBindCol(hstmt_gc, 1, SQL_C_LONG,(PTR) &jobid,(SDWORD)sizeof(jobid), &jobid_len);
	_ret = SQLExecute(hstmt_gc);
	SQLFetch( hstmt_gc );
	_ret =SQLFreeStmt(hstmt_gc, SQL_DROP) ;
	}
//cancel() 함수 실행	
	jobid_cancel = fn_x_cancel( jobid,0,&ret_cancel	);

if(ret_cancel==0 && jobid_cancel==1)	
{
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_,"delete from msg_result where gid=%d",gid);
	_ret=SQLPrepare(hstmt,delete_,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)delete_,"delete from msg_group where gid=%d",gid);
	_ret=SQLPrepare(hstmt,delete_,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	printf("del=%d\n",_ret);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}


	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_,"UPDATE MSG SET RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret_cancel,jobid_cancel,gid);
	printf("update_data=%s\n",update_);
	_ret=SQLPrepare(hstmt,update_,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
puts("//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//");
printf("예약 취소 성공 -> [gid=%d]\n",gid);
puts("//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//");

}else{

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_,"UPDATE MSG SET JOB_STATUS=20, RET_RETURN=%d, RET_SUBMIT=%d WHERE gid=%d",ret_cancel,jobid_cancel,gid);
	printf("update_data=%s\n",update_);
	_ret=SQLPrepare(hstmt,update_,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
	puts("//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//");
	printf("예약 취소 실패 -> [gid=%d]\n",gid);
	puts("//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//");
	}

	LeaveCriticalSection(&cs_db);
return;
}



void 
help_report()
{
	help_basic();
}


/*
char *get_now()
{
#if ( defined(WIN32) || defined(_WIN32) )
	static char  str[20];
	char *ret;
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
#else
	static char str[20];
	char	*ret;
	time_t	t;
	struct tm *systm;

	t = time( NULL );
	t += 60;

	systm = localtime( &t );

	sprintf( str, "%.4d%.2d%.2d%.2d%.2d",
		systm->tm_year + 1900,
		systm->tm_mon + 1,
		systm->tm_mday,
		systm->tm_hour,
		systm->tm_min);

	ret = str;
	
	return ret;
#endif
}
*/

char *get_now() //연도 날짜 시간 분
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


char *get_date() //연도 날짜 
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


char *get_time() //시간 초 초/10
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

char *get_time2() //시간 분
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

int	lpfn_submit( int ret_code, char *session_id, int msg_id, int job_id, int fee, int content_size )
{
	/*
	 * 이함수는 콜백 함수로 서버에서 들어 온 jobid 를 받아 큐에 저장한다.
	 * 저장된 jobid 는 스래드에 의해 데이타 베이스에 저장 ( 스래드 동기화 요구 !!! ) 
	*/

	//int count;
	int s[3];
	s[0]=msg_id;
	s[1]=job_id;
	s[2]=ret_code;
	int cnt;
	

	printf("[CALL BACK (submin)] : lpfn_submit job_id[%d] ret_code[%d] <> ", job_id, ret_code);

	// put(...) 함수는 스래드와 동기화를 위한 루틴이 포함되어 있다.
    //count = put(submit_set[0],submit_set[1]);
//	put(submit_set[0],submit_set[1],submit_set[2]);

	//printf("[put()] count : %d \n", count);


		EnterCriticalSection( &cs_db );
		hstmt1=0;
		_ret=SQLAllocStmt(hdbc,&hstmt1);

		UCHAR submit_ret[200];
		UCHAR submit_bak[200];

		sprintf((char *)submit_ret,"UPDATE MSG SET RET_CALLBACK=%d WHERE GID=%d",s[2],s[0]);
		_ret=SQLPrepare(hstmt1,submit_ret,SQL_NTS); 
		printf("submit _ret1=%d",_ret);
		_ret=SQLExecute(hstmt1);
		printf("submit _ret2=%d",_ret);
		SQLFreeStmt(hstmt1,SQL_DROP);


		_ret=SQLAllocStmt(hdbc,&hstmt1);
		sprintf((char *)submit_bak,"UPDATE MSG_RESULT SET JOBID=%d WHERE GID=%d",s[1],s[0]);
		printf("submit_bak=%s\n",submit_bak);

		SQLPrepare(hstmt1,submit_bak,SQL_NTS); 

		
		while ( (_ret=SQLExecute(hstmt1) == -1)) {	// Error  이면 다시 데이타 베이스에 저장.
			fprintf(stderr, "SQL 질의 재실행[1][%d] !!\n", cnt);
			if( (--cnt) < 2 ) break;
			//submit_err(s[0], s[1]);
			Sleep(40);
		}
		cnt=10;

		db_error((UCHAR *)"[스래드_1] SQLEXE ", henv, hdbc, hstmt1);
		SQLFreeStmt(hstmt1,SQL_DROP);
		LeaveCriticalSection( &cs_db );
    return 1;
}

// 레포트
/*
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

totalReportCount++;
Sleep(5);

char dest[15]="";
char s_time2[15]="";
char e_time2[15]="";
char r_file2[60]="";
strcpy(dest,dest_addr);
strcpy(s_time2,start_time);
strcpy(e_time2,end_time);
strcpy(r_file2,replied_file);
dnode_2 *t;
if((t=(dnode_2 *)malloc(sizeof(dnode_2)))==NULL)
{
	puts("Memory can not alloction");
	return -1;
}


t->job_id	=	job_id;
t->ret_code	=	ret_code;
t->talktime	=	duration;
t->pages	=	pages;
t->fee		=	fee;
t->dtmf		=	reply_dtmf;
strcpy(t->tel,dest);
strcpy(t->s_time,s_time2);
strcpy(t->e_time,e_time2);
strcpy(t->replied_file,r_file2);
t->totalReportCount=totalReportCount;
tail->prev->next = t;
t ->prev = tail ->prev;
tail->prev=t;
t->next=tail;
	++k_2;	

	
	// 입력 자료의 갯수
	if (q_flag_2 == 1) {
		// 세마포어(1) unlock
		LeaveCriticalSection( &cs_2 );
		t->job_id	=	job_id;
		t->ret_code	=	ret_code;
		t->talktime	=	duration;
		t->pages	=	pages;
		t->fee		=	fee;
		t->dtmf		=	reply_dtmf;
		strcpy(t->tel,dest);
		strcpy(t->s_time,s_time2);
		strcpy(t->e_time,e_time2);
		strcpy(t->replied_file,r_file2);
		t->totalReportCount=totalReportCount;
		printf("[Q_2 : 자료가 다시 들어 갑니다.\n");
//		printf("%d, %d, %d, %d, %d, %d, %s\n",	Q_2[ R_2 ].job_id, Q_2[ R_2 ].ret_code,
//				Q_2[ R_2 ].talktime, Q_2[ R_2 ].pages, 	Q_2[ R_2 ].fee, Q_2[ R_2 ].dtmf, 
//				Q_2[ R_2 ].tel);
		
		q_flag_2 = 0;
	}

	if(k_2>=10000) Sleep(240000);
	printf("[%d]번째 자료가 큐에 들어갑니다.\n",k_2);
	LeaveCriticalSection(&cs_2);
	
	return 1;
}
*/

int	lpfn_report_v2(
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
char*	replied_file,
int		msgid)
{
	totalReportCount ++;

Sleep(5);

char dest[15]="";
char s_time2[15]="";
char e_time2[15]="";
char r_file2[60]="";
strcpy(dest,dest_addr);
strcpy(s_time2,start_time);
strcpy(e_time2,end_time);
strcpy(r_file2,replied_file);
dnode_2 *t;
if((t=(dnode_2 *)malloc(sizeof(dnode_2)))==NULL)
{
	puts("Memory can not alloction");
	return -1;
}


t->job_id	=	job_id;
t->msgid	=	msgid;
t->ret_code	=	ret_code;
t->talktime	=	duration;
t->pages	=	pages;
t->fee		=	fee;
t->dtmf		=	reply_dtmf;
strcpy(t->tel,dest);
strcpy(t->s_time,s_time2);
strcpy(t->e_time,e_time2);
strcpy(t->replied_file,r_file2);
t->totalReportCount=totalReportCount;
tail->prev->next = t;
t ->prev = tail ->prev;
tail->prev=t;
t->next=tail;
	++k_2;	

	
	// 입력 자료의 갯수
	if (q_flag_2 == 1) {
		// 세마포어(1) unlock
		LeaveCriticalSection( &cs_2 );
		t->job_id	=	job_id;
		t->msgid	=	msgid;
		t->ret_code	=	ret_code;
		t->talktime	=	duration;
		t->pages	=	pages;
		t->fee		=	fee;
		t->dtmf		=	reply_dtmf;
		strcpy(t->tel,dest);
		strcpy(t->s_time,s_time2);
		strcpy(t->e_time,e_time2);
		strcpy(t->replied_file,r_file2);
		t->totalReportCount=totalReportCount;
		printf("[Q_2 : 자료가 다시 들어 갑니다.\n");
//		printf("%d, %d, %d, %d, %d, %d, %s\n",	Q_2[ R_2 ].job_id, Q_2[ R_2 ].ret_code,
//				Q_2[ R_2 ].talktime, Q_2[ R_2 ].pages, 	Q_2[ R_2 ].fee, Q_2[ R_2 ].dtmf, 
//				Q_2[ R_2 ].tel);
		
		q_flag_2 = 0;
	}

	if(k_2>=10000) Sleep(240000);
	printf("[%d]번째 자료가 큐에 들어갑니다.\n",k_2);
	LeaveCriticalSection(&cs_2);


/*
	printf("[REPORT_CB] : total[%d] session_id [%s] \n"
			"\t job id [%d:%d] msgid[%d] \n"
			"\t dest_addr [%s] \n"
			"\t ret_code  [%d] \n"
			"\t start time | end time [%s|%s] \n"
			"\t duration, pages [%d:%d] \n"
			"\t fee [%d] \n"
			"\t reply_dtmf [%d] \n"
			"\t replied_file [%s] \n",
			totalReportCount,
			session_id,
			job_id,
			subjob_id,
			msgid,
			dest_addr,
			ret_code,
			start_time,
			end_time,
			duration,
			pages,
			fee, 
			reply_dtmf,
			replied_file );
*/
	return 1;
}


// *************** Queue ***********************
// 초기화 
void init_queue(void)
{

	head=(dnode_2 *)malloc(sizeof(dnode_2));
	tail=(dnode_2 *)malloc(sizeof(dnode_2));
	head -> prev = head;
	head -> next = tail;
	tail -> prev = head;
	tail -> next = tail;
}

DWORD WINAPI getThread_2(void *args)
{

	int s[8]={0};
		
	SQLRETURN res;  
	UCHAR report_bak[1000]="";
	int cnt=10;
	fprintf(stdout, "Thread BLOCK[2]!!\n");
	EnterCriticalSection( &cs_2 );	// 최초 대기 상태
	fprintf(stdout, "Thread First[2] - Start!! \n");
	
	char *tel="";
	char *s_time="";
	char *e_time="";
	char *r_file="";
	
	while (1) {
			dnode_2 *t;
			t=head->next;
		if (t == tail) {			// 데이타가 없다 
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
		s[1] = t->msgid;
		s[2] = t->ret_code;
		s[3] = t->talktime;
		s[4] = t->pages;
		s[5] = t->fee;
		s[6] = t->dtmf;
		s[7] = t->totalReportCount;
		
		tel  = t->tel;
		s_time = t->s_time;
		e_time = t->e_time;
		r_file = t->replied_file;

		printf("[report CB] 질의 대기중...\n");
		EnterCriticalSection( &cs_db );
		printf("[report CB] 질의 시작\n");

		th_1=0;
		res=SQLAllocStmt(hdbc,&th_1);
		sprintf((char *)report_bak, "UPDATE MSG_RESULT SET RET_REPORT=%d,DURATION=%d,PAGE=%d,FEE=%d,STARTTIME='%s',ENDTIME='%s',REPLIED_FILE='%s' where GID=%d and DEST_ADDR='%s'",s[2],s[3],s[4],s[5],s_time,e_time,r_file,s[1],tel);

//		sprintf((char *)report_bak, "UPDATE MSG_RESULT SET RET_REPORT=%d,DURATION=%d,PAGE=%d,FEE=%d,STARTTIME='%s',ENDTIME='%s',REPLIED_FILE='%s' where JOBID=%d and DEST_ADDR='%s'",t->ret_code,t->talktime,t->pages,t->fee,s_time,e_time,r_file,t->job_id,tel);

		printf("report_bak=%s\n",report_bak);
		res=SQLPrepare(th_1, report_bak, SQL_NTS);
		printf("totalReportCount=%d\n",s[6]);
		 
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
	
	--k_2;
	printf("현재 큐에 있는 데이터 : [%d개]\n",k_2);
	LeaveCriticalSection( &cs_db);
	printf("동기화 해제 \n");
	}//while(1)
//	free(t);
	return 1;
}

DWORD WINAPI build(void *)
{
//	EnterCriticalSection(&cs_bu);
//	puts("/*********************************************************/");
//	printf("* Xroshot Message Server by %s @ Hansung Build ver 2.01 *\n",dev);
//	puts("/*********************************************************/");
//	Sleep(3000);
//	fn_x_statusd( 58838492, &ret);
//	LeaveCriticalSection(&cs_bu);
	return 1;
}



void 
help_uadd()
{
	char	uid[21], uname[21], pw[20], mobile_no[30], mo_pw[5], reg_code[20];
	int		ret, i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}
	
	printf("\n\n");

	printf(" USER ID : ");
	gets( uid );

	printf(" USER NAME : ");
	gets( uname );
	
	printf(" P/W : ");
	gets( pw );

	printf(" MOBILE NO. : ");
	gets( mobile_no );

	printf(" MOBILE PW. : ");
	gets( mo_pw );

	printf(" REG CODE : ");
	gets( reg_code );

	i = fn_x_useradd( uid, uname, pw, mobile_no, mo_pw, reg_code, &ret );

	printf( " USER ADD FTN [%d], RET [%d] \n", i, ret );

	help_basic();
}


void
help_uadd2()
{
	char	uname[21], mail_pw[20], ivr_pw[20], mobile_no[30], reg_code[20],
			m_host[20], m_hostid[20], p_host[20], p_hostid[20];
	int		ret, i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}
	
	printf("\n\n");
	printf(" USER NAME : ");
	gets( uname );
	
	printf(" MAIL P/W : ");
	gets( mail_pw );

	printf(" IVR P/W : ");
	gets( ivr_pw );

	printf(" MOBILE NO. : ");
	gets( mobile_no );

	printf(" REG CODE : ");
	gets( reg_code );

	printf(" MAIL HOST : ");
	gets( m_host );

	printf(" MAIL HOST ID : ");
	gets( m_hostid );

	printf(" POP3 HOST : ");
	gets( p_host );

	printf(" POP3 HOST ID : ");
	gets( p_hostid );

	i = fn_x_useradd_2( uname, mail_pw, ivr_pw, mobile_no, reg_code, m_host, m_hostid, p_host, p_hostid, &ret );

	printf( " USER ADD 2 FTN [%d], RET [%d] \n", i, ret );

	help_basic();

}



void
help_uchange()
{
	char	uname[21], mail_pw[20], ivr_pw[20], mobile_no[30], reg_code[20];
	int		ret, i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}
	
	printf("\n\n");
	printf(" USER NAME : ");
	gets( uname );
	
	printf(" MAIL P/W : ");
	gets( mail_pw );

	printf(" IVR P/W : ");
	gets( ivr_pw );

	printf(" MOBILE NO. : ");
	gets( mobile_no );

	printf(" REG CODE : ");
	gets( reg_code );

	i = fn_x_userchange( uname, mail_pw, ivr_pw, mobile_no, reg_code, &ret );

	printf( " USER CHANGE FTN [%d], RET [%d] \n", i, ret );

	help_basic();
}



void
help_uchangeraw()
{
	char str[10000];
	int ret, i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}

	printf( " \n\n ");

	printf(" STRING  : ");
	gets( str );

	i = fn_x_userchangeraw( str, &ret );

	printf( " USER CHANGE RAW FTN [%d], RET [%d] \n", i, ret );

	help_basic();
}



void
help_udel()
{
	int ret, i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}

	printf( " \n\n ");

	i = fn_x_userdel( &ret );

	printf( " USER DELETE FTN [%d], RET [%d] \n", i, ret );

	help_basic();
}



void 
help_uinfo()
{
	int			ret;	
	USERINFO	i;

	if ( bind_flag == 0 ) 
	{
		printf("BIND를 먼저 하세요...\n ");
		help_basic();
	}

	printf( " \n\n ");

	i = fn_x_userinfo( &ret );

	printf( " USER INFO FTN [%d], RET [%d] \n", i, ret );

	help_basic();
}



void 
help_reportfile()
{
	char	retnum;
	int		ret, loop;

	if ( bind_flag == 1 )
	{
		printf("벌써 BIND하셨습니다.\n");
		help_basic();
	}

	printf("\n\n");

	printf(" 이용자ID : ");
	gets( USERID );
	printf(" 비밀번호 : ");
	gets( USERPW );
	printf(" s : 전송\n" );
	printf(" x : 취소\n");
	printf(" 입력 : ");
	gets( in_str );

	for ( loop = 0 ; loop < 5 ; loop ++ )
	{
	if ( in_str[0] == 's' ) 
	{
		retnum = fn_x_bind(	0, USERID, USERPW, "xroshot.com", 
			1, lpfn_submit,
			1, lpfn_report_v1,
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
			case 0x00 :
				printf( "바인드 되었습니다. \n" );
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
#if ( defined(WIN32) || defined(_WIN32) )
	Sleep(1000);
#else
	sleep(1);
#endif

	}
	else if ( in_str[0] == 'x' ) help_basic();
	else help_bind1();
	}
}

char *file_processing(int gid,char *attach_file,char *subject)
{
	UCHAR insert_data[INSERT_SIZE]="";
	UCHAR update_data[DELETE_SIZE]="";
	FILE *fp=0;
	FILE *fp2=0;
	char file_name[30]="";
	char file_name2[30]="";
	int i=0;
	int index=0;
	char fbuf[200]="";
	char fbuf_c[200]="";
	char fbuf_c2[200]="";
	char *name="";
	char *tell="";
	char *temp_field="";
	char muge[5][20]={""};
	char merge[120]="";
	char *p="";
	char noinfo[7]="noinfo";
	char *file_name3;
	int count=0;
	
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=12 WHERE gid=%d",gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//----------------------------------------------------------------------------//");
	printf("\t\t메시지 : [%s] 전송중\n",subject);
	puts("//----------------------------------------------------------------------------//");


	strcpy(file_name,attach_file);
	sprintf(file_name,"d:\\sendfile\\%s",attach_file);
	strtok(file_name," ");
	
	if((fp=fopen(file_name,"rw"))==NULL) puts("file 1 open error");
	strtok(file_name,".");
	sprintf(file_name2,"%s_.txt",file_name);

	
	if((fp2=fopen(file_name2,"w"))==NULL) puts("file 2 open error");

  
	int	ret=0;
	int cnt=0;
//이름^전화번호^머지1^머지2.....&증번호
	while(1)
	{
		fgets(fbuf,200,fp);
		if(!strncmp(fbuf,"\n",1)){
			continue;
		}
		strcpy(fbuf_c,fbuf);
		if(!strncmp(fbuf,"EOF",3)){
			break;
		}
		if(strstr(fbuf,"&")!=NULL) strtok(fbuf_c,"&");
		strcpy(fbuf_c2,fbuf_c);
		fputs(fbuf_c2,fp2);
		if(strstr(fbuf,"&")!=NULL) fputs("\n",fp2);
		if(strstr(fbuf,"&")!=NULL)
			{
				p=strtok(NULL,"&");
				temp_field=(char *)malloc(strlen(p)+2);
				strcpy(temp_field,p);
			}else{
				temp_field=(char *)malloc(strlen(noinfo)+2);
				strcpy(temp_field,noinfo);
			}

		
		p=strtok(fbuf_c2,"^");
		name=(char *)malloc(strlen(p)+2);
		strcpy(name,p);
		
		
		p=strtok(NULL,"^");
		tell=(char *)malloc(strlen(p)+2);
		strcpy(tell,p);
			
		
	for(index=0; ;++index)
		{
			p=strtok(NULL,"^"); 
			if(p==NULL)
			{
				break;		
			}
			strcpy(muge[index],p);
			strcat(muge[index],"^");
			strcat(merge,muge[index]);
				
		}


	strtok(tell,"\n");
	Sleep(5);
	hstmt=0;
              
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into MSG_RESULT(GID,DEST_NAME,DEST_ADDR,MERGE,TEMP_FIELD) values(%d,'%s','%s','%s','%s')",gid,name,tell,merge,temp_field); 
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	free(name);
	free(temp_field);
	free(tell);
	memset(merge,0,120);
	++count;
} //end while()	

file_name3 = file_name2;
printf("-------------------------total : [ %d]개 파일처리 완료!!------------------------\n",count);
	fflush(fp);
	fflush(fp2);
	fclose(fp);
	fclose(fp2);
return file_name3;
}


char *file_processing2(int gid,char *attach_file,char *subject)
{
	UCHAR insert_data[INSERT_SIZE]="";
	UCHAR update_data[DELETE_SIZE]="";
	FILE *fp=0;
	FILE *fp2=0;
	char file_name[30]="";
	char file_name2[30]="";
	int i=0;
	int index=0;
	char fbuf[200]="";
	char fbuf_c[200]="";
	char fbuf_c2[200]="";
	char *name="";
	char *tell="";
	char *temp_field="";
	char muge[5][20]={""};
	char merge[120]="";
	char *p="";
	char noinfo[7]="noinfo";
	char *file_name3;
	int count=0;
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=12 WHERE gid=%d",gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	puts("//----------------------------------------------------------------------------//");
	printf("\t\t메시지 : [%s] 전송중\n",subject);
	puts("//----------------------------------------------------------------------------//");


	strcpy(file_name,attach_file);
	sprintf(file_name,"d:\\sendfile\\%s",attach_file);
	strtok(file_name," ");
	
	if((fp=fopen(file_name,"rw"))==NULL) puts("file 1 open error");
	strtok(file_name,".");
	sprintf(file_name2,"%s_.txt",file_name);

	
	if((fp2=fopen(file_name2,"w"))==NULL) puts("file 2 open error");

  
	int	ret=0;
	int cnt=0;

	while(1)
	{
		fgets(fbuf,200,fp);
		if(!strncmp(fbuf,"\n",1)){
			continue;
		}
		strcpy(fbuf_c,fbuf);
		if(!strncmp(fbuf,"EOF",3)){
			break;
		}
		if(strstr(fbuf,"&")!=NULL) strtok(fbuf_c,"&");
		strcpy(fbuf_c2,fbuf_c);
		fputs(fbuf_c2,fp2);
		if(strstr(fbuf,"&")!=NULL) fputs("\n",fp2);
		if(strstr(fbuf,"&")!=NULL)
			{
				p=strtok(NULL,"&");
				temp_field=(char *)malloc(strlen(p)+2);
				strcpy(temp_field,p);
			}else{
				temp_field=(char *)malloc(strlen(noinfo)+2);
				strcpy(temp_field,noinfo);
			}

		
		p=strtok(fbuf_c2,"^");
		name=(char *)malloc(strlen(p)+2);
		strcpy(name,p);
		
		
		p=strtok(NULL,"^");
		tell=(char *)malloc(strlen(p)+2);
		strcpy(tell,p);
			
		
	for(index=0; ;++index)
		{
			p=strtok(NULL,"^"); 
			if(p==NULL)
			{
				break;		
			}
			strcpy(muge[index],p);
			strcat(muge[index],"^");
			strcat(merge,muge[index]);
				
		}


	strtok(tell,"\n");
	Sleep(5);
	hstmt=0;
              
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_data,"insert into MSG_RESULT(GID,DEST_NAME,DEST_ADDR,MERGE,TEMP_FIELD) values(%d,'%s','%s','%s','%s')",gid,name,tell,merge,temp_field); 
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}
	free(name);
	free(temp_field);
	free(tell);
	memset(merge,0,120);
	++count;
} //end while()	
fputs("EOF",fp2);
file_name3 = file_name2;
printf("-------------------------total : [ %d]개 파일처리 완료!!------------------------\n",count);
	fflush(fp);
	fflush(fp2);
	fclose(fp);
	fclose(fp2);
return file_name3;
}


void status(int gid)
{
	UCHAR update_data[UPDATE_SIZE];
	UCHAR select_[160];
	int jobid;
	SDWORD jobid_len=SQL_NTS;
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=32 WHERE gid=%d",gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}

	hstmt = 0;
	SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)select_,"select distinct jobid from msg_result where gid=%d",gid);
	SQLPrepare(hstmt, select_, SQL_NTS);
	SQLBindCol(hstmt, 1, SQL_C_LONG,(PTR) &jobid,(SDWORD)sizeof(jobid), &jobid_len);
	_ret = SQLExecute(hstmt);
	SQLFetch(hstmt);
	_ret =SQLFreeStmt(hstmt, SQL_DROP) ;
	}
	LeaveCriticalSection(&cs_db);
	fn_x_statusd( jobid, &ret);

	return ;
}

int	lpfn_statusd( int ret_code, char *session_id, int msg_id, int job_id )
{

	EnterCriticalSection(&cs_db);
	UCHAR update_data[UPDATE_SIZE];
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)update_data,"UPDATE MSG SET JOB_STATUS=33 WHERE gid=%d",gid);
	printf("update_data=%s\n",update_data);
	_ret=SQLPrepare(hstmt,update_data,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
	printf("[STATUSD_CB] : ret_code | %d, msg_id | %d, job_id | %d \n", ret_code, msg_id, job_id );
	LeaveCriticalSection(&cs_db);
	return 1;
}