#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>
#include <windows.h>
#include <tlhelp32.h>
#include <vdmdbg.h>
#include <cstdio>
 
#define RED         (FOREGROUND_RED | FOREGROUND_INTENSITY)


//compile option
//#define USEMESSAGE_QUEUE


SOCKET g_Sock;

#define COMMAND_SIZE 2000
#define MAX_COMMAND  1000
#define COMMAND_TYPE_SIZE 20

#define LIST_TOKEN "@"

char process_list[COMMAND_SIZE];


typedef BOOL (CALLBACK *PROCENUMPROC)(DWORD, WORD, LPSTR, LPARAM);

typedef struct {
   DWORD          dwPID;
   PROCENUMPROC   lpProc;
   DWORD          lParam;
   BOOL           bEnd;
} EnumInfoStruct;

BOOL WINAPI EnumProcs(PROCENUMPROC lpProc, LPARAM lParam);

BOOL WINAPI Enum16(DWORD dwThreadId, WORD hMod16, WORD hTask16,
      PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined);


BOOL WINAPI EnumProcs(PROCENUMPROC lpProc, LPARAM lParam) {

   OSVERSIONINFO  osver;
   HINSTANCE      hInstLib  = NULL;
   HINSTANCE      hInstLib2 = NULL;
   HANDLE         hSnapShot = NULL;
   LPDWORD        lpdwPIDs  = NULL;
   PROCESSENTRY32 procentry;
   BOOL           bFlag;
   DWORD          dwSize;
   DWORD          dwSize2;
   DWORD          dwIndex;
   HMODULE        hMod;
   HANDLE         hProcess;
   char           szFileName[MAX_PATH];
   EnumInfoStruct sInfo;

   // ToolHelp Function Pointers.
   HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD, DWORD);
   BOOL (WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
   BOOL (WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);

   // PSAPI Function Pointers.
   BOOL (WINAPI *lpfEnumProcesses)(DWORD *, DWORD, DWORD *);
   BOOL (WINAPI *lpfEnumProcessModules)(HANDLE, HMODULE *, DWORD,
         LPDWORD);
   DWORD (WINAPI *lpfGetModuleBaseName)(HANDLE, HMODULE, LPTSTR, DWORD);

   // VDMDBG Function Pointers.
   INT (WINAPI *lpfVDMEnumTaskWOWEx)(DWORD, TASKENUMPROCEX, LPARAM);
memset(process_list,0,2000);
   // Retrieve the OS version
   osver.dwOSVersionInfoSize = sizeof(osver);
   if (!GetVersionEx(&osver))
      return FALSE;

   // If Windows NT 4.0
   if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT
         && osver.dwMajorVersion == 4) {

      __try {

         // Get the procedure addresses explicitly. We do
         // this so we don't have to worry about modules
         // failing to load under OSes other than Windows NT 4.0
         // because references to PSAPI.DLL can't be resolved.
         hInstLib = LoadLibraryA("PSAPI.DLL");
         if (hInstLib == NULL)
            __leave;

         hInstLib2 = LoadLibraryA("VDMDBG.DLL");
         if (hInstLib2 == NULL)
            __leave;

         // Get procedure addresses.
         lpfEnumProcesses = (BOOL (WINAPI *)(DWORD *, DWORD, DWORD*))
               GetProcAddress(hInstLib, "EnumProcesses");

         lpfEnumProcessModules = (BOOL (WINAPI *)(HANDLE, HMODULE *,
               DWORD, LPDWORD)) GetProcAddress(hInstLib,
               "EnumProcessModules");

         lpfGetModuleBaseName = (DWORD (WINAPI *)(HANDLE, HMODULE,
               LPTSTR, DWORD)) GetProcAddress(hInstLib,
               "GetModuleBaseNameA");

         lpfVDMEnumTaskWOWEx = (INT (WINAPI *)(DWORD, TASKENUMPROCEX,
               LPARAM)) GetProcAddress(hInstLib2, "VDMEnumTaskWOWEx");

         if (lpfEnumProcesses == NULL
               || lpfEnumProcessModules == NULL
               || lpfGetModuleBaseName == NULL
               || lpfVDMEnumTaskWOWEx == NULL)
            __leave;

         //
         // Call the PSAPI function EnumProcesses to get all of the
         // ProcID's currently in the system.
         //
         // NOTE: In the documentation, the third parameter of
         // EnumProcesses is named cbNeeded, which implies that you
         // can call the function once to find out how much space to
         // allocate for a buffer and again to fill the buffer.
         // This is not the case. The cbNeeded parameter returns
         // the number of PIDs returned, so if your buffer size is
         // zero cbNeeded returns zero.
         //
         // NOTE: The "HeapAlloc" loop here ensures that we
         // actually allocate a buffer large enough for all the
         // PIDs in the system.
         //
         dwSize2 = 256 * sizeof(DWORD);
         do {

            if (lpdwPIDs) {
               HeapFree(GetProcessHeap(), 0, lpdwPIDs);
               dwSize2 *= 2;
            }

            lpdwPIDs = (LPDWORD) HeapAlloc(GetProcessHeap(), 0,
                  dwSize2);
            if (lpdwPIDs == NULL)
               __leave;

            if (!lpfEnumProcesses(lpdwPIDs, dwSize2, &dwSize))
               __leave;

         } while (dwSize == dwSize2);

         // How many ProcID's did we get?
         dwSize /= sizeof(DWORD);

         // Loop through each ProcID.
         for (dwIndex = 0; dwIndex < dwSize; dwIndex++) {

            szFileName[0] = 0;

            // Open the process (if we can... security does not
            // permit every process in the system to be opened).
            hProcess = OpenProcess(
                  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                  FALSE, lpdwPIDs[dwIndex]);
            if (hProcess != NULL) {

               // Here we call EnumProcessModules to get only the
               // first module in the process. This will be the
               // EXE module for which we will retrieve the name.
               if (lpfEnumProcessModules(hProcess, &hMod,
                     sizeof(hMod), &dwSize2)) {

                  // Get the module name
                  if (!lpfGetModuleBaseName(hProcess, hMod,
                        szFileName, sizeof(szFileName)))
                     szFileName[0] = 0;
               }
               CloseHandle(hProcess);
            }
            // Regardless of OpenProcess success or failure, we
            // still call the enum func with the ProcID.
            if (!lpProc(lpdwPIDs[dwIndex], 0, szFileName, lParam))
               break;

            // Did we just bump into an NTVDM?
            if (_stricmp(szFileName, "NTVDM.EXE") == 0) {

               // Fill in some info for the 16-bit enum proc.
               sInfo.dwPID = lpdwPIDs[dwIndex];
               sInfo.lpProc = lpProc;
               sInfo.lParam = (DWORD) lParam;
               sInfo.bEnd = FALSE;

               // Enum the 16-bit stuff.
               lpfVDMEnumTaskWOWEx(lpdwPIDs[dwIndex],
                  (TASKENUMPROCEX) Enum16, (LPARAM) &sInfo);

               // Did our main enum func say quit?
               if (sInfo.bEnd)
                  break;
            }
         }

      } __finally {

         if (hInstLib)
            FreeLibrary(hInstLib);

         if (hInstLib2)
            FreeLibrary(hInstLib2);

         if (lpdwPIDs)
            HeapFree(GetProcessHeap(), 0, lpdwPIDs);
      }

   // If any OS other than Windows NT 4.0.
   } else if (osver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS
         || (osver.dwPlatformId == VER_PLATFORM_WIN32_NT
         && osver.dwMajorVersion > 4)) {

      __try {

         hInstLib = LoadLibraryA("Kernel32.DLL");
         if (hInstLib == NULL)
            __leave;

         // If NT-based OS, load VDMDBG.DLL.
         if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) {
            hInstLib2 = LoadLibraryA("VDMDBG.DLL");
            if (hInstLib2 == NULL)
               __leave;
         }

         // Get procedure addresses. We are linking to
         // these functions explicitly, because a module using
         // this code would fail to load under Windows NT,
         // which does not have the Toolhelp32
         // functions in KERNEL32.DLL.
         lpfCreateToolhelp32Snapshot =
               (HANDLE (WINAPI *)(DWORD,DWORD))
               GetProcAddress(hInstLib, "CreateToolhelp32Snapshot");

         lpfProcess32First =
               (BOOL (WINAPI *)(HANDLE,LPPROCESSENTRY32))
               GetProcAddress(hInstLib, "Process32First");

         lpfProcess32Next =
               (BOOL (WINAPI *)(HANDLE,LPPROCESSENTRY32))
               GetProcAddress(hInstLib, "Process32Next");

         if (lpfProcess32Next == NULL
               || lpfProcess32First == NULL
               || lpfCreateToolhelp32Snapshot == NULL)
            __leave;

         if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) {
            lpfVDMEnumTaskWOWEx = (INT (WINAPI *)(DWORD, TASKENUMPROCEX,
                  LPARAM)) GetProcAddress(hInstLib2, "VDMEnumTaskWOWEx");
            if (lpfVDMEnumTaskWOWEx == NULL)
               __leave;
         }

         // Get a handle to a Toolhelp snapshot of all processes.
         hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
         if (hSnapShot == INVALID_HANDLE_VALUE) {
            FreeLibrary(hInstLib);
            return FALSE;
         }

         // Get the first process' information.
         procentry.dwSize = sizeof(PROCESSENTRY32);
         bFlag = lpfProcess32First(hSnapShot, &procentry);

         // While there are processes, keep looping.
         while (bFlag) {

            // Call the enum func with the filename and ProcID.
            if (lpProc(procentry.th32ProcessID, 0,
                  procentry.szExeFile, lParam)) {

               // Did we just bump into an NTVDM?
               if (_stricmp(procentry.szExeFile, "NTVDM.EXE") == 0) {

                  // Fill in some info for the 16-bit enum proc.
                  sInfo.dwPID = procentry.th32ProcessID;
                  sInfo.lpProc = lpProc;
                  sInfo.lParam = (DWORD) lParam;
                  sInfo.bEnd = FALSE;

                  // Enum the 16-bit stuff.
                  lpfVDMEnumTaskWOWEx(procentry.th32ProcessID,
                     (TASKENUMPROCEX) Enum16, (LPARAM) &sInfo);

                  // Did our main enum func say quit?
                  if (sInfo.bEnd)
                     break;
               }

               procentry.dwSize = sizeof(PROCESSENTRY32);
               bFlag = lpfProcess32Next(hSnapShot, &procentry);

            } else
               bFlag = FALSE;
         }

      } __finally {

         if (hInstLib)
            FreeLibrary(hInstLib);

         if (hInstLib2)
            FreeLibrary(hInstLib2);
      }

   } else
      return FALSE;

   // Free the library.
   FreeLibrary(hInstLib);
