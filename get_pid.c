#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

int FindPID(const char *procname) {

HANDLE Snap;
PROCESSENTRY32 pe32;
int pid=0;

Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if(INVALID_HANDLE_VALUE == Snap) {
	printf("CreateToolhelp32Snapshot() failed, error: %x\n", GetLastError());
	return 0;
	}

pe32.dwSize = sizeof(PROCESSENTRY32);

if(!Process32First(Snap, &pe32)){
	printf("Process32First() failed, error: %x\n", GetLastError());
	CloseHandle(Snap);
	return 0;
	}

while(Process32Next(Snap, &pe32)){
		if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
			pid = pe32.th32ProcessID;
			break;
		}
}

CloseHandle(Snap);
return pid;

}
	
int KillPID(int pid){

HANDLE hPid;

hPid = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
if (!hPid){
	printf("Unable to obtain %d handle, error %x\n", pid, GetLastError());
	CloseHandle(hPid);
	return 0;
}

if(!TerminateProcess(hPid, 0)){
	printf("Unable to terminate process %d, error %x\n", pid, GetLastError());
	CloseHandle(hPid);
	return 0;
}

CloseHandle(hPid);
return 1;


}	
	
int main(int argc, char *argv[]) {

if (argc==1){
	printf("usage: %s procname.exe", argv[0]);
	return -1;
}

//char proc[]="acad.exe";
char *proc=argv[1];
int pid=0;
int kpid=0;

pid=FindPID(proc);


if (pid){
	printf("%s - PID:%d\n", proc, pid);
	}
else
	{
	printf("locating PID of %s failed\n", proc);
	return -1;
	}
	
kpid = KillPID(pid);

if(kpid){
	printf("successfully kill of proc %s - %d\n", proc, pid);
	return 0;
}
else
{
	printf("failed kill of proc %s - %d\n", proc, pid);
	return 1;
}

}