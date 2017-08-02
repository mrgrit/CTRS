// EZ_lib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#include <wininet.h>

#include <sql.h>
#include <sqlext.h>

//#include "shReg.h"


#define SHLM HKEY_LOCAL_MACHEINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

#define EZCALL_API extern "C" __declspec(dllexport)

//compile option
//#define IPCHECK
//#define USEQUEUE
//#define SERVER164--
//#define FULL_REPORT

//file path define
#if defined SERVER164
#define ATTACHFILE "e:\\sendfax\\%s"
#else
#define ATTACHFILE "d:\\sendfile\\%s"
#endif

#define FTPUSER "xroshotuser"
#define FTPPASS "xroshotuser1234"

#define S164 "211.224.130.164"
#define S165 "211.224.130.165"

#if defined SERVER164
#define DSN "goodshot"
#define DB_ID "goodshot"
#define DB_PASS "daenyx"
#else
#define DSN "goodshot"
#define DB_ID "karma"
#define DB_PASS "karmahaja8282"
#endif
/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;

//FTP 관련 핸들
HINTERNET hInternet, hFtp;
#if defined SERVER164
char g_Server_Userfile[50]=S164;
char g_User[20]=FTPUSER;
char g_Pass[20]=FTPPASS;
#else
char g_Server_Userfile[50]=S165;
char g_User[20]=FTPUSER;
char g_Pass[20]=FTPPASS;
#endif



//전역변수
char now_user[20];
char now_userfile[50];
int now_uid=0;
char origin_file[40];
char temp_file[50];
char copy_file[50];
char now_attachfile[50];  
char now_subject[50];
int now_jobtype;
char now_callback_number[15];
int now_gid;
int bind_ok=0;

/*
typedef struct _report_queue
{
	int gid;
	char title[50];
	int meche;
	int suss;
	int fail;
	int fee;
	int talktime;
	int total;
	struct _report_queue *prev;
	struct _report_queue *next;
} report_queue;

report_queue *head, *tail;
int report_queue_count=0;
*/
typedef struct _result_queue
{
	int gid;
	char title[100];
	char name[15];
	char tel[15];
	int meche;
	char start_time[15];
	char end_time[15];
	int jobid;
	int duration;
	int pages;
	int fee;
	int retcode;
	struct _result_queue *result_prev;
	struct _result_queue *result_next;
} result_queue;
result_queue *result_head, *result_tail;
int result_queue_count=0;

BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey,LPCTSTR lpValue,LPCTSTR lpData);
void MakeSQLDsn(HKEY key,char *Dsn, char *Server, char *DB);

void init_result_queue(void);
int result_queue_put(int gid,char *title,char *name,char *tel,int meche_int,char *id_no,char *msg_plus,int jobid,int talktime,int page,int fee,int retcode);
int result_queue_get(void);

char *file_processing(char *);
char *get_now();
char *get_now_second();
char *get_date();
char *get_time();
LPCTSTR GetLocalIP();

/*****FTP FUNCTIONS*****/
BOOL Internet_Open()
{
	hInternet=InternetOpen("EZCALL",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(hInternet == NULL){
		return FALSE;
	}
	return TRUE;
}

BOOL Connect_Userfile()
{
	hFtp=InternetConnect(hInternet,g_Server_Userfile,INTERNET_DEFAULT_FTP_PORT,g_User,g_Pass,INTERNET_SERVICE_FTP,0,0);
	if(hFtp==NULL)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL Upload_Userfile(char *Path, int flag)
{
//	char drive[_MAX_DRIVE];
//	char dir[_MAX_DIR];
	char ext[_MAX_EXT];
	char Remote[MAX_PATH];

	char user_itoa[4];

	itoa(now_uid,user_itoa,10);
	strtok(user_itoa," ");	
	_splitpath(Path,NULL,NULL,Remote,ext);

	sprintf(Remote,"%.5s%s",user_itoa,get_now_second());
	strcat(Remote,ext);
	if(flag==1) //userfile
	{
        sprintf(now_userfile,ATTACHFILE,Remote);
	} else if(flag==2){ //tempfile
		sprintf(temp_file,ATTACHFILE,Remote);
	}else if(flag==3){ //attachfile
		strcpy(now_attachfile,Remote);
	}
	return FtpPutFile(hFtp,Path,Remote,FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,0);
}

void FtpDisconnect()
{
	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
	hFtp=NULL;
	hInternet=NULL;
}

#if defined(IPCHECK)
EZCALL_API int ezCall_bind(char *id, char *pass)
{
	puts("start bind-ipcheck");
	MakeSQLDsn(HKEY_CURRENT_USER,DSN,S165,DB_ID);
	
	henv = hdbc = hstmt = 0;  
	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
	_ret=SQLConnect(hdbc,(UCHAR *)DSN,SQL_NTS,
		 (UCHAR *)DB_ID,SQL_NTS,(UCHAR *)DB_PASS,SQL_NTS);
	printf("connect=%d\n",_ret);

	UCHAR use[30];
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)use,"use goodshot");
	_ret = SQLPrepare(hstmt,use,SQL_NTS);
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}

	UCHAR select[65];
	char password[20]="";
	SDWORD password_len=SQL_NTS;
	char allow_ip[16]="";
	SDWORD allow_ip_len=SQL_NTS;

	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
	{
		sprintf((char *)select,"select password,id_no from member where userid='%s'",id);
		SQLPrepare(hstmt,select,SQL_NTS);
		SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) password,
				  (SDWORD)sizeof(password),&password_len);
		SQLBindCol(hstmt,2,SQL_C_CHAR,(PTR) allow_ip,
				  (SDWORD)sizeof(allow_ip),&allow_ip_len);
	}
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	strcpy(now_user,id);
	strtok(allow_ip," ");
	LPCTSTR ip="";
	ip=GetLocalIP();
	if((strncmp(pass,password,strlen(pass))==0) && (strncmp(ip,allow_ip,strlen(allow_ip))==0)){
		bind_ok=1;
		return 0;   
	}else { 
		fprintf(stderr,"Login Error or not allowed IP\n"); 
		bind_ok=-1;
		exit(-1);
	}
}
#else
//non-ipcheck
EZCALL_API int ezCall_bind(char *id, char *pass)
{
	puts("start bind - no ipcheck");
#if defined SERVER164 
	MakeSQLDsn(HKEY_CURRENT_USER,DSN,S164,DB_ID);
		henv = hdbc = hstmt = 0;  
	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
	_ret=SQLConnect(hdbc,(UCHAR *)DSN,SQL_NTS,
		 (UCHAR *)DB_ID,SQL_NTS,(UCHAR *)DB_PASS,SQL_NTS);
#else
	MakeSQLDsn(HKEY_CURRENT_USER,DSN,S165,DB_ID);
		henv = hdbc = hstmt = 0;  
	// MS SQL 에 접속을 시도 한다
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv,&hdbc);
	_ret=SQLConnect(hdbc,(UCHAR *)DSN,SQL_NTS,
		 (UCHAR *)DB_ID,SQL_NTS,(UCHAR *)DB_PASS,SQL_NTS);
