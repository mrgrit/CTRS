
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

		 01.29 : 1000����Ʈ �̻��� ��� ������������ ���Ϸ� �����ϴµ� �������̸����� ����
		         upload������ userid_msgid_type_index.Ȯ���� �� �����ϵ��� ������ - By min

		 02.16 : ReportCallback�� replied_file ���� �߰��� (���������� �亯�̳� ���������� �ش�)


		 02.16 : FTP FILE UPLOAD - ACTIVE MODE�� ���ɿ��� PASSIVE MODE ���� �����ϵ��� ����

		 04.08 : Reconnect ���� : �Ϻη�, Hardcording.. : XROSHOT V2.2.2009

		 06.28  : V2.2.2200

			API �߰�

			1) Config���� ���� ���� API
			  * �Լ��� 
				 - WINDOW : XROSHOT_API int fn_x_set_conf_file( char* pull_name)
				 - UNIX  : int fn_x_set_conf_file( char* pull_name )
			  * ����
				 - ũ�μ� ���̺귯������ ����ϴ� xroshot.conf ������ ��ġ�� ������ �� �ִ�.
			  * ���
				 - bind �Լ��� ȣ���ϱ� ���� set_conf_file() �Լ��� ȣ���Ͽ� config ������ ��ġ�� �̸��� �� �����Ѵ�.
			  * ����
				 - fn_x_set_conf_file("c:\\temp\\xroshot.conf") ;
				 - retnum = fn_x_bind( 0, USERID, USERPW, "xroshot.com", 1, lpfn_submit, 1, 
									 lpfn_report, 1, lpfn_cancel, 1, lpfn_status, 1, lpfn_statusd, &ret);

		09.01 : V2.3
			0) KT_OFFICE �߰� 
			1) BIND_V2 �߰�
			2) ReportCallback_v2 �߰�

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
/* �Լ� ���۷����� */
/* �Լ� ȣ��� ������ �Ķ���ͷ� �Ѿ���� 	int *ret_code */
/* ������ ������ ǥ���� */
#define X_RET_FUNC_SUCCESS				0x01 /* ���� */
#define X_RET_FUNC_NOT_BOUND			0x02 /* ���ε� ���� ���� */
#define X_RET_FUNC_DATA_SEND			0x03 /* ���� ���� */
#define X_RET_FUNC_NOT_SUPPORTED		0x04 /* ���� �ȵǴ� �Լ� */
#define X_RET_FUNC_NOT_CONNECT			0x05 /* ���� ���� */
#define X_RET_FUNC_NO_RESPONSE			0x06 /* ���� ���� */
#define X_RET_FUNC_FTP_ERROR			0x07 /* ȭ�� ���� ���� */
#define X_RET_FUNC_EXCEED_DEST_COUNT	0x08	/* �����ڼ� �ʰ� */
#define X_RET_FUNC_EXCEED_FILE_COUNT	0x09	/* ÷������ ���� �ʰ� */
#define X_RET_FUNC_FILE_NOT_EXIST		0x10	/* ÷������ ���� */
#define X_RET_FUNC_ETC_ERROR		0x99 /* ��Ÿ ���� */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/* Response Code :�Լ� ��ü�� ���ϰ� - �Լ� ���� �����  */
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

