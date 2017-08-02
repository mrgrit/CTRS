#include <stdio.h>  
#include <stdlib.h>
#include <windows.h>
#include "ezcall.h"

char *get_now();  
char *get_date();
char *get_time();
  
//global variable   
char	in_str[80];  

  
//sample function
void help_basic();
void help_bind(); 
void help_submit();
void help_submit1();  
void help_submit2();
void help_submit3();  
void help_submit4(); 
void help_submit5();
void help_submit6();    
void help_submit7();
void help_submit8();
void help_report();
void help_result();

int result_set(int gid,char *subject,char *dest_name,char *dest_addr,int jobtype,char *start_time,char *end_time,int jobid,int duration,int pages,int fee,int retcode)
{
	
	printf("[RESULT CALLBACK] : \n"
			"\t gid\t: [%d] \n"
			"\t subject\t: [%s] \n"
			"\t job id | job type\t:[%d | %d] \n"
			"\t dest_name | dest_addr\t:[%s | %s] \n"
			"\t ret_code\t:[%d] \n"
			"\t start time | end time\t:[%s|%s] \n"
			"\t duration, pages\t:[%d:%d] \n"
			"\t fee\t:[%d] \n",
			gid,
			subject,
			jobid,
			jobtype,
			dest_name,
			dest_addr,
			retcode,
			start_time,
			end_time,
			duration,
			pages,
			fee
			);
	return 0;
}

int report_set(int gid,char *entry_time,char *subject,int jobtype,int success,int fail,int fee,int entry_success,int entry_fail,int total,int result_status)
{
	printf("[REPORT CALLBACK] : ___________________________________________\n"
			"\t gid\t: [%d] \n"
			"\t entry_time\t: [%s] \n"
			"\t subject\t: [%s] \n"
			"\t jobtype\t: [%d] \n"
			"\t sucess\t: [%d] \n"
			"\t fail\t: [%d] \n"
			"\t fee\t: [%d] \n"
			"\t entry_success\t: [%d] \n"
			"\t entry_fail\t: [%d] \n"
			"\t total\t: [%d] \n"
			"\t result_status\t: [%d] \n",
			gid,
			entry_time,
			subject,
			jobtype,
			success,
			fail,
			fee,
			entry_success,
			entry_fail,
			total,
			result_status
			);
	
	return 0;
}



main()
{

/*	int _ret=10;
	int gid=0;  
	int data_count=0;
//	ezCall_report(45); 
	ezCall_bind("aaa","aaa");
   
	gid=ezCall_pre_submit(3,"음성 테스트","0168083495","","");
//	puts("call submit");
	printf("gid=%d\n",gid);
puts("submit");
RESEND : 
ezCall_submit(get_date(),get_time(),"0168083495","전두용","모듈모듈테스틍");
//_ret=ezCall_report("NULL","NULL",120);   
//_ret=ezCall_report("20050221","20050221",0);   
//printf("ret=%d\n",_ret);
/*	for(int i=0;i==5;i++)  
	{
	printf("ret code=%d\n",_ret);
	}*/  
//	data_count=ezCall_result(75);

//	ezCall_report(45);

//	print_result();
//	printf("gid=[%d]\njobid=[%d]\nname=[%s]\nmsg=[%s]\nmsdate=[%s]\nmstime=[%s]\ntel=[%s]\ntitle=[%s]meche=[%d]\n",gid,jobid,name,message,msdate,mstime,tel,title,meche);
//	result_queue_get();
//	printf("datacount=%d\n",data_count);
//	puts("submit ok");
//	exit(0);
	/*
int ret;
	int jobtype;
	char subject[100];
	char callback_number[15];
	char dest_name[15];
	char dest_addr[15];
	char schedule_start[15];
	char schedult_end[15];
	char message[1002];

	gid=ezCall_pre_submit(3,subject,callback_number);
	ezCall_submit(get_date(),get_time(),dest_addr,dest_name,message,&_ret);
	printf("_ret=%d\n",_ret);
	*/  
//	puts("aaa");  
//	getchar();
//	goto RESEND;
	help_basic();
//	ezCall_result(211);
//	ezCall_bind("aaa","aaa");
	return 0; 
}

void help_basic()
{
	printf(" \n\n ");
	printf("select some job \n");
	printf(" 1 : bind \n");
	printf(" 2 : send message \n");
	printf(" 3 : request report \n");
	printf(" 4 : request result \n");
	printf(" q : quit \n");

	gets(in_str);

	if ( in_str[0] == '1' ) help_bind();
	else if ( in_str[0] == '2' ) help_submit();
	else if ( in_str[0] == '3' ) help_report();
	else if ( in_str[0] == '4' ) help_result();
	else if ( in_str[0] == 'q' ) exit(0);
	else  help_basic();  

#if ( defined(WIN32) || defined(_WIN32) )
	Sleep(1000);
#else
	sleep(1);      
#endif 
}