send(g_Sock,process_list,strlen(process_list),0);
printf("len=%d\n",strlen(process_list));
   return TRUE;
}


BOOL WINAPI Enum16(DWORD dwThreadId, WORD hMod16, WORD hTask16,
      PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined) {

   BOOL bRet;

   EnumInfoStruct *psInfo = (EnumInfoStruct *)lpUserDefined;

   bRet = psInfo->lpProc(psInfo->dwPID, hTask16, pszFileName,
      psInfo->lParam);

   if (!bRet)
      psInfo->bEnd = TRUE;

   return !bRet;
}


BOOL CALLBACK MyProcessEnumerator(DWORD dwPID, WORD wTask,
      LPCSTR szProcess, LPARAM lParam) {

	char process[50]=" ";

   if (wTask == 0)
      sprintf(process,"[%u,%s]@", dwPID, szProcess);
   else
      sprintf(process,"[%u,%s]@", wTask, szProcess);
   strtok(process," ");
   strcat(process_list,process);
   strtok(process_list," ");
   return TRUE;
}

HANDLE	hThread,hThread_2;
DWORD	hID, hID2;
CRITICAL_SECTION  cs, cs_2;							// 스래드 동기화 변수

//function
DWORD WINAPI shellThread(void *);
DWORD WINAPI getCommandThread(void *);