#endif
	printf("connect=%d\n",_ret);

	UCHAR use[30];
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)use,"use goodshot");
	_ret = SQLPrepare(hstmt,use,SQL_NTS);
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}

	
	UCHAR select[55];
	char password[20]="";
	SDWORD password_len=SQL_NTS;
	int uid=0;
	SDWORD uid_len=SQL_NTS;

	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
	{
		sprintf((char *)select,"select password,uid from member where userid='%s' and status>0",id);
		SQLPrepare(hstmt,select,SQL_NTS);
		SQLBindCol(hstmt,1,SQL_C_CHAR,(PTR) password,(SDWORD)sizeof(password),&password_len);
		SQLBindCol(hstmt,2,SQL_C_LONG,(PTR) &uid,(SDWORD)sizeof(uid), &uid_len);
	}
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	if(_ret==100) 
	{
		fprintf(stderr,"confirm userid again\n");
		exit(-1);
	}
	printf("pass_len=%d\n",strlen(pass));
	printf("password_len=%d\n",strlen(password));
	printf("uid=%d\n",uid);
//	if(strncmp(pass,password,strlen(password))==0){
	if(strcmp(pass,password)==0){
		bind_ok=1;
		strcpy(now_user,id);
		now_uid=uid;
		return 0;
	}else{
		fprintf(stderr,"Incorrect Password!!\n");
		exit(-1);
	}
}
#endif


EZCALL_API int ezCall_pre_submit(	int		jobtype,
									char	*subject,
							//		char	*schedule_stime,
							//		char	*schedule_dtime,
									char	*callback_number,
								//	int		dest_type,
								//	int		dest_file_count,
									char	*dest_file,
								//	int		dest_addr_count,
							//		char	*dest_addr,
							//		char	*sms_msg,
							//		char	*tts_content,
							//		int		attach_file_count,
									char	*attach_file)
								//	char	*temp_file,  /* Server에 존재하는 파일 이름 */
								//	int		reply_type,	
								//	int		reply_count,
								//	char	*counselor_number,
								//	char	*header_msg,	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
								//	char	*tail_msg,		/* 맺음말 메시지, 파일만 사용 가능 */
								//	int		re_listen,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
								//	char    *kt_office,		/* 사원 번호 */
								//	int		*ret_code)
								
{
	if(bind_ok != 1) 
	{
		fprintf(stderr,"You must run bind() first!\n");
		exit(-1);
	}
	UCHAR insert_goodyymm2[150];
//	UCHAR insert_message2[300];
	UCHAR select_gid[200];

	int gid;
	SDWORD gid_len=SQL_NTS;

	strcpy(now_subject,subject);
	now_jobtype=jobtype;
	strcpy(now_callback_number,callback_number);
	
	srand(time(0)); //rand()의 seed값을 설정한다.
	int random=rand();
		
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)insert_goodyymm2,"insert into goodyymm2(yymm,userid,title,meche,rettel,fee_p)values(getdate(),'%s','%s',%d,'%s','%d')",now_user,subject,jobtype,callback_number,random);
	_ret = SQLPrepare(hstmt,insert_goodyymm2,SQL_NTS);
	_ret = SQLExecute(hstmt);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	}

	if((jobtype == 5) || (jobtype == 6) || (jobtype == 7) || (jobtype == 8) || (jobtype == 9))
	{
		char temp[40]="";
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char filename[_MAX_FNAME];
		char ext[_MAX_EXT];

		
		strcpy(origin_file,dest_file);
		strtok(origin_file," ");
		
		strtok(dest_file," ");
		strtok(attach_file," ");
		
		strcpy(temp,file_processing(dest_file));
		Internet_Open();
		Connect_Userfile();
		Upload_Userfile(temp,1);
		
		Sleep(1000);
		
		_splitpath(dest_file,drive,dir,filename,ext);
		strncat(filename,"__",2);
		
		sprintf(dest_file,"%s%s%s%s",drive,dir,filename,ext);
		Upload_Userfile(dest_file,2);
		
		Sleep(1000);
		
		int ret=Upload_Userfile(attach_file,3);
		FtpDisconnect();
	/*
		_splitpath(dest_file,drive,dir,filename,ext);
		sprintf(filename,"%s",get_now_second());
		sprintf(copy_file,"%s%s%s%s",drive,dir,filename,ext);
				
		_splitpath(origin_file,drive,dir,filename,ext);
		sprintf(origin_file,"%s%s%s%s",drive,dir,filename,ext);
			
		ret=rename(origin_file,copy_file);
	*/
	}

	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
	{
	sprintf((char *)select_gid,"select max(gid) from goodyymm2 where userid='%s' and title='%s' and meche=%d and fee_p=%d",now_user,subject,jobtype,random);
	SQLPrepare(hstmt,select_gid,SQL_NTS);
	SQLBindCol(hstmt,1, SQL_C_LONG, (PTR) &gid,(SDWORD)sizeof(gid), &gid_len);
	}
	_ret=SQLExecute(hstmt);  
	_ret=SQLFetch(hstmt);
	now_gid=gid;