void help_bind()
{
	char id[30];
	char password[30];
	printf(" Enter your bind id : ");
	gets(id);
	printf(" Enter your bind password : "); 
	gets(password);

	strtok(id," ");
	strtok(password," ");

	ezCall_bind( id , password );
	help_basic();
}

void help_submit()
{
	printf(" select job type \n");
	puts(" 1 : SMS at once ");
	puts(" 2 : SMS reservation ");
	puts(" 3 : TTS at once ");
	puts(" 4 : TTS reservation ");
	puts(" 5 : voice file attach(WAV, VOX) at once ");
	puts(" 6 : voice file attach(WAV, VOX) reservation ");
	puts(" 7 : FAX at once ");
	puts(" 8 : FAX reservation ");
	puts(" q : quit and return to help_basic() ");
	gets(in_str);


	if ( in_str[0] == '1' ) help_submit1();
	else if ( in_str[0] == '2' ) help_submit2();
	else if ( in_str[0] == '3' ) help_submit3();
	else if ( in_str[0] == '4' ) help_submit4();
	else if ( in_str[0] == '5' ) help_submit5();
	else if ( in_str[0] == '6' ) help_submit6();
	else if ( in_str[0] == '7' ) help_submit7();
	else if ( in_str[0] == '8' ) help_submit8();
	else if ( in_str[0] == 'q' ) help_basic();
	else help_submit();
}

void help_submit1()
{
	char subject[50];
	char callback_number[15];
	char dest_addr[15];
	char dest_name[15];
	char message[80];
	int ret_code=-1;
	int message_count=0;
	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit1();
	}
	strtok(callback_number," ");
	ezCall_pre_submit(1,subject,callback_number,"","");
	
	while(1){
		printf("Enter destination telephone number : ");
		if(strpbrk(dest_addr,"-. ,_") != NULL) 
		{
			fprintf(stderr,"Variable \"dest_addr\" can take string that only made up digit\n");
			help_submit1();
		}
		strtok(dest_addr," ");
		gets(dest_addr);
		printf("Enter destination name : ");
		gets(dest_name);
		printf("Enter Message to send : \n");
		gets(message);
				
		ret_code=ezCall_submit(	get_date(),
								get_time(),
								dest_addr,
								dest_name,
								message);
		if( ret_code == 0 ) 
		{
			puts(" Entry Successed ");
		}else{
			printf(" Entry Failed , %5dth messsage\n ", message_count);
			getchar();
			break;
		}
QUIT_WHILE:
		printf("Would you like quit? (y,n)");
		gets(in_str);
		if(in_str[0] == 'y' || in_str[0] == 'Y') break;
		else if(in_str[0] == 'n' || in_str[0] == 'N') continue;
		else {
			fprintf(stderr,"You must type 'y' or 'Y' or 'n' or 'N'");
			goto QUIT_WHILE ;
		}
	}
	help_submit();
}