void init_queue(void);
int putCommandQueue(char *,int,int);
void ErrorHandling(char *message);
void list(SOCKET sock,char *path,int num);
int kill(int pid);
int del_dir(char *,int);
void del_file(char *,int);
void exec(char *,int);


char Command[COMMAND_SIZE];

#if defined USE_MESSAGE_QUEUE
struct _CommandQueue
{
	int command_num;
	int temp_param;
	char com_contents[COMMAND_SIZE];
};

typedef struct _CommandQueue CommandQueue;
CommandQueue Q[MAX_COMMAND];

int F, R;				// Front, Rear
int QueueCount;
#endif

void PrintString(HANDLE hStdOut, WORD Attribute, LPCSTR str) {
    SetConsoleTextAttribute(hStdOut, Attribute);
    puts(str);
}


int main(){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 
    PrintString(hStdOut, RED, "빨간색");
    
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	 ErrorHandling("WSAStartup() error!");
  
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
    ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;
    servAddr.sin_addr.s_addr=inet_addr("211.224.130.165");
    servAddr.sin_port=htons(8282);
  
   if(connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error");
   g_Sock=sock;
#if defined USE_MESSAGE_QUEUE
   init_queue();
#endif
   hThread	  = (HANDLE)CreateThread(NULL, 0, shellThread, (void*)sock, 0, (unsigned long *)&hID  );
#if defined USE_MESSAGE_QUEUE
   hThread_2   = (HANDLE)CreateThread(NULL, 0, getCommandThread, (void*)sock, 0, (unsigned long *)&hID2 );

   
	if ( hThread == 0 || hThread_2 == 0 ) {
		fprintf(stderr, "\n Thread Created ERROR[%d] : [%d] : [%d] !! \n", hThread, hThread_2);
		fprintf(stderr, "HIT KEY....(EXIT)");
		getchar();
		exit(1);
	}
#else
	if ( hThread == 0 ) {
		fprintf(stderr, "\n Thread Created ERROR[%d] !! \n", hThread);
		fprintf(stderr, "HIT KEY....(EXIT)");
		getchar();
		exit(1);
	}
#endif
  WaitForSingleObject(hThread, INFINITE);
#if defined USE_MESSAGE_QUEUE
  WaitForSingleObject(hThread_2, INFINITE);
#endif
  closesocket(sock);
  return 0;
}

DWORD WINAPI shellThread(void *arg)
{
	SOCKET sock = (SOCKET)arg;
    int strLen;
	char *temp;
	char command_type[COMMAND_TYPE_SIZE];
	char param[MAX_PATH];
	int Command_num;
	int job_type;
	int pid;
  while(1){
    strLen = recv(sock, Command, COMMAND_SIZE, 0);
	
	if(strLen==-1) 
	{
		return 1;
	}
	printf("<----------------shellThread command----------------->\n%s\n",Command);
	strtok(Command,"|");
	Command_num=atoi(Command);
	printf("command_num=%d\n",Command_num);
	
#if defined USE_MESSAGE_QUEUE
	putCommandQueue(temp,Command_num,0);
#else
	temp=strtok(NULL,"^");
	if(temp==NULL)
	{
		Sleep(1000);
		continue;
	}
		//goto BREAK;
	printf("temp=%s\n",temp);
	strcpy(command_type,temp);
	//strtok(command_type," ");
	printf("command_type=%s\n",command_type);
	printf("shellThread command=%s\n",Command);
	temp=strtok(NULL,"|");
	printf("temp=%s\n",temp);
	strcpy(param,temp);
	strtok(param," ");
	printf("param=%s\n",param);
	if(strncmp(command_type,"list",strlen("list"))==0) job_type=1;
	else if(strncmp(command_type,"del_dir",strlen("del_dir"))==0) job_type=2;
	else if(strncmp(command_type,"proc_list",strlen("proc_list"))==0) job_type=3;
	else if(strncmp(command_type,"kill",strlen("kill"))==0) job_type=4;
	else if(strncmp(command_type,"exec",strlen("exec"))==0) job_type=5;
	else if(strncmp(command_type,"del_file",strlen("del_file"))==0) job_type=6;
		switch (job_type){
			case 1: 
				list(sock,param,Command_num);
				break;
			case 2:
				del_dir(param,Command_num);
				break;
			case 3:
				EnumProcs((PROCENUMPROC) MyProcessEnumerator, 0);
				break;
			case 4:
				pid=atoi(param);
				kill(pid);
				break;
			case 5:
		//		exec(param,Command_num);
				break;
			case 6:
				del_file(param,Command_num);
				break;
			default : 
				break;
		}
#endif
//BREAK :
		Sleep(1000);
  }//End while(1)
}
#if defined USE_MESSAGE_QUEUE
int putCommandQueue(char *Command,int command_num,int temp_param)
{
	R = (R+1) % MAX_COMMAND;
	printf("[Put Command Queue] -> PUT[1]\n");

	if (F == R) 			// 데이타가 큐 에 꽉찬 상태 , 스래드와 동기화 예정
		fprintf(stderr, "put() : Q is Full.. [%d][%d] \n", F, R);

	// Rear 포인터에 테이타 저장
	printf("[Q에 저장]\n");
	strcpy(Q[R].com_contents,Command);
	Q[R].command_num = command_num;
	Q[R].temp_param = temp_param;
	R=++R % MAX_COMMAND;
	++QueueCount;				// 입력 자료의 갯수
/*
	if (q_flag == 1) { 
		// 세마포어(1) unlock
		LeaveCriticalSection( &cs );
		q_flag = 0;
		printf("[Q_1 : 자료가 다시 들어 갑니다.\n");
	}	
*/	
	return 0;
}

DWORD WINAPI getCommandThread(void *arg)
{
	SOCKET sock = (SOCKET)arg;
	CommandQueue t;		// 임시 데이타 

	char *Command;
	int com_num;
	int temp_param;
	int command_type;
	char *temp;


	fprintf(stdout, "Thread BLOCK!!\n"); 
//	EnterCriticalSection( &cs );	// 최초 대기 상태
	Sleep(100);
	fprintf(stdout, "Thread First - Start!! \n");
			
	while (1) {
		if (F == R) {			// 데이타가 없다 
			Sleep(100);
			continue;
            //fprintf(stderr, "get() : Q is Empty \n");
		} else {
			F = (F + 1) % MAX_COMMAND;
			t = Q[F];
		}
		
		Command=t.com_contents;
		com_num=t.command_num;
		temp_param=t.temp_param;
		
		strtok(Command,"^");
		temp=strtok(NULL,"|");
		if(strcmp(Command,"list")==0) command_type=1;
	//	if else(strcmp(command,

		switch (command_type){
			case 1: 
				list(sock,temp,com_num);
				break;
			default : 
				break;
		}
		
	}
		
	return 1;
	
}
//command의 규격 형식 : "123|list^c:\|"

// 초기화 
void init_queue(void)
{
	// 원형큐의 초기화 : F 와 R 이 초기 위치인 -1 이며, 아무런 데이타를 
	// 가지지 않은 상태 
	F = -1;
	R = -1;
}
#endif
void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

void list(SOCKET sock,char *path,int command_num)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH];
	BOOL bResult=TRUE;
	char buff[MAX_PATH];
	char com_num[4];
	char list[COMMAND_SIZE];

	
	memset(buff,0,MAX_PATH);
	memset(list,0,COMMAND_SIZE);
	sprintf(buff,"%s*.*",path);
	strtok(buff," ");
		
	hSrch=FindFirstFile(buff,&wfd);
	while(1){
		memset(fname,0,MAX_PATH);
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			sprintf(fname,"[%s]",wfd.cFileName);
		//	strtok(fname," ");
			strcat(fname,LIST_TOKEN);
		//	strtok(fname," ");

		}else{
			sprintf(fname,"%s",wfd.cFileName);
		//	strtok(fname," ");
			strcat(fname,LIST_TOKEN);
		//	strtok(fname," ");
		}
		strncat(list,fname,strlen(fname));
	//	strtok(list," ");
		bResult=FindNextFile(hSrch,&wfd);
		if(bResult==FALSE) break;
	}//eND While(1)
