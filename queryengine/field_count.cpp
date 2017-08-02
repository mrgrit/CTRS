#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int field_count(char *);
int row_count(char *);

main()
{
	int count;
	count=field_count("select a,b,c,d,e from test");
	printf("count=%d\n",count);
	count=row_count("select a,b,c,d,e, from test");
	getchar();
}

int field_count(char *message)
{
	int count=0;
	int i;
	char *message_temp;
	char temp;
	puts(message);
//	strtok(message,"from");
//	puts(message);
//	printf("msg_temp=%s\n",message);
	getchar();
	for(i=0;i<=strlen(message);i++)
	{
		temp=message[i];
		if(temp==',') count++;
	}
	return count+1;

}

int row_count(char *message)
{

	int count=0;
	char *message_temp;
	message_temp=strstr(message,"from");
	printf("message=%s\n",message_temp);
	getchar();
	return count;
}