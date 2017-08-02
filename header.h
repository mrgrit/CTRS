
/*
 The following ifdef block is the standard way of creating macros 
 which make exporting from a DLL simpler. All files within this DLL 
 are compiled with the XROSHOT_EXPORTS symbol defined on the command 
 line. this symbol should not be defined on any project that uses 
 this DLL. This way any other project whose source files include this 
 file see  XROSHOT_API functions as being imported from a DLL, whereas 
 this DLL sees symbols defined with this macro as being exported.
/*********************************************************************
   Initiation Data : 2003. 09. 02
   Programmed by Yoon Hee Sang, ( Inisoft Co. Ltd. )

   Objectives :
      To provide the users with the access to B2B Gateway Server 
	  (aka. Server) and  the interface capabilities 
		to fulfill proposed demands of individual users.

  Update History
   2003. 09. 02.  Program start
                    By Yoon Hee Sang

         09. 07.  Structure and Constants fixed.
		            By Yoon Hee Sang

		 09. 15.  UNIX version complete.
					By Yoon Hee Sang

		 09. 22.  Windows Version complete. 
					By Yoon Hee Sang

		 10. 11.  Debugging Capability Added. (DebugLog function )
					By Yoon Hee Sang

		 10. 14.  Report Callback Changed. 
				  fn_x_status function updated. 
				  ( Covering File Xmission )
				  fn_x_statusd function updated.
				  ( Covering Report Callback )
				    By Yoon Hee Sang

		 10. 17.  fn_x_bind updated.  
				  ( Making var. report_ack to accept 2 as File Xfer) 
		            By Yoon Hee Sang

		 10. 22.  Windows version/ UNIX version Merged and tested.
					By Yoon Hee Sang

		 10. 27 ~ 28  ftp Bug Fix.
					File Name Uniqueness guarantees and 
					Deletes temporary files.

		 01.29 : 1000바이트 이상일 경우 착신자정보를 파일로 저장하는데 동일한이름으로 저장
		         upload파일을 userid_msgid_type_index.확장자 로 생성하도록 수정됨 - By min

		 02.16 : ReportCallback에 replied_file 변수 추가함 (설문조사의 답변이나 녹음파일이 해당)


		 02.16 : FTP FILE UPLOAD - ACTIVE MODE만 가능에서 PASSIVE MODE 선택 가능하도록 적용

		 04.08 : Reconnect 수정 : 일부러, Hardcording.. : XROSHOT V2.2.2009

		 06.28  : V2.2.2200

			API 추가

			1) Config파일 설정 가능 API
			  * 함수명 
				 - WINDOW : XROSHOT_API int fn_x_set_conf_file( char* pull_name)
				 - UNIX  : int fn_x_set_conf_file( char* pull_name )
			  * 설명
				 - 크로샷 라이브러리에서 사용하는 xroshot.conf 파일의 위치를 변경할 수 있다.
			  * 사용
				 - bind 함수를 호출하기 전에 set_conf_file() 함수를 호출하여 config 파일의 위치와 이름을 재 설정한다.
			  * 예제
				 - fn_x_set_conf_file("c:\\temp\\xroshot.conf") ;
				 - retnum = fn_x_bind( 0, USERID, USERPW, "xroshot.com", 1, lpfn_submit, 1, 
									 lpfn_report, 1, lpfn_cancel, 1, lpfn_status, 1, lpfn_statusd, &ret);

		09.01 : V2.3
			0) KT_OFFICE 추가 
			1) BIND_V2 추가
			2) ReportCallback_v2 추가

********************************************************************/

//#ifdef XROSHOT_EXPORTS
#ifndef __HEADER_H__
#define __HEADER_H__
#include "header_s.h"

#if ( defined(WIN32) || defined(_WIN32) )
#define XROSHOT_API __declspec(dllexport)
#endif
//#else
//#define XROSHOT_API __declspec(dllimport)
//#endif

/*
// This class is exported from the Xroshot.dll
class XROSHOT_API CXroshot {
public:
	CXroshot(void);
	// TODO: add your methods here.
};


extern XROSHOT_API int nXroshot;

XROSHOT_API int fnXroshot(void);
*/

