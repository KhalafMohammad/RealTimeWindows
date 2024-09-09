#include <iostream>
#include <windows.h>
#include <processthreadsapi.h>
#include <tchar.h>
#include <winbase.h>
#include "newtestclass.h"
#include <stdio.h>
#include <TlHelp32.h>

void Get_Process_Handle(DWORD ProcessAccessInfo, DWORD processID);
void Set_Process_Priority(HANDLE hprocess, DWORD priorityclass);
void Set_Process_Affinity(HANDLE process_handle, DWORD_PTR dwProcessAffinityMask);


DWORD dwError, dwPriClass, processid;
DWORD_PTR dwProcessAffinityMask = 0x000000000000001; // 0xE (1110) for the last 3 cores reserved for real time
HANDLE hProcess;

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
#define PriorityClass HIGH_PRIORITY_CLASS
#define ProcessID 15756
DWORD_PTR processAffinityMask;
DWORD_PTR systemAffinityMask;



int main(void)
{
	std::cout << "hello there....\n" << std::endl;
	//asking for process handle and information
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	Get_Process_Handle(PROCESS_ALL_ACCESS, ProcessID); //returns the hProcess handle and the processid


	Set_Process_Priority(hProcess, PriorityClass);

	Set_Process_Affinity(hProcess, dwProcessAffinityMask);

Cleanup:
	// Clean up
	CloseHandle(hProcess);
	;
	return 0;
}

//function to get the process id + error handling
void Get_Process_Handle(DWORD ProcessAccessInfo, DWORD processID) {
	//asking for process handle and information
	hProcess = OpenProcess(ProcessAccessInfo, FALSE, processID);
	if (hProcess == NULL) {
		_tprintf(TEXT("OpenProcess failed (%d)\n"), GetLastError());
		return;
	}
	else {
		processid = GetProcessId(hProcess);
		_tprintf(TEXT("Current process ID is %lu\n"), processid);
		_tprintf(TEXT("Process handle is %p\n"), hProcess);
	}
}


void Set_Process_Priority(HANDLE process_handle, DWORD priorityclass) {
	//sets the priority of the process to the desiered class
	SetPriorityClass(process_handle, priorityclass);
	if (!SetPriorityClass(process_handle, priorityclass)) {
		_tprintf(TEXT("Failed to set priority class (%d)\n"), GetLastError());
	}
	else if (GetPriorityClass(process_handle) == priorityclass) {
		_tprintf(TEXT("Priority class is set to 0x%x\n"), priorityclass);
	}
	else {
		_tprintf(TEXT("Priority class is not set to 0x%x\n"), priorityclass);
	}
}

void Set_Process_Affinity(HANDLE process_handle, DWORD_PTR dwProcessAffinityMask) {
	// set affinity mask (on how many cores do you want the procss to run)
	if (!SetProcessAffinityMask(process_handle, dwProcessAffinityMask)) {
		dwError = GetLastError();
		if (ERROR_INVALID_PARAMETER == dwError) { // ERROR_INVALID_PARAMETER for a not configured peocessor
			//_tprintf(TEXT("mask is cheanged succesfully!"));
			_tprintf(TEXT("ERROR_INVALID Processor configuerd"));
		}
		else {
			_tprintf(TEXT("mask did'nt change :("));
			goto Cleanup;
		}
	}
	else {
		if (!GetProcessAffinityMask(process_handle, &processAffinityMask, &systemAffinityMask))
		{
			_tprintf(TEXT("Failed to get process affinity mask (%d)\n"), GetLastError());
		}
		_tprintf(TEXT("Current process affinity Mask is %x\n"), processAffinityMask);
	}

Cleanup:
	// Clean up
	CloseHandle(process_handle);
}