// SUBMIT �Լ� ���� CODES (SUBMIT ERRORS )
#define	X_SUBMIT_ERROR_NOFILE			0x20		/* ������ ���� */
#define	X_SUBMIT_ERROR_NOTARGET			0x21
#define	X_SUBMIT_ERROR_LENGTH			0x22
#define	X_SUBMIT_ERROR_TIME				0x23
#define	X_SUBMIT_ERROR_CBNUMBER			0x24
#define	X_SUBMIT_ERROR_CBURL			0x25
#define	X_SUBMIT_ERROR_SMS				0x26
#define	X_SUBMIT_ERROR_FILETRANSFER		0x27
#define	X_SUBMIT_ERROR_RE_LISTEN		0x28	/* ��û�� Ƚ�� ���� */
#define	X_SUBMIT_ERROR_MENT_TYPE		0x29	/* ��Ʈ Ÿ�� ���� */
#define	X_SUBMIT_ERROR_HEADER			0x30	/* Header ���� ���� */
#define	X_SUBMIT_ERROR_TAIL				0x31	/* TAIL ���� ���� */
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/* REPORT CODE :�޽��� ���۰�� �����ڵ� */
#define LAST_ERROR_WAITING          0x00    /* 1 */		/* ���޴���� */
#define LAST_ERROR_SENDING          0x01    /* 2 */		/* ȣ������ */
#define LAST_ERROR_SUCCESS          0x02    /* 3 */		/* ���ۼ��� */
#define LAST_ERROR_ETC              0x03    /* 4 */		/* ������(��Ÿ����) */
#define LAST_ERROR_NOANSWER         0x04    /* 5 */		/* ������, �ܸ��� ����, ����, TIME OUT, �޽��� FULL */
#define LAST_ERROR_BUSY             0x05    /* 6 */		/* ��ȭ�� */
#define LAST_ERROR_UNRECHABLE       0x06    /* 7 */		/* ���, ����, ���� ���� */
#define LAST_ERROR_COLLECTION       0x08    /* 8 */		/* �ǰ߼������� */
#define LAST_ERROR_NO_FILE          0x14    /* 20 */		/* ���޸޽��� ���� */
#define LAST_ERROR_LISTEN_DENIED    0x15    /* 21 */		/* �޽���û��ź�(DTMF�� ������ ����) */
#define LAST_ERROR_DENIED           0x16    /* 22 */		/* �޽������Űź� */
#define LAST_ERROR_TTS              0x17    /* 23 */		/* TTS ��ȯ ���� */
#define LAST_ERROR_TTF              0x18    /* 24 */		/* TTF ��ȯ ���� */
#define LAST_ERROR_SMSC             0x19    /* 25 */		/* SMSC(midc) ���� ���� */
#define LAST_ERROR_SCP              0x20    /* 32 */		/* SCP ���� ���� */
#define LAST_ERROR_MINUTE_FULL      0x21    /* 33 */		/* �ð��� ���� ���� �Ǽ� �ʰ� */
#define LAST_ERROR_USER_FULL        0x23    /* 35 */		/* �����ڴ� ���۰��� �Ǽ� �ʰ� */
#define LAST_ERROR_DATA             0x24    /* 36 */		/* �Է� ����Ÿ ���� */
#define LAST_ERROR_DB               0x25    /* 37 */		/* DB �۾� ���� */
#define LAST_ERROR_EXPIRE           0x26    /* 38 */		/* TXQ EXPIRE */
#define LAST_ERROR_ADMIN_CANCEL     0x27    /* 39 */		/* �����ڿ� ���� ���� */
#define LAST_ERROR_RMP              0x28        /* 40 */	/* RMP ���� ���� : ä���� �Ҵ���� ���� ��� */
#define LAST_ERROR_RECV_REJECT      0x29        /* 41 */	/* reject */
#define LAST_ERROR_BASE_CHANNEL_LACK	0x2A    /* 42 */	/* �뱹 ä�� ���� */
#define LAST_ERROR_MIDC_SEND        0x2B    /* 43 */		/* MIDC ���� �Ϸ� �� ����� ���� ���� ���� : ������ */
#define LAST_ERROR_SPAM_MSG         0x2C    /* 44 */		/* SPAM MSG */
#define LAST_ERROR_NON_REG	        0x2D	/* 45 */		/* ��Ͻ���:status_d ��û�� �����ߴٰ� ���� */
//---------------------------------------------------------------------------


/*  Message subtype. */
/* SMS + ���� + ����� ���� ��� */
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
#define JOBTYPE_SMS                 0x01        /* ���� �޽��� */
#define JOBTYPE_VOICE               0x02        /* ���� �޽��� */
#define JOBTYPE_FAX                 0x03        /* �ѽ� �޽��� */
#define JOBTYPE_EMAIL               0x04        /* ���� �޽��� */
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

/* ���� ��Ʈ �÷��� Ÿ�� */
#define MT_DEFAULT				0x00	/* ���� ũ�μ� ����Ʈ �Ӹ��� ��� */
#define MT_HEAD					0x01	/* �Ӹ����κи� ��� */
#define MT_HEAD_TAIL			0x02	/* �Ӹ����� ������ ��� ��� */
#define MT_TAIL					0x03	/* �Ӹ����� ����Ʈ �Ӹ����� ����ϰ� �������� �����ڰ� ���ϴ� ���·� ��� */
#define MT_NOTUSE				0x04	/* �Ӹ��� ��� ���� ������ ������ ��� */

#define FTP_RET_SUCCESS		0		//����
#define FTP_RET_FAIL		1		//����


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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,
	char	*counselor_number,
	char	*header_msg,	/* �Ӹ��� �޽���, ���ø��� ������ ���ÿ� ��� ����  */ 
	char	*tail_msg,		/* ������ �޽���, ���ϸ� ��� ���� */
	int		re_listen,		/* �� û�� Ƚ���� 0ȸ���� 3ȸ���� ���� ���� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,
	char	*counselor_number,
	char	*header_msg,	/* �Ӹ��� �޽���, ���ø��� ������ ���ÿ� ��� ����  */ 
	char	*tail_msg,		/* ������ �޽���, ���ϸ� ��� ���� */
	int		re_listen,		/* �� û�� Ƚ���� 0ȸ���� 3ȸ���� ���� ���� */
	char    *kt_office,		/* ��� ��ȣ */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,
	char	*counselor_number,
	char	*header_msg,	/* �Ӹ��� �޽���, ���ø��� ������ ���ÿ� ��� ����  */ 
	char	*tail_msg,		/* ������ �޽���, ���ϸ� ��� ���� */
	int		re_listen,		/* �� û�� Ƚ���� 0ȸ���� 3ȸ���� ���� ���� */
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
	char	*temp_file,  /* Server�� �����ϴ� ���� �̸� */
	int		reply_type,	
	int		reply_count,
	int		exp_time,   /* Expire Time */
	char	*email_addr, /* Return email address */
	int		ment_play,   /*  0 : No ment, 1 : Ment */
	int		voice_kind,  /*  0 : default ( Select voice to hear ) */
	int		counselor_dtmf,
	char	*counselor_number,
	char	*header_msg,	/* �Ӹ��� �޽���, ���ø��� ������ ���ÿ� ��� ����  */ 
	char	*tail_msg,		/* ������ �޽���, ���ϸ� ��� ���� */
	int		re_listen,		/* �� û�� Ƚ���� 0ȸ���� 3ȸ���� ���� ���� */
	char    *kt_office,		/* ��� ��ȣ */
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