/* Dest ttype */
#define	DEST_TEMP_TYPE		0
#define	DEST_FILE_TYPE		1

//---------------------------------------------------------------------------
/* 함수 레퍼런스값 */
/* 함수 호출시 마지막 파라메터로 넘어오는 	int *ret_code */
/* 서버의 정보를 표시함 */
#define X_RET_FUNC_SUCCESS				0x01 /* 성공 */
#define X_RET_FUNC_NOT_BOUND			0x02 /* 바인드 되지 않음 */
#define X_RET_FUNC_DATA_SEND			0x03 /* 전송 에러 */
#define X_RET_FUNC_NOT_SUPPORTED		0x04 /* 지원 안되는 함수 */
#define X_RET_FUNC_NOT_CONNECT			0x05 /* 접속 오류 */
#define X_RET_FUNC_NO_RESPONSE			0x06 /* 응답 없음 */
#define X_RET_FUNC_FTP_ERROR			0x07 /* 화일 전송 오류 */
#define X_RET_FUNC_EXCEED_DEST_COUNT	0x08	/* 착신자수 초과 */
#define X_RET_FUNC_EXCEED_FILE_COUNT	0x09	/* 첨부파일 개수 초과 */
#define X_RET_FUNC_FILE_NOT_EXIST		0x10	/* 첨부파일 없음 */
#define X_RET_FUNC_ETC_ERROR		0x99 /* 기타 오류 */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/* Response Code :함수 자체의 리턴값 - 함수 수행 결과값  */
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

// SUBMIT 함수 리턴 CODES (SUBMIT ERRORS )
#define	X_SUBMIT_ERROR_NOFILE			0x20		/* 파일이 없음 */
#define	X_SUBMIT_ERROR_NOTARGET			0x21
#define	X_SUBMIT_ERROR_LENGTH			0x22
#define	X_SUBMIT_ERROR_TIME				0x23
#define	X_SUBMIT_ERROR_CBNUMBER			0x24
#define	X_SUBMIT_ERROR_CBURL			0x25
#define	X_SUBMIT_ERROR_SMS				0x26
#define	X_SUBMIT_ERROR_FILETRANSFER		0x27
#define	X_SUBMIT_ERROR_RE_LISTEN		0x28	/* 재청취 횟수 실패 */
#define	X_SUBMIT_ERROR_MENT_TYPE		0x29	/* 멘트 타입 오류 */
#define	X_SUBMIT_ERROR_HEADER			0x30	/* Header 파일 오류 */
#define	X_SUBMIT_ERROR_TAIL				0x31	/* TAIL 파일 오류 */
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/* REPORT CODE :메시지 전송결과 응답코드 */
#define LAST_ERROR_WAITING          0x00    /* 1 */		/* 전달대기중 */
#define LAST_ERROR_SENDING          0x01    /* 2 */		/* 호연결중 */
#define LAST_ERROR_SUCCESS          0x02    /* 3 */		/* 전송성공 */
#define LAST_ERROR_ETC              0x03    /* 4 */		/* 미전달(기타에러) */
#define LAST_ERROR_NOANSWER         0x04    /* 5 */		/* 무응답, 단말기 꺼짐, 음영, TIME OUT, 메시지 FULL */
#define LAST_ERROR_BUSY             0x05    /* 6 */		/* 통화중 */
#define LAST_ERROR_UNRECHABLE       0x06    /* 7 */		/* 결번, 비가입, 서비스 정지 */
#define LAST_ERROR_COLLECTION       0x08    /* 8 */		/* 의견수집실패 */
#define LAST_ERROR_NO_FILE          0x14    /* 20 */		/* 전달메시지 없음 */
#define LAST_ERROR_LISTEN_DENIED    0x15    /* 21 */		/* 메시지청취거부(DTMF를 누르지 않음) */
#define LAST_ERROR_DENIED           0x16    /* 22 */		/* 메시지수신거부 */
#define LAST_ERROR_TTS              0x17    /* 23 */		/* TTS 변환 실패 */
#define LAST_ERROR_TTF              0x18    /* 24 */		/* TTF 변환 실패 */
#define LAST_ERROR_SMSC             0x19    /* 25 */		/* SMSC(midc) 연동 실패 */
#define LAST_ERROR_SCP              0x20    /* 32 */		/* SCP 연동 실패 */
#define LAST_ERROR_MINUTE_FULL      0x21    /* 33 */		/* 시간당 전송 가능 건수 초과 */
#define LAST_ERROR_USER_FULL        0x23    /* 35 */		/* 가입자당 전송가능 건수 초과 */
#define LAST_ERROR_DATA             0x24    /* 36 */		/* 입력 데이타 오류 */
#define LAST_ERROR_DB               0x25    /* 37 */		/* DB 작업 오류 */
#define LAST_ERROR_EXPIRE           0x26    /* 38 */		/* TXQ EXPIRE */
#define LAST_ERROR_ADMIN_CANCEL     0x27    /* 39 */		/* 관리자에 의한 삭제 */
#define LAST_ERROR_RMP              0x28        /* 40 */	/* RMP 연동 실패 : 채널을 할당받지 못한 경우 */
#define LAST_ERROR_RECV_REJECT      0x29        /* 41 */	/* reject */
#define LAST_ERROR_BASE_CHANNEL_LACK	0x2A    /* 42 */	/* 대국 채널 부족 */
#define LAST_ERROR_MIDC_SEND        0x2B    /* 43 */		/* MIDC 전송 완료 후 결과를 받지 못한 상태 : 전송중 */
#define LAST_ERROR_SPAM_MSG         0x2C    /* 44 */		/* SPAM MSG */
#define LAST_ERROR_NON_REG	        0x2D	/* 45 */		/* 등록실패:status_d 요청시 실패했다고 가정 */
//---------------------------------------------------------------------------