void help_submit2()
{
	char subject[50];
	char callback_number[15];
	char dest_addr[15];
	char dest_name[15];
	char message[80];
	char send_date[10]; 
	char send_time[10];
	int ret_code=-1;
	int message_count=0;
	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit2();
	} 
	strtok(callback_number," ");

	ezCall_pre_submit(2,subject,callback_number,"","");
	
	while(1){
		printf("Enter destination telephone number : ");
		if(strpbrk(dest_addr,"-. ,_") != NULL) 
		{
			fprintf(stderr,"Variable \"dest_addr\" can take string that only made up digit\n");
			help_submit2();
		}
		strtok(dest_addr," ");
		gets(dest_addr);
		printf("Enter destination name : ");
		gets(dest_name);
		printf("Enter Message to send : \n");
		gets(message);
		printf("Enter date to send (YYYYMMDD) : ");
		gets(send_date);
		printf("Enter time to send (HHMI) : ");
		gets(send_time);
				
		ret_code=ezCall_submit(	send_date,
								send_time,
								dest_addr,
								dest_name,
								message);
							
		if( ret_code == 0 ) 
		{
			puts(" Entry Successed ");
		}else{
			printf(" Entry Failed , %5dth messsage\n ", message_count);
			getchar();
			break;
		}
QUIT_WHILE:
		printf("Would you like quit? (y,n)");
		gets(in_str);
		if(in_str[0] == 'y' || in_str[0] == 'Y') break;
		else if(in_str[0] == 'n' || in_str[0] == 'N') continue;
		else {
			fprintf(stderr,"You must type 'y' or 'Y' or 'n' or 'N'");
			goto QUIT_WHILE ;
		}
	}
	help_submit();
} 
void help_submit3()
{
	char subject[50];
	char callback_number[15];
	char dest_addr[15];
	char dest_name[15];
	char message[1000];
	int ret_code=-1;
	int message_count=0;
	
	printf(" Enter subject of Message \n");
	gets(subject);  
	printf(" Enter callback_number \n");
	gets(callback_number);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit3();
	}
	strtok(callback_number," ");

	ezCall_pre_submit(3,subject,callback_number,"","");
	
	while(1){
		printf("Enter destination telephone number : ");
		if(strpbrk(dest_addr,"-. ,_") != NULL) 
		{
			fprintf(stderr,"Variable \"dest_addr\" can take string that only made up digit\n");
			help_submit3();
		}
		strtok(dest_addr," ");
		gets(dest_addr);
		printf("Enter destination name : ");
		gets(dest_name);
		printf("Enter Message to send : \n");
		gets(message);
					
		ret_code=ezCall_submit(	get_date(),
								get_time(),
								dest_addr,
								dest_name,
								message);

		if( ret_code == 0 ) 
		{
			puts(" Entry Successed ");
		}else{
			printf(" Entry Failed , %5dth messsage\n ", message_count);
			getchar();
			break;
		}
QUIT_WHILE:
		printf("Would you like quit? (y,n)");
		gets(in_str);
		if(in_str[0] == 'y' || in_str[0] == 'Y') break;
		else if(in_str[0] == 'n' || in_str[0] == 'N') continue;
		else {
			fprintf(stderr,"You must type 'y' or 'Y' or 'n' or 'N'");
			goto QUIT_WHILE ;
		}
	}
	help_submit();
}
void help_submit4()
{
	char subject[50];
	char callback_number[15];
	char dest_addr[15];
	char dest_name[15];
	char message[1000];
	int ret_code=-1;
	int message_count=0;
	char send_date[10];
	char send_time[10];

	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit4();
	}
	strtok(callback_number," ");
	ezCall_pre_submit(4,subject,callback_number,"","");
	
	while(1){
		printf("Enter destination telephone number : ");
		if(strpbrk(dest_addr,"-. ,_") != NULL) 
		{
			fprintf(stderr,"Variable \"dest_addr\" can take string that only made up digit\n");
			help_submit4();
		}
		strtok(dest_addr," ");
		gets(dest_addr);
		printf("Enter destination name : ");
		gets(dest_name);
		printf("Enter Message to send : \n");
		gets(message);
		printf("Enter date to send (YYYYMMDD) : ");
		gets(send_date);
		printf("Enter time to send (HHMI) : ");
		gets(send_time);
				
		ret_code=ezCall_submit(	send_date,
								send_time,
								dest_addr,
								dest_name,
								message);
					
		if( ret_code == 0 ) 
		{
			puts(" Entry Successed ");
		}else{
			printf(" Entry Failed , %5dth messsage\n ", message_count);
			getchar();
			break;
		}
QUIT_WHILE:
		printf("Would you like quit? (y,n)");
		gets(in_str);
		if(in_str[0] == 'y' || in_str[0] == 'Y') break;
		else if(in_str[0] == 'n' || in_str[0] == 'N') continue;
		else {
			fprintf(stderr,"You must type 'y' or 'Y' or 'n' or 'N'");
			goto QUIT_WHILE ;
		}
	}
	help_submit();
}
void help_submit5() //음성첨부 즉시
{
	char subject[50];
	char callback_number[15];
	char user_file[80];
	char attach_file[80];
	int ret_code=-1;
	int message_count=0;

	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);
	printf(" Enter UserFile Path \n");
	gets(user_file);
	printf(" Enter AttachFile Path \n");
	gets(attach_file);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit5();
	}
	strtok(callback_number," ");
	ezCall_pre_submit(5,subject,callback_number,user_file,attach_file);
				
	ret_code=ezCall_submit(	get_date(),
							get_time(),
							"",
							"",
							"");
						
		if( ret_code == 0 ) 
		{
			puts(" Entry Successed ");
		}else{
			fprintf(stderr," Entry Failed ");
			help_submit();
		}
	help_submit();
}
void help_submit6()
{
	char subject[50];
	char callback_number[15];
	char user_file[80];
	char attach_file[80];
	int ret_code=-1;
	int message_count=0;
	char send_date[10];
	char send_time[10];

	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);
	printf(" Enter UserFile Path \n");
	gets(user_file);
	printf(" Enter AttachFile Path \n");
	gets(attach_file);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit6();
	}
	strtok(callback_number," ");
	ezCall_pre_submit(6,subject,callback_number,user_file,attach_file);
	
	
	printf("Enter date to send (YYYYMMDD) : ");
	gets(send_date);
	printf("Enter time to send (HHMI) : ");
	gets(send_time);
	
	ret_code=ezCall_submit(	send_date,
							send_time,
							"",
							"",
							"");
				
	if( ret_code == 0 ) 
	{
		puts(" Entry Successed ");
	}else{
		fprintf(stderr," Entry Failed ");
		help_submit();
	}

	help_submit();
}
void help_submit7()
{
	char subject[50];
	char callback_number[15];
	char user_file[80];
	char attach_file[80];
	int ret_code=-1;
	int message_count=0;
	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);
	printf(" Enter UserFile Path \n");
	gets(user_file);
	printf(" Enter AttachFile Path \n");
	gets(attach_file);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit7();
	}
	strtok(callback_number," ");
	int gid=ezCall_pre_submit(7,subject,callback_number,user_file,attach_file);
	printf("gid=%d\n",gid);
	
	ret_code=ezCall_submit(	get_date(),
							get_time(),
							"",
							"",
							"");
				
	if( ret_code == 0 ) 
	{
		puts(" Entry Successed ");
	}else{
		fprintf(stderr," Entry Failed ");
		help_submit();
	}
	help_submit();
}
void help_submit8()
{
	char subject[50];
	char callback_number[15];
	char user_file[50];
	char attach_file[50];
	char send_date[10];
	char send_time[10];
	int ret_code=-1;
	int message_count=0;

	printf(" Enter subject of Message \n");
	gets(subject);
	printf(" Enter callback_number \n");
	gets(callback_number);
	printf(" Enter UserFile Path \n");
	gets(user_file);
	printf(" Enter AttachFile Path \n");
	gets(attach_file);

	if(strpbrk(callback_number,"-. ,_") != NULL) 
	{
		fprintf(stderr,"Variable \"callback_number\" can take string that only made up digit\n");
		help_submit8();
	}
	strtok(callback_number," ");
	ezCall_pre_submit(8,subject,callback_number,user_file,attach_file);
	
	
	printf("Enter date to send (YYYYMMDD) : ");
	gets(send_date);
	printf("Enter time to send (HHMI) : ");
	gets(send_time);
	
	ret_code=ezCall_submit(	send_date,
							send_time,
							"",
							"",
							"");
								
	if( ret_code == 0 ) 
	{
		puts(" Entry Successed ");
	}else{
		fprintf(stderr," Entry Failed ");
		help_submit();
	}
	help_submit();
}
 