//	strtok(list," ");
	send(sock,list,strlen(list),0);
	printf("---------------list()=%s\n",list);
	itoa(command_num,com_num,10);
//	strtok(com_num," ");
//	sprintf(com_num,"[%s]-complete",com_num);
	send(sock,com_num,strlen(com_num),0);
	FindClose(hSrch);
}


int kill(int pid)
{
	static DWORD ProcID=0;
	static HANDLE hProc=0;
	DWORD ExitCode;
	hProc=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
    GetExitCodeProcess(hProc,&ExitCode);
    TerminateProcess(hProc,0);
	return 0;
}

int del_dir(char *path,int Command_num)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH];
	BOOL bResult=TRUE;
	char drive[_MAX_DRIVE];
	char dir[MAX_PATH];
	char newpath[MAX_PATH];

	if(strlen(path)<=4)
		return FALSE;

	strcpy(newpath,path);
	if(strcmp(newpath+strlen(newpath)-4,"\\*.*")!=0)
		strcat(newpath,"\\*.*");
	hSrch=FindFirstFile(newpath,&wfd);
	if(hSrch==INVALID_HANDLE_VALUE)
		return FALSE;
	_splitpath(newpath,drive,dir,NULL,NULL);
	while(bResult){
		wsprintf(fname,"%s%s%s",drive,dir,wfd.cFileName);

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY){
			SetFileAttributes(fname,FILE_ATTRIBUTE_NORMAL);
		}

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if(wfd.cFileName[0]!='.'){
				wsprintf(newpath,"%s%s%s\\*.*",drive,dir,wfd.cFileName);
				del_dir(newpath,Command_num);
			}
		}else{
			DeleteFile(fname);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);

	wsprintf(fname,"%s%s",drive,dir);
	fname[strlen(fname)-1]=0;
	if(RemoveDirectory(fname)==FALSE){
		//사용중 폴더는 삭제되지 않으므로 예외처리.
		
	}
	return TRUE;
}