/*  Message subtype. */
/* SMS + 내용 + 사용자 전송 방식 */
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
/* Added Multistage Type - 2004/04/08 */
#define	VOICE_MULTISTAGE_FILE		0x19
#define	VOICE_MULTISTAGE_TEMP		0x20

/* Added voice upload Type - 2004/04/30 */
#define	VOICE_UPLOAD_FILE			0x44
#define	VOICE_UPLOAD_TEMP			0x45

/* FAX */
#define	FAX_TEXT_FILE		0x21
#define	FAX_TEXT_TEMP		0x22
#define	FAX_TEMP_FILE		0x23
#define	FAX_TEMP_TEMP		0x24

/* Added fax upload Type - 2004/04/30 */
#define	FAX_UPLOAD_FILE		0x46
#define	FAX_UPLOAD_TEMP		0x47

/* VOICE */
#define	SPV_FILE_FILE		0x31 /* SPV = SMS PLUS VOICE  */
#define	SPV_FILE_TEMP		0x32
#define	SPV_IVR_FILE		0x33
#define	SPV_IVR_TEMP		0x34
#define	SPV_TEMP_FILE		0x35
#define	SPV_TEMP_TEMP		0x36
#define	SPV_VXML_FILE		0x37
#define	SPV_VXML_TEMP		0x38

/* Added spv upload Type - 2004/04/30 */
#define	SPV_UPLOAD_FILE		0x48
#define	SPV_UPLOAD_TEMP		0x49


/* VOICE */
#define	CONFERENCE			0x41
#define	COUNSELOR			0x42
#define	MULTISTAGE			0x43


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
#define	X_MSG_REPORT_FILE		0x14

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

#define X_MSG_SUBMIT_V3         0x46  
#define X_MSG_SUBMIT_V4         0x47 

/* 음성 멘트 플레이 타입 */
#define MT_DEFAULT				0x00	/* 기존 크로샷 디폴트 머리말 사용 */
#define MT_HEAD					0x01	/* 머리말부분만 사용 */
#define MT_HEAD_TAIL			0x02	/* 머리말과 맺음말 모두 사용 */
#define MT_TAIL					0x03	/* 머리말은 디폴트 머리말을 사용하고 맺음말만 가입자가 원하는 형태로 사용 */
#define MT_NOTUSE				0x04	/* 머리말 사용 없이 무조건 음성을 재생 */