//	puts("___________________________________");
//	printf("user=%s\n,subject=%s\n,jobtype=%d\n,callback_number=%s\n,gid=%d\n",now_user,now_subject,now_jobtype,now_callback_number,now_gid);
//	puts("___________________________________");
	return gid;
}


EZCALL_API int ezCall_submit(												
							char	*msdate,
							char	*mstime,
				//			char	*callback_number,
						//	int		dest_type,
						//	int		dest_file_count,
						//	char	*dest_file_info,
						//	int		dest_addr_count,
							char	*dest_addr,
							char	*dest_name,
							char	*message)
				//			char	*sms_msg,
				//			char	*tts_content,
				//			int		attach_file_count,
					//		char	*attach_file,
						//	char	*temp_file,  /* Server에 존재하는 파일 이름 */
						//	int		reply_type,	
						//	int		reply_count,
						//	char	*counselor_number,
						//	char	*header_msg,	/* 머릿말 메시지, 템플릿과 파일을 동시에 사용 가능  */ 
						//	char	*tail_msg,		/* 맺음말 메시지, 파일만 사용 가능 */
						//	int		re_listen,		/* 재 청취 횟수로 0회부터 3회까지 설정 가능 */
						//	char    *kt_office,		/* 사원 번호 */
						//	int		*ret_code)
{
	int ret=-1;
	if(bind_ok != 1) 
	{
		fprintf(stderr,"You must run bind() first! \n");
		exit(-1);
	}

	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	UCHAR insert_data[1500];
	if((now_jobtype==5) || (now_jobtype==6)	|| (now_jobtype==7) || (now_jobtype==8) || (now_jobtype==9))
	{
//		char temp_userfile[40];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char filename[_MAX_FNAME];
		char ext[_MAX_EXT];
				
		char delete_file[40];

		sprintf((char *)insert_data,"insert into message2(wdate,gid,name,msg,userid,meche,title,rettel,msdate,mstime,msg_plus,id_no)values(getdate(),%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s','%s')",now_gid,now_userfile,now_attachfile,now_user,now_jobtype,now_subject,now_callback_number,msdate,mstime,temp_file,message);
		puts((char *)insert_data);
		_splitpath(origin_file,drive,dir,filename,ext);
		sprintf(delete_file,"%s%s%s_%s",drive,dir,filename,ext);
		DeleteFile(delete_file);

		_splitpath(origin_file,drive,dir,filename,ext);
		sprintf(delete_file,"%s%s%s__%s",drive,dir,filename,ext);
		DeleteFile(delete_file);

		memset(now_attachfile,0,50);
	}else{
		sprintf((char *)insert_data,"insert into message2(wdate,gid,name,msg,userid,meche,tel,title,rettel,msdate,mstime)values(getdate(),%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s')",now_gid,dest_name,message,now_user,now_jobtype,dest_addr,now_subject,now_callback_number,msdate,mstime);
	}
//	puts((char *)insert_data);
	_ret = SQLPrepare(hstmt,insert_data,SQL_NTS); 
	_ret=SQLExecute(hstmt);
	ret=_ret;
	}
