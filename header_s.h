#ifndef __HEADER_S_H_
#define __HEADER_S_H_

/* fn_x_moneystate()'s Returning structure.*/
typedef struct{
    char    maincode[2+1]; /* 02 후불제, 99사업용 ID, */
    char    subcode[2+1]; /* 11 */
    unsigned int    xpro_reg;
    unsigned int    charge_money;
    unsigned int    reserved_money;
    unsigned int    charge_per_sms;
    unsigned int    charge_per_fax;
    unsigned int    cell_stdtime;
    unsigned int    cell_stdcharge;
    unsigned int    phone_stdtime;
    unsigned int    phone_stdcharge;
    char            reg_date[8+1];
    unsigned int    pay_apply_flag;
    unsigned int    free_dayleft;
    char            charge_type[2+1];
} MONEYSTATUSINFO;

char *dev="nuno";

/* fn_x_status()'s Returning structure.*/
typedef struct{
	unsigned int		msgid;
	unsigned int		jobid;
	unsigned int		jobtype;
	char			subject[50+1];
	char			schedule_stime[12+1];
	char			schedule_etime[12+1];
	unsigned int		reserved_count;
	unsigned int		fee;
	unsigned int		content_size;
	unsigned int		success_count;
	unsigned int		fail_count;
} STATUSINFO;



/* fn_x_statusd()'s Returning structure.*/
typedef struct{
	unsigned int		msgid;
	unsigned int		jobid;
	unsigned int		jobtype;
	unsigned int		report_type;
	unsigned int		result_count;
	char			result_content[500+1];
} STATUSDETAIL_INFO;


/* fn_x_userinfo()'s Returning structure.*/
typedef struct {
    char    phone_id[20+1];
    char    user_name[40+1];
    char    user_nick[40+1];
    char    mobile_no[20+1];
} USERINFO;

#endif //__HEADER_S_H_