#define FTP_RET_SUCCESS		0		//실패
#define FTP_RET_FAIL		1		//성공


#ifdef __cplusplus
extern "C" {
#endif 

extern int (*X_SUBMIT_CALLBACK)(int, 
								char*, 
								int, 
								int, 
								int, 
								int);

extern int (*X_REPORT_CALLBACK_V1)(char*, 
								int, 
								int, 
								char*, 
								int, 
								char*, 
								char*, 
								int, 
								int, 
								int, 
								int,
								char* );


extern int (*X_REPORT_CALLBACK_V2)(char*, 
								int, 
								int, 
								char*, 
								int, 
								char*, 
								char*, 
								int, 
								int, 
								int, 
								int,
								char*,
								int);


extern int (*X_CANCEL_CALLBACK)(int, 
								char* );

extern int (*X_STATUS_CALLBACK)(int, 
								char*, 
								int, 
								int);

extern int (*X_STATUSD_CALLBACK)(int, 
								 char*, 
								 int, 
								 int);


   

/*-----------------------------------------------------*/
/* Client library only structures*/

/**

typedef struct {
	DWORD_32		jobtype;
	DWORD_32		subjobtype;
	BYTE			subject[50+1];
	DWORD_32		schedule_type;
	BYTE			schedule_stime[12+1];
	BYTE			schedule_dtime[12+1];
	BYTE			callback_number[30];
	BYTE			callback_url[120];
	DWORD_32		dest_type;
	DWORD_32		dest_file_count;
	BYTE			dest_file_info[30+1];
	BYTE			*dest_addr_count;
	BYTE			*dest_addr;
	BYTE			*sms_msg;
	BYTE			*tts_content;
	BYTE			*template_content;
	DWORD_32		attach_file_count;
	BYTE			*attach_file;
	DWORD_32		reply_type;
	DWORD_32		reply_count;
} MSGINFO;




typedef struct{
    RES_BIZ_BIND_INFO    bindinfo_res;
    RES_BIZ_BIND        bind_res; 
    RES_BIZ_LINK_STATUS  linkstatus_res;
    RES_BIZ_MONEY_STATUS  moneystate_res;
    RES_BIZ_SUBMIT      submit_res;
    REQ_BIZ_REPORT      report_req;
    RES_BIZ_STATUS      status_res;
    RES_BIZ_STATUS_DETAIL     statusd_res;
    RES_BIZ_CANCEL   canceldel_res;
    RES_BIZ_USER_ADD     useradd_res;
    RES_BIZ_USER_DEL     userdel_res;
    RES_BIZ_USER_CHANGE  userchange_res;
    RES_BIZ_USER_INFO    userinfo_res;
    RES_BIZ_USER_CHANGE_RAW  userchange_raw_res;
    RES_BIZ_USER_ADD_2    useradd2_res;
    RES_BIZ_UNBIND      unbind_res;
} LIB_RESULT;
*/


#if ( defined(WIN32) || defined(_WIN32) )

XROSHOT_API int fn_x_set_conf_file( char* pull_name) ;

XROSHOT_API int fn_x_bind( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	int		lpfn_submit(int, char *, int, int, int, int),
	int		report_ack,
	int		lpfn_report(char *, int, int, char *, int, char *, char*, int, int, int, int, char*),
	int		cancel_ack,
	int		lpfn_cancel(int, char *),
	int		status_ack,
	int		lpfn_status(int, char *, int, int),
	int		statusd_ack,
	int		lpfn_statusd(int, char *, int, int),
	int		*ret);


XROSHOT_API int fn_x_bind_v2( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	int		lpfn_submit(int, char *, int, int, int, int),
	int		report_ack,
	int		lpfn_report(char *, int, int, char *, int, char *, char*, int, int, int, int, char*, int),
	int		cancel_ack,
	int		lpfn_cancel(int, char *),
	int		status_ack,
	int		lpfn_status(int, char *, int, int),
	int		statusd_ack,
	int		lpfn_statusd(int, char *, int, int),
	int		*ret);


XROSHOT_API int	fn_x_unbind( 
	char *bind_id, 
	char *bind_passwd );



XROSHOT_API int fn_x_cancel( 
	int job_id, 
	int subjob_id, 
	int *ret_code );


XROSHOT_API MONEYSTATUSINFO fn_x_moneystatus( 
	char *net_code, 
	char *net_subcode, 
	int *ret_code );


XROSHOT_API STATUSINFO fn_x_status(  
	char	*stime,
	int		period,
	int		*ret_code);


XROSHOT_API STATUSDETAIL_INFO fn_x_statusd( 
	int job_id, 
	int *ret_code );



XROSHOT_API int fn_x_submit( 
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
	int		*ret_code);



XROSHOT_API int fn_x_submit_2( 
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
	int		*ret_code);


XROSHOT_API int fn_x_submit_3 (
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
	int		*ret_code);


XROSHOT_API int fn_x_submit_4 (
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



XROSHOT_API int fn_x_useradd(
	char	*userid,
	char	*username,
	char	*password,
	char	*mobile_no,
	char	*mobile_pw,
	char	*reg_code,
	int		*ret_code );



XROSHOT_API int fn_x_useradd_2( 
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


XROSHOT_API int fn_x_userchange( 
	char	*username,
	char	*mail_password,
	char	*ivr_password,
	char	*mobile_no,
	char	*reg_code,
	int		*ret_code );


XROSHOT_API int fn_x_userchangeraw( 
	char	*query_string,
	int		*ret_code );


XROSHOT_API int fn_x_userdel(
	int		*ret_code );


XROSHOT_API USERINFO fn_x_userinfo( int *ret_code );

XROSHOT_API int fn_x_upload(char* file_name, int unique_num, char* upload_file_name) ;

#else

int fn_x_set_conf_file( char* pull_name ) ; 


int fn_x_bind( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	int		lpfn_submit(int, char *, int, int, int, int),
	int		report_ack,
	int		lpfn_report(char *, int, int, char *, int, char *, char*, int, int, int, int, char*),
	int		cancel_ack,
	int		lpfn_cancel(int, char *),
	int		status_ack,
	int		lpfn_status(int, char *, int, int),
	int		statusd_ack,
	int		lpfn_statusd(int, char *, int, int),
	int		*ret);


int fn_x_bind_V2( 
	int		user_type,
	char	*bind_name,
	char	*bind_passwd,
	char	*bind_domain,
	int		submit_ack,
	int		lpfn_submit(int, char *, int, int, int, int),
	int		report_ack,
	int		lpfn_report(char *, int, int, char *, int, char *, char*, int, int, int, int, char*, int),
	int		cancel_ack,
	int		lpfn_cancel(int, char *),
	int		status_ack,
	int		lpfn_status(int, char *, int, int),
	int		statusd_ack,
	int		lpfn_statusd(int, char *, int, int),
	int		*ret);

int	fn_x_unbind( char *bind_id, 
	char *bind_passwd );



int fn_x_cancel( int job_id, 
	int subjob_id, 
	int *ret_code );


MONEYSTATUSINFO fn_x_moneystatus( char *net_code, 
	char *net_subcode, 
	int *ret_code );


STATUSINFO fn_x_status(  
	char	*stime,
	int		period,
	int		*ret_code);


STATUSDETAIL_INFO fn_x_statusd( 
	int job_id, 
	int *ret_code );



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
	int		*ret_code);



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
	int		counselor_dtmf,
	char	*counselor_number,
	int		*ret_code);


int fn_x_submit_3( 
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
	int		*ret_code);


int fn_x_submit_4( 
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
	char	*mobile_pw,
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

int fn_x_upload(char* file_name, int unique_num, char* upload_file_name) ;


#endif



#ifdef __cplusplus
}
#endif



#endif