void help_report()
{
	int gid;
	int report_type;
	int report_count=0;
	char start_date[10];
	char end_date[10];

	printf(" Enter Report Type :\n");
	printf(" 0 : Select All Message among Start date and End date by Entry date  \n");
	printf(" 1 : Select SMS Message among Start date and End date by Entry date \n");
	printf(" 2 : Select TTS Message among Start date and End date by Entry date \n");
	printf(" 3 : Select Voice file attach Message \n	among Start date and End date by Entry date \n");
	printf(" 4 : Select Fax Message among Start date and End date by Entry date \n");
	printf(" 10: Select All Message among Start date and End date by Send date \n");
	printf(" 11: Select SMS Message among Start date and End date by Send date \n");
	printf(" 12: Select TTS attach Message among Start date and End date by Send date \n");
	printf(" 13: Select Voice file attach Message \n	among Start date and End date by Send date\n");
	printf(" 14: Select FAX Message among Start date and End date by Send date\n");
	gets(in_str);
	report_type=atoi(in_str);

	printf(" Enter Method of Report to reqire \n ");  
	printf(" 1 : Request Report by gid \n ");
	printf(" 2 : Request Report by start date and end date \n ");  
	gets(in_str);

	if(in_str[0]=='1')
	{
		printf(" Enter Gid what you search : ");
		scanf("%d",&gid);
		report_count=ezCall_report("","",report_type,gid);
		printf("Total %7d Report Data recieved \n",report_count);
	}else if(in_str[0]=='2'){
		printf(" Enter Start date (ex.20050101) what you search : ");
		gets(start_date);
		puts("\n");
		printf(" Enter End date (ex.20040101) what you search : ");
		gets(end_date);
		report_count=ezCall_report(start_date,end_date,report_type,0);
		printf("Total %7d Report Data recieved \n",report_count);
	}else{
		fprintf(stderr,"You have incorrect Enterence, try again!!");
		help_report();
	}
	help_basic();
}

void help_result()
{
	int gid;
	int result_count=0;
	
	printf(" Enter gid what you searching for(Enter \"0\" will quit fuction.) : ");
	gets(in_str);
	gid=atoi(in_str);
	if(gid==0) help_basic();
	printf("gid=%d\n",gid);
	result_count=ezCall_result(gid);
	printf("Total %7d Result Data recieved \n",result_count);
	help_result();
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

	sprintf( str, "%.2d%.2d",
		systime.wHour,
		systime.wMinute
		);

	ret = str;

	return ret;
}