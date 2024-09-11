#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <processthreadsapi.h>
#include <tchar.h>
#include <winbase.h>
//#include "mainheader.cpp"
#include <TlHelp32.h>

int Get_Process_Handle(DWORD ProcessAccessInfo, DWORD processID);
int Set_Process_Priority(HANDLE hprocess, DWORD priorityclass);
int Set_Process_Affinity(HANDLE process_handle, DWORD_PTR dwProcessAffinityMask);
int Get_Process_Snapshot(DWORD processID);
int Set_Threads_Priority(HANDLE thread_snap, int Priority);
int Set_Threads_Affinity(HANDLE thread_snap, DWORD_PTR dwThreadAffinityMask);


//sets the priority of the process to the desiered class
/*
* ABOVE_NORMAL_PRIORITY_CLASS 0x8000
* BELOW_NORMAL_PRIORITY_CLASS 0x4000
* HIGH_PRIORITY_CLASS 0x80
* IDLE_PRIORITY_CLASS 0x40
* NORMAL_PRIORITY_CLASS 0x20
* PROCESS_MODE_BACKGROUND_BEGIN 0x100000
* PROCESS_MODE_BACKGROUND_END 0x200000
* REALTIME_PRIORITY_CLASS 0x100 Xx-*NUKE*-xX {{DO NOT USE IT WITHOUT RESERVING CPU CORES}}
*/
#define Process_Priority_Class HIGH_PRIORITY_CLASS //priority class of the process you want to change
#define Thread_Priority_Class  THREAD_PRIORITY_NORMAL //priority class of the thread you want to change
//#define ProcessID 18096 //process id of the process you want to change the priority of
extern int ProcessID;
const int SUCCESS = 0;
const int ERROR_OCCURRED = 1;
extern wchar_t ProcessName[MAX_PATH]; //process name of the process you want to change the priority of

//variables
extern DWORD dwError, dwPriClass;
extern DWORD_PTR Mask; // 0xE (1110) for the last 3 cores reserved for real time
extern DWORD_PTR processAffinityMask;
extern DWORD_PTR systemAffinityMask;
extern HANDLE hProcess, hThreadSnap, hThread;
extern THREADENTRY32 te32;

#endif HEADER_H