//	printf("submit exe=%d\n",_ret);
	_ret = SQLFreeStmt(hstmt,SQL_DROP);
	return ret;
}
#if defined FULL_REPORT
EZCALL_API int result_callback(int gid,char *title,char *name,char *tel,int meche_int,char *id_no,char *msg_plus,int jobid,int talktime,int page,int fee,int retcode)
{
	HINSTANCE hInst;
	int (*CallBack2)(int,char *,char *,char *,int,char *,char *,int,int,int,int,int);
	hInst=LoadLibrary("sample.exe");
	CallBack2=(int (*)(int,char *,char *,char *,int,char *,char *,int,int,int,int,int))GetProcAddress(hInst,"result_set");
	CallBack2(gid,title,name,tel,meche_int,id_no,msg_plus,jobid,talktime,page,fee,retcode);
	FreeLibrary(hInst);
	
	return 0;
}
#else
EZCALL_API int result_callback(int gid,char *title,char *name,char *tel,int meche_int,char *id_no,char *msg_plus,int jobid,int page,int fee,int retcode)
{
	HINSTANCE hInst;
	int (*CallBack2)(int,char *,char *,char *,int,char *,char *,int,int,int,int);
	hInst=LoadLibrary("sample.exe");
	CallBack2=(int (*)(int,char *,char *,char *,int,char *,char *,int,int,int,int))GetProcAddress(hInst,"result_set");
	CallBack2(gid,title,name,tel,meche_int,id_no,msg_plus,jobid,page,fee,retcode);
	FreeLibrary(hInst);
	
	return 0;
}
#endif
/*
extern "C" __declspec(dllexport) int road(void)
{
	HINSTANCE hInst;
	int (*loaded)(int,int);
	hInst=LoadLibrary("sampel.exe");
	loaded=(int (*)(int,int))GetProcAddress(hInst,"print");
	loaded(3,5);
	return 0;
}
*/
#if defined(USEQUEUE)
EZCALL_API int ezCall_result(int gid)
{
	init_result_queue();
	char title[100]="";
	SDWORD title_len=SQL_NTS;
	int jobid=0;
	SDWORD jobid_len=SQL_NTS;
	char meche[2]="";
	SDWORD meche_len=SQL_NTS;
	char name[20]="";
	SDWORD name_len=SQL_NTS;
	char tel[15]="";
	SDWORD tel_len=SQL_NTS;
	int retcode=-1;
	SDWORD retcode_len=SQL_NTS;
	char id_no[15]="";
	SDWORD id_no_len=SQL_NTS;
	char msg_plus[15]="";
	SDWORD msg_plus_len=SQL_NTS;
	int talktime=0;
	SDWORD talktime_len=SQL_NTS;
	int page=0;
	SDWORD page_len=SQL_NTS;
	int fee=0;
	SDWORD fee_len=SQL_NTS;
	char wdate[30]="";
	SDWORD wdate_len=SQL_NTS;
	
	int data_count;
	int meche_int=0;

	UCHAR select_result[1500];
	hstmt1=0;
	SQLAllocStmt(hdbc,&hstmt1);
	{
	sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate from message_bak2 where gid=%d and userid=%s",gid,now_user);
	SQLPrepare(hstmt1,select_result,SQL_NTS);
	SQLBindCol(hstmt1,1,SQL_C_CHAR,(PTR) title,(SDWORD)sizeof(title),&title_len);
	SQLBindCol(hstmt1,2,SQL_C_LONG,(PTR) &jobid,(SDWORD)sizeof(jobid), &jobid_len);
	SQLBindCol(hstmt1,3,SQL_C_CHAR,(PTR) meche,(SDWORD)sizeof(meche),&meche_len);
	SQLBindCol(hstmt1,4,SQL_C_CHAR,(PTR) name,(SDWORD)sizeof(name),&name_len);
	SQLBindCol(hstmt1,5,SQL_C_CHAR,(PTR) tel,(SDWORD)sizeof(tel),&tel_len);
	SQLBindCol(hstmt1,6,SQL_C_LONG,(PTR) &retcode,(SDWORD)sizeof(retcode), &retcode_len);
	SQLBindCol(hstmt1,7,SQL_C_CHAR,(PTR) id_no,(SDWORD)sizeof(id_no),&id_no_len);
	SQLBindCol(hstmt1,8,SQL_C_CHAR,(PTR) msg_plus,(SDWORD)sizeof(msg_plus),&msg_plus_len);
	SQLBindCol(hstmt1,9,SQL_C_LONG,(PTR) &talktime,(SDWORD)sizeof(talktime), &talktime_len);
	SQLBindCol(hstmt1,10,SQL_C_LONG,(PTR) &page,(SDWORD)sizeof(page), &page_len);
	SQLBindCol(hstmt1,11,SQL_C_LONG,(PTR) &fee,(SDWORD)sizeof(fee), &fee_len);
	SQLBindCol(hstmt1,12,SQL_C_CHAR,(PTR) wdate,(SDWORD)sizeof(wdate),&wdate_len);
	}
	_ret=SQLExecute(hstmt1);

	while(_ret=SQLFetch(hstmt1) != SQL_NO_DATA)
	{
		meche_int=atoi(meche);
		data_count=result_queue_put(gid,title,name,tel,meche_int,id_no,msg_plus,jobid,talktime,page,fee,retcode);
		if(data_count==-1) 
		{
			puts("[[[[[[[[  !! !! E M E R G E N C Y   M E M O R Y   C R A S H E D !! !!  ]]]]]]]]]");
			getchar();
			exit(-1);
		}
	}
	printf("[%10d] 개의 데이터를 가져 옵니다.",data_count);
	while(result_queue_count != 0) result_queue_get();
	
	return data_count;
}
#else
EZCALL_API int ezCall_result(int gid=0)
{

//	printf("gidgid=%d\n",gid);
	if(bind_ok != 1) 
	{
		fprintf(stderr,"You must run bind() first! \n");
		exit(-1);
	}

	char title[100]="";
	SDWORD title_len=SQL_NTS;
	int jobid=0;
	SDWORD jobid_len=SQL_NTS;
	char meche[2]="";
	SDWORD meche_len=SQL_NTS;
	char name[20]="";
	SDWORD name_len=SQL_NTS;
	char tel[15]="";
	SDWORD tel_len=SQL_NTS;
	int retcode=-1;
	SDWORD retcode_len=SQL_NTS;
	char id_no[15]="";
	SDWORD id_no_len=SQL_NTS;
	char msg_plus[15]="";
	SDWORD msg_plus_len=SQL_NTS;
	int talktime=0;
	SDWORD talktime_len=SQL_NTS;
	int page=0;
	SDWORD page_len=SQL_NTS;
	int fee=0;
	SDWORD fee_len=SQL_NTS;
	char wdate[30]="";
	SDWORD wdate_len=SQL_NTS;
	
	int groupid=0;
	SDWORD groupid_len=SQL_NTS;

	int data_count=0;
	int meche_int=0;

	UCHAR select_result[1500];
	
	sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' ",gid,now_user);
	hstmt1=0;
	SQLAllocStmt(hdbc,&hstmt1);
	SQLPrepare(hstmt1,select_result,SQL_NTS);
	SQLBindCol(hstmt1,1,SQL_C_CHAR,(PTR) title,(SDWORD)sizeof(title),&title_len);
	SQLBindCol(hstmt1,2,SQL_C_LONG,(PTR) &jobid,(SDWORD)sizeof(jobid), &jobid_len);
	SQLBindCol(hstmt1,3,SQL_C_CHAR,(PTR) meche,(SDWORD)sizeof(meche),&meche_len);
	SQLBindCol(hstmt1,4,SQL_C_CHAR,(PTR) name,(SDWORD)sizeof(name),&name_len);
	SQLBindCol(hstmt1,5,SQL_C_CHAR,(PTR) tel,(SDWORD)sizeof(tel),&tel_len);
	SQLBindCol(hstmt1,6,SQL_C_LONG,(PTR) &retcode,(SDWORD)sizeof(retcode), &retcode_len);
	SQLBindCol(hstmt1,7,SQL_C_CHAR,(PTR) id_no,(SDWORD)sizeof(id_no),&id_no_len);
	SQLBindCol(hstmt1,8,SQL_C_CHAR,(PTR) msg_plus,(SDWORD)sizeof(msg_plus),&msg_plus_len);
	SQLBindCol(hstmt1,9,SQL_C_LONG,(PTR) &talktime,(SDWORD)sizeof(talktime), &talktime_len);
	SQLBindCol(hstmt1,10,SQL_C_LONG,(PTR) &page,(SDWORD)sizeof(page), &page_len);
	SQLBindCol(hstmt1,11,SQL_C_LONG,(PTR) &fee,(SDWORD)sizeof(fee), &fee_len);
	SQLBindCol(hstmt1,12,SQL_C_CHAR,(PTR) wdate,(SDWORD)sizeof(wdate),&wdate_len);
	SQLBindCol(hstmt1,13,SQL_C_LONG,(PTR) &groupid,(SDWORD)sizeof(groupid), &groupid_len);
	
	_ret=SQLExecute(hstmt1);

	while(SQLFetch(hstmt1) != SQL_NO_DATA)
	{
		meche_int=atoi(meche);
	#if defined FULL_REPORT
		result_callback(groupid,title,name,tel,meche_int,id_no,msg_plus,jobid,talktime,page,fee,retcode);
	#else
		result_callback(groupid,title,name,tel,meche_int,id_no,msg_plus,jobid,page,fee,retcode);
	#endif
		data_count++;
	}
	return data_count;
}
#endif
int main()
{
	return 0;
}

