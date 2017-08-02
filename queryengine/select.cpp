#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

int query_exe(char *);
char *select_query(char *,int);
int field_count(char *);

/***********************************/
/* DB핸들, 리턴코드 선언 */                         
HENV 		henv;
HDBC 		hdbc;
HSTMT 		hstmt,hstmt1;
HSTMT		th_1, hstmt_c;
RETCODE 	_ret;
/***********************************/


//struct _DATA_TYPE{
	int type_int[20];
	char type_char[20][100];
	int count_int;
	int count_char;
	SDWORD type_int_len=SQL_NTS;
	SDWORD type_char_len=SQL_NTS;
//};
//typedef struct _DATA_TYPE DATA_TYPE;

int main()
{
  int query_return;
  char *select_result;
  int field_num;
  
  
/********************************************************************************************************/
// 초기화 - MS SQL
henv = hdbc = hstmt = 0;  

// MS SQL 에 접속을 시도 한다
SQLAllocEnv(&henv);
SQLAllocConnect(henv,&hdbc);
_ret=SQLConnect(hdbc,(UCHAR *)"KARMA",SQL_NTS,(UCHAR *)"karma",SQL_NTS,(UCHAR *)"karmahaja8282",SQL_NTS);
/********************************************************************************************************/
 //   char *query="select message2,gid from msg where gid=3";
//	select_result=select_query(query,2);
	char *query="update msg_group set uid=1";
	query_return=query_exe(query);
	puts("aaaaaaaaaaaaaaaaaa");
	printf("query_return=%d\n",query_return);
getchar();
  return 0;
}


int query_exe(char *query)
{
//	query=(char *)malloc(strlen(query));
	puts(query);
	hstmt=0;
	_ret=SQLAllocStmt(hdbc,&hstmt);
	{
	_ret=SQLPrepare(hstmt,(UCHAR *)query,SQL_NTS); 
   	_ret=SQLExecute(hstmt);
	printf("exe=%d\n",_ret);
	getchar();
	_ret=SQLFreeStmt(hstmt,SQL_DROP);
	}
//	free(query);
	return _ret;
}

char *select_query(char *query,int val_num)
{
	count_int=0; // int형 변수의 갯수.
	count_char=0;// char형 변수의 갯수.
	char result[1000];//쿼리 질의결과 
	memset(result,0,1000);
	char *result_ptr;//result를 가리키는 포인터
	char result_set[40];//int와 char의 순서를 저장하는 변수
	int i;
	long cbValue;

	hstmt=0;
	SQLAllocStmt(hdbc,&hstmt);
	SQLPrepare(hstmt,(UCHAR *)query,SQL_NTS);
	char *field[40]; //필드 이름의 집합
	char *field_temp;
	char itoa_temp[15]="";
	char *itoa_temp_ptr;
	char *type_char_ptr;

//(type_char[count_char])
	for(i=0;i<=val_num-1;i++)
	{
		printf("before count_int=%d\n",count_int);
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
	_ret=SQLFetch(hstmt);
	int count_int2=0;
	int count_char2=0;
	for(i=0;i<=val_num-1;i++)
	{
		if(result_set[i]=='i')
		{
			printf("count_int2=%d\n",count_int2);
			printf("type_int[0]=%d\n",type_int[0]);
			printf("type_int[count_int2]=%d\n",type_int[count_int2]);
			itoa(type_int[count_int2],itoa_temp,10);
			itoa_temp_ptr=itoa_temp;
			field[i]=itoa_temp_ptr;
			strtok(field[i]," ");
			count_int2++;
			strncat(field[i],"^",1);
			strncat(result,field[i],strlen(field[i]));
		}else if(result_set[i]=='c'){
			type_char_ptr=type_char[count_char2];
			field[i]=type_char_ptr;
			count_char2++;
			strncat(field[i],"^",1);
			printf("field[cc]=%s\n",field[i]);
			strncat(result,field[i],strlen(field[i]));
			printf("result ccc=%s\n",result);
		}//else if(result_set[i]+i=='c') ENd
	}//for(i=0;i<=val_num-1;i++) ENd
	printf("result_len=%d\n",strlen(result));
	result[strlen(result)-1]=0;
	result_ptr=result;
	printf("result_ptr=%s\n",result_ptr); 
	printf("strlen=%d\n",strlen(result_ptr));
	getchar();
	return result_ptr;
}

int field_count(char *query)
{



}