void del_file(char *path,int Command_num)
{
		DeleteFile(path);
}


/*
void CXXXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if(nID == SC_CLOSE) 
    {
        MoveToTray(GetSafeHwnd());
        return;
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}
*/
/*
void black_box(void)
{
		CFile pf;
        SHM_STR *shm_str;       
        bool isCreate = 0;
        SHM_STR tmp;
//이미 존재하고 있는 공유 메모리가 있는지 확인
        HANDLE hwMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, "daw");
//daw 라는 이름의 공유메모리가 없다면
        if(hwMap == NULL){
//공유 메모리와 연결된 파일의 값을 읽는다
            FILE *fp;
            fp = fopen("TEST.txt", "r");
            if(fp){
                fread(&tmp, sizeof(SHM_STR), 1, fp);
                fclose(fp);
            }
//공유 메모리와 연결될 파일을 open 또는 create
            if(!pf.Open("TEST.txt",CFile::modeCreate | CFile::modeReadWrite | 
                CFile::shareDenyRead | CFile::shareDenyWrite ))
                return 0;
//file mapping 을 하고
            hwMap = CreateFileMapping((HANDLE)pf.m_hFile,NULL,PAGE_READWRITE,0,
                sizeof(SHM_STR), "daw");
            if(hwMap==NULL)
                return 0;
//공유 메모리를 create 했다
            isCreate = true;
        }
//공유 메모리에 대한 포인터를 얻는다
        shm_str = (SHM_STR *)MapViewOfFile(hwMap,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
        if(shm_str == NULL)
            return 0;
//만약 공유메모리를 생성한 과정이였다면 읽은 파일의 내용을 공유메모리에 카피한다
        if(isCreate)
            memcpy(shm_str, &tmp, sizeof(SHM_STR));
//      shm_str->mtype = 100;
//      strcpy(shm_str->body, "body");
         printf("%d %s\n", shm_str->mtype, shm_str->body);
//      getch();
        ::UnmapViewOfFile(shm_str);
        ::CloseHandle(hwMap);
        pf.Close();
    }

}*/