EZCALL_API int ezCall_report(char *start_date="NULL", char *end_date="NULL", int report_type=0, int gid=0)
{
	if(bind_ok != 1) 
	{
		fprintf(stderr,"You must run bind() first! \n");
		exit(-1);
	}

	char yymm[30]="";
	SDWORD yymm_len=SQL_NTS;
	char title[100]="";
	SDWORD title_len=SQL_NTS;
	char meche[2]="";
	SDWORD meche_len=SQL_NTS;
	int suss=0;
	SDWORD suss_len=SQL_NTS;
	int fail=-1;
	SDWORD fail_len=SQL_NTS;
	int fee=0;
	SDWORD fee_len=SQL_NTS;
	int suss_p=0;
	SDWORD suss_p_len=SQL_NTS;
	int fail_p=-1;
	SDWORD fail_p_len=SQL_NTS;
	int total=0;
	SDWORD total_len=SQL_NTS;
	int pkey=-1;
	SDWORD pkey_len=SQL_NTS;
	int meche_int=0;

	int groupid=0;
	SDWORD groupid_len=SQL_NTS;

	int datacount=0;

	UCHAR select_report[1500];

	if((gid==0) && (strncmp(start_date,end_date,8) > 0) || (strlen(start_date) !=8) || (strlen(end_date) !=8)) 
	{
		fprintf(stderr,"-->incorrect parameter - start_date,end_date at the function ezCall_report\n");
		return -1;
	}
	
	if(report_type==0) //start_date,end_date로 하루 모든 메시지를 검색
	{
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s'",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s 00:00:00.000' and yymm<='%s 23:59:59.000' ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s' and yymm<='%s'", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==1){//start_date,end_date로 하루 문자메시지 검색(yymm)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=1 or meche=2)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s 00:00:00.000' and yymm<='%s 23:59:59.000' and (meche=1 or meche=2)", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s' and yymm<='%s' and (meche=1 or meche=2)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==2){//start_date,end_date로 하루 음성메시지 검색(yymm)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=3 or meche=4)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s 00:00:00.000' and yymm<='%s 23:59:59.000' and (meche=3 or meche=4)", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s' and yymm<='%s' and (meche=3 or meche=4)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==3){//start_date,end_date로 하루 음성첨부메시지 검색(yymm)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=5 or meche=6)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s 00:00:00.000' and yymm<='%s 23:59:59.000' and (meche=5 or meche=6) ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s' and yymm<='%s' and (meche=5 or meche=6)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==4){//start_date,end_date로 하루 팩스메시지 검색(yymm)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=7 or meche=8)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s 00:00:00.000' and yymm<='%s 23:59:59.000' and (meche=7 or meche=8) ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and yymm>='%s' and yymm<='%s' and (meche=7 or meche=8)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==10){//start_date,end_date로 하루 전체 메시지 검색(redate)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=1 or meche=2)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s 00:00:00.000' and redate<='%s 23:59:59.000' and (meche=1 or meche=2)", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s' and redate<='%s' and (meche=1 or meche=2)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==11){//start_date,end_date로 하루 문자 메시지 검색(redate)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=1 or meche=2)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s 00:00:00.000' and redate<='%s 23:59:59.000' and (meche=1 or meche=2)", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s' and redate<='%s' and (meche=1 or meche=2)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==12){//start_date,end_date로 하루 음성 메시지 검색(redate)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=3 or meche=4)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s 00:00:00.000' and redate<='%s 23:59:59.000' and (meche=3 or meche=4) ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s' and redate<='%s' and (meche=3 or meche=4)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==13){//start_date,end_date로 하루 음성첨부 메시지 검색(redate)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=5 or meche=6)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s 00:00:00.000' and redate<='%s 23:59:59.000' and (meche=5 or meche=6) ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s' and redate<='%s' and (meche=5 or meche=6)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}else if(report_type==14){//start_date,end_date로 하루 팩스 메시지 검색(redate)
		if(((strncmp(start_date,"",1)==0) && (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where gid=%d and userid='%s' and (meche=7 or meche=8)",gid, now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s 00:00:00.000' and redate<='%s 23:59:59.000' and (meche=7 or meche=8) ", now_user, start_date,end_date);
		else if ((strncmp(start_date,"",1)!=0) && (strncmp(end_date,"",1)!=0) && gid==0 )
			sprintf((char *)select_report,"select yymm,title,meche,suss,fail,fee,suss_p,fail_p,total,pkey,gid from goodyymm2 where userid='%s' and redate>='%s' and redate<='%s' and (meche=7 or meche=8)", now_user, start_date, end_date);
		else{
			fprintf(stderr,"incorrect parameter at the fuction ezCall_report\n");
			return -1;
		}
	}
	

	
	hstmt1=0;
	SQLAllocStmt(hdbc,&hstmt1);
	SQLPrepare(hstmt1,select_report,SQL_NTS);
	SQLBindCol(hstmt1,1,SQL_C_CHAR,(PTR) yymm,(SDWORD)sizeof(yymm),&yymm_len);
	SQLBindCol(hstmt1,2,SQL_C_CHAR,(PTR) title,(SDWORD)sizeof(title),&title_len);
	SQLBindCol(hstmt1,3,SQL_C_CHAR,(PTR) meche,(SDWORD)sizeof(meche),&meche_len);
	SQLBindCol(hstmt1,4,SQL_C_LONG,(PTR) &suss,(SDWORD)sizeof(suss), &suss_len);
	SQLBindCol(hstmt1,5,SQL_C_LONG,(PTR) &fail,(SDWORD)sizeof(fail), &fail_len);
	SQLBindCol(hstmt1,6,SQL_C_LONG,(PTR) &fee,(SDWORD)sizeof(fee), &fee_len);
	SQLBindCol(hstmt1,7,SQL_C_LONG,(PTR) &suss_p,(SDWORD)sizeof(suss_p), &suss_p_len);
	SQLBindCol(hstmt1,8,SQL_C_LONG,(PTR) &fail_p,(SDWORD)sizeof(fail_p), &fail_p_len);
	SQLBindCol(hstmt1,9,SQL_C_LONG,(PTR) &total,(SDWORD)sizeof(total), &total_len);
	SQLBindCol(hstmt1,10,SQL_C_LONG,(PTR) &pkey,(SDWORD)sizeof(pkey), &pkey_len);
	SQLBindCol(hstmt1,11,SQL_C_LONG,(PTR) &groupid,(SDWORD)sizeof(groupid), &groupid_len);
	_ret=SQLExecute(hstmt1);

	while(SQLFetch(hstmt1) != SQL_NO_DATA)
	{
		datacount++;
		meche_int=atoi(meche);
		HINSTANCE hInst2;

		int (*CallBack)(int,char *,char *,int,int,int,int,int,int,int,int);
		hInst2=LoadLibrary("sample.exe");
		CallBack=(int (*)(int,char *,char *,int,int,int,int,int,int,int,int))GetProcAddress(hInst2,"report_set");
		CallBack(groupid,yymm,title,meche_int,suss,fail,fee,suss_p,fail_p,total,pkey);
		FreeLibrary(hInst2);
	}
	return datacount;
}



BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey,LPCTSTR lpValue,LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisp)!=ERROR_SUCCESS) return FALSE;
	if(RegSetValueEx(key,lpValue,0,REG_SZ,(LPBYTE)lpData,strlen(lpData)+1)!=ERROR_SUCCESS) return FALSE;
	RegCloseKey(key);
	return TRUE;
}


