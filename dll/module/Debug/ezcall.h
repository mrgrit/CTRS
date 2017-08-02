
#define EZCALL_API extern "C" __declspec(dllimport)
#define CALLBACK_API extern "C" __declspec(dllexport)


EZCALL_API int ezCall_bind(char *id,char *password);

EZCALL_API int ezCall_pre_submit(int jobtype,char *subject,char *callback_number,char *dest_file,char *attach_file);

EZCALL_API int ezCall_submit(char *msdate, 
							 char *mstime, 
						     char *dest_addr,
						     char *dest_name, 
						     char *message);
						     
EZCALL_API int ezCall_result(int gid);

CALLBACK_API int result_set(int gid,
							char *subject,
							char *dest_name,
							char *dest_addr,
							int jobtype,
							char *start_time,
							char *end_time,
							int jobid,
							int duration,
							int pages,
							int fee,
							int retcode);

EZCALL_API int ezCall_report(char *start_date, char *end_date, int report_type, int gid);

CALLBACK_API int report_set(int gid,
							char *entry_time,
							char *subject,
							int jobtype,
							int success,
							int fail,
							int fee,
							int entry_success,
							int entry_fail,
							int total,
							int result_status);