void MakeSQLDsn(HKEY key,char *Dsn, char *Server, char *DB)
{
	char szDriver[MAX_PATH];
	char szRegPath[MAX_PATH];

	GetSystemDirectory(szDriver,MAX_PATH);
	strcat(szDriver,"\\sqlsvr32.dll");
	wsprintf(szRegPath,"Software\\Odbc\\Odbc.ini\\%s",Dsn);

	SHRegWriteString(key,"Software\\Odbc\\Odbc.ini\\Odbc Data Sources",Dsn,"SQL Server");
	SHRegWriteString(key,szRegPath,"DataBase",DB);
	SHRegWriteString(key,szRegPath,"Description",DB);
	SHRegWriteString(key,szRegPath,"Driver",szDriver);
	SHRegWriteString(key,szRegPath,"Server",Server);
}


//result queue
void init_result_queue(void)
{
	result_head = (result_queue *)malloc(sizeof(result_queue));
	result_tail = (result_queue *)malloc(sizeof(result_queue));

	result_head->result_prev=result_head;
	result_head->result_next=result_tail;
	result_tail->result_prev=result_head;
	result_tail->result_next=result_tail;
}

int result_queue_put(int gid,char *title,char *name,char *tel,int meche_int,char *id_no,char *msg_plus,int jobid,int talktime,int page,int fee,int retcode)
{
	result_queue *result;
	if((result=(result_queue *)malloc(sizeof(result_queue)))==NULL)
	{
		puts("Out of Memory");
		return -1;
	}
	result->gid=gid;
	strcpy(result->title,title);
	strcpy(result->name,name);
	strtok(result->name," ");
	strcpy(result->tel,tel);
	result->meche=meche_int;
	strcpy(result->start_time,id_no);
	strcpy(result->end_time,msg_plus);
	result->jobid=jobid;
	result->duration=talktime;
	result->fee=fee;
	result->retcode=retcode;
	
	result_tail->result_prev->result_next=result;
	result->result_prev=result_tail->result_prev;
	result_tail->result_prev=result;
	result->result_next=result_tail;
	
	result_queue_count++;
	return result_queue_count;
}

int result_queue_get(void)//duration 제거 안했음 
{
	result_queue *result_get;

	result_get=result_head->result_next;
	if(result_get==result_tail)
	{
		puts("Queue underflow");
		return -1;
	}

	int gid,jobid,meche,duration,pages,fee=0;
	int retcode=-1;
//	char *name,*message,*msdate,*mstime,*tel,*title="";
	char title[100],name[20],tel[15],start_time[15],end_time[15]="";

	gid=result_get->gid;
	strcpy(title,result_get->title);
	strcpy(name,result_get->name);
	strcpy(tel,result_get->tel);
	meche=result_get->meche;
	strcpy(start_time,result_get->start_time);
	strcpy(end_time,result_get->end_time);
	jobid=result_get->jobid;
	duration=result_get->duration;
	pages=result_get->pages;
	fee=result_get->fee;
	retcode=result_get->retcode;

//	printf("[queueget]gid=[%d]\njobid=[%d]\nname=[%s]\nmsg=[%s]\nmsdate=[%s]\nmstime=[%s]\ntel=[%s]\ntitle=[%s]meche=[%d]\n",gid,jobid,name,message,msdate,mstime,tel,title,meche);
#if defined FULL_REPORT
	result_callback(gid,title,name,tel,meche,start_time,end_time,jobid,duration,pages,fee,retcode);
#else
	result_callback(gid,title,name,tel,meche,start_time,end_time,jobid,pages,fee,retcode);
#endif
	result_head->result_next=result_get->result_next;
	result_get->result_next->result_prev=result_head;
	free(result_get);
	result_queue_count--;
	
	return result_queue_count;
}

//system time 관련 함수
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

char *get_now_second()
{
	char *ret;
	static char  str[20];
	SYSTEMTIME	systime;
	GetLocalTime( &systime );
	sprintf( str, "%.2d%.2d%.2d%.2d%.2d",
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond);
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


//local ip return
LPCTSTR GetLocalIP()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  char name[255];
  LPCTSTR ip; 
  PHOSTENT hostinfo;
  wVersionRequested = MAKEWORD( 2, 0 );
  if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
  {
        if( gethostname ( name, sizeof(name)) == 0)
        {
                if((hostinfo = gethostbyname(name)) != NULL)
                {
                  ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
                }
        }      
        WSACleanup( );
  } 
  return ip;

}

char *file_processing(char *path)
{
	
	FILE *fp;
	FILE *fp1;
	FILE *fp2;
	char name[40];
	char tel[20];
	char dest_set[60];
	char dest_set2[80];
	char dest_file[50];
	char dest_file2[50];
	char buff[80]="";
	char *temp;
	char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
	char *ret_pointer="";
	int j_kind;
	int message_count=0;

    strtok(path," ");
	strcpy(dest_file,path);
	strcpy(dest_file2,path);
    _splitpath( dest_file, drive, dir, fname, ext );
    strncat(fname,"_",1);
    sprintf(dest_file,"%s%s%s%s",drive,dir,fname,ext);
    _splitpath( dest_file2, drive, dir, fname, ext );
    strncat(fname,"__",2);
    sprintf(dest_file2,"%s%s%s%s",drive,dir,fname,ext);
   
	if((fp=fopen(path,"r+"))==NULL) //원본파일
	{
		fprintf(stderr,"Userfile 0 open err");  
		getchar();
		exit(-1);
	}
	if((fp1=fopen(dest_file,"w+"))==NULL)//이름^전화번호
	{
		fprintf(stderr,"Userfile 1 open err"); 
		getchar();
		exit(-1);
	}
	if((fp2=fopen(dest_file2,"w+"))==NULL) //이름^전화번호^증번호
	{
		fprintf(stderr,"Userfile 2 open err");
		getchar();
		exit(-1);
	}	
	//유저 파일의 맨 마지막 부분에 \n이 없을때를 대비해서.. 
	//무조건 파일 맨 끝에 \n을 넣어 준다.
	fseek(fp,0,2);
	int size=ftell(fp);
//	printf("size=%d\n",size);
	fputs("\n",fp);
	fseek(fp,0,0);
	while(1)
	{
		j_kind=0; //0:증번호 있음 , 1 : 증번호 없음.

		memset(name,0,40);
		memset(tel,0,20);
		memset(buff,0,80);
		fgets(buff,80,fp);
		if(strncmp(buff,"EOF",3)==0)
		{
			puts("end fo file");
			break;
		}
		if(strlen(buff)==0) break;
		if(strncmp(buff,"\n",1)==0)
		{
//			puts("줄빠짐");
			continue;
		}
		temp=strtok(buff,"^");
		strcpy(name,temp);
		temp=strtok(NULL,"^");
		strcpy(tel,temp);
		puts(tel);
		temp=strtok(NULL,"^");
		if(temp==NULL) 
		{
			j_kind=1;
//			puts("증번호가 없습니다.");
			temp="NOINFO";
		}
		strtok(name," ");
		strtok(tel," ");
		sprintf(dest_set,"%s^%s",name,tel);
	//	printf("j_kind=%d\n",j_kind);
		if(j_kind==1) dest_set[strlen(dest_set)-1]=0;
		sprintf(dest_set2,"%s^%s",dest_set,temp);
		fputs(dest_set,fp1);
		fputs("\n",fp1);
		fputs(dest_set2,fp2);
		if(j_kind==1) fputs("\n",fp2);
		message_count++;
	
	}
		fclose(fp);
		fclose(fp1);
		fclose(fp2);
		printf("message_count=%d\n",message_count);
		ret_pointer=dest_file;
		return ret_pointer;
}


//  SQLNumResultCols ( SQLHSTMT stmt,SQLSMALLINT col);  //result set의 갯수를 col에 넣어 준다.

/*0223에 지운것 (ezCall_result)

	sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' ",gid,now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate >='%s 00:00:00.000' and wdate<='%s 23:59:59.000' and userid='%s'",start_date,end_date,now_user);
		else if ((strcmp(start_date,"")!=0) && (strcmp(end_date,"")!=0) && gid==0)
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate>='%s' and wdate<='%s' and userid='%s'",start_date,end_date,now_user);
		else{
			fprintf(stderr,"incorrect pameter at the fuction ezCall_result\n");
			return -1;
		}
	}else if(result_type==1){
		if(((strncmp(start_date,"",1)==0) || (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' and (meche=1 or meche=2) ",gid,now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate >='%s 00:00:00.000' and wdate<='%s 23:59:59.000' and userid='%s' and (meche=1 or meche=2) ",start_date,end_date,now_user);
		else if ((strcmp(start_date,"")!=0) && (strcmp(end_date,"")!=0) && gid==0)
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate>='%s' and wdate<='%s' and userid='%s' and (meche=1 or meche2) ",start_date,end_date,now_user);
		else{
			fprintf(stderr,"incorrect pameter at the fuction ezCall_result\n");
			return -1;
		}
	}else if(result_type==2){
			if(((strncmp(start_date,"",1)==0) || (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' and (meche=3 or meche=4)",gid,now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate >='%s 00:00:00.000' and wdate<='%s 23:59:59.000' and userid='%s' and (meche=3 or meche=4)",start_date,end_date,now_user);
		else if ((strcmp(start_date,"")!=0) && (strcmp(end_date,"")!=0) && gid==0)
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate>='%s' and wdate<='%s' and userid='%s' and (meche=3 or meche=4)",start_date,end_date,now_user);
		else{
			fprintf(stderr,"incorrect pameter at the fuction ezCall_result\n");
			return -1;
		}
	}else if(result_type==3){
			if(((strncmp(start_date,"",1)==0) || (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' and (meche=5 or meche=6)",gid,now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate >='%s 00:00:00.000' and wdate<='%s 23:59:59.000' and userid='%s' and (meche=5 or meche=6)",start_date,end_date,now_user);
		else if ((strcmp(start_date,"")!=0) && (strcmp(end_date,"")!=0) && gid==0)
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate>='%s' and wdate<='%s' and userid='%s' and (meche=5 or meche=6)",start_date,end_date,now_user);
		else{
			fprintf(stderr,"incorrect pameter at the fuction ezCall_result\n");
			return -1;
		}
	}else if(result_type==4){
			if(((strncmp(start_date,"",1)==0) || (strncmp(end_date,"",1)==0)) && gid>=0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where gid=%d and userid='%s' and (meche=7 or meche=8)",gid,now_user);
		else if ((strncmp(start_date,end_date,8)==0) && gid==0 )
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate >='%s 00:00:00.000' and wdate<='%s 23:59:59.000' and userid='%s' and (meche=7 or meche=8)",start_date,end_date,now_user);
		else if ((strcmp(start_date,"")!=0) && (strcmp(end_date,"")!=0) && gid==0)
			sprintf((char *)select_result,"select title,jobid,meche,name,tel,ret_code,id_no,msg_plus,talktime,page,fee,wdate,gid from message_bak2 where wdate>='%s' and wdate<='%s' and userid='%s' and (meche=7 or meche=8)",start_date,end_date,now_user);
		else{
			fprintf(stderr,"incorrect pameter at the fuction ezCall_result\n");
			return -1;
		}
	}
	*/

