#include <iostream>
#include <windows.h>
#include <processthreadsapi.h>
#include <tchar.h>
#include <winbase.h>
#include "newtestclass.h"
#include <stdio.h>
#include <TlHelp32.h>

void Get_Process_Handle(DWORD ProcessAccess, DWORD processID);

DWORD dwError, dwPriClass, processid;
DWORD_PTR dwProcessAffinityMask = 0x00000000000000E; // 0xE (1110) for the last 3 cores reserved for real time
HANDLE hProcess;
#define PriorityClass HIGH_PRIORITY_CLASS
#define ProcessID 6972
DWORD_PTR processAffinityMask;
DWORD_PTR systemAffinityMask;



int main(void)
{
	std::cout << "hello there....\n" << std::endl;
	//asking for process handle and information
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	Get_Process_Handle(PROCESS_ALL_ACCESS, ProcessID);

	//sets the priority of the process to the desiered class
	/*ABOVE_NORMAL_PRIORITY_CLASS 0x8000
	* BELOW_NORMAL_PRIORITY_CLASS 0x4000
	* HIGH_PRIORITY_CLASS 0x80
	* IDLE_PRIORITY_CLASS 0x40
	* NORMAL_PRIORITY_CLASS 0x20
	* PROCESS_MODE_BACKGROUND_BEGIN 0x100000
	* PROCESS_MODE_BACKGROUND_END 0x200000
	* REALTIME_PRIORITY_CLASS 0x100 Xx-*NUKE*-xX {{DO NOT USE IT WITHOUT RESERVING CPU CORES}}
	*/
	SetPriorityClass(hProcess, PriorityClass);

	// set affinity mask (on how many cores do you want the procss to run)
	if (!SetProcessAffinityMask(hProcess, dwProcessAffinityMask)) {
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


	//this is for background processing only
	if (PriorityClass == PROCESS_MODE_BACKGROUND_BEGIN) {
		if (!SetPriorityClass(hProcess, PROCESS_MODE_BACKGROUND_BEGIN))
		{
			dwError = GetLastError();
			if (ERROR_PROCESS_MODE_ALREADY_BACKGROUND == dwError)
				_tprintf(TEXT("Already in background mode\n"));
			else _tprintf(TEXT("Failed to enter ABOVE_NORMAL mode (%d)\n"), dwError);
			goto Cleanup;
		}
	}



	// gets process affinity information
	if (!GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask))
	{
		return -1;
	}


	//display the process id and its priority class and affinity mask

	processid = GetProcessId(hProcess);
	_tprintf(TEXT("Current process ID is %lu\n"), processid);

	_tprintf(TEXT("Current process affinity Mask is %llu\n"), processAffinityMask);

	dwPriClass = GetPriorityClass(hProcess);
	_tprintf(TEXT("Current priority class is 0x%x\n"), dwPriClass);




	// for background mode only, if the the is done it gotta end this priority
	//if (!SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END))
	//{
	//    _tprintf(TEXT("Failed to end background mode (%d)\n"), GetLastError());
	//}

	//cleanup when error please ;)
	// 
Cleanup:
	// Clean up
	CloseHandle(hProcess);
	;
	return 0;
}

//function to get the process id + error handling
void Get_Process_Handle(DWORD ProcessAccess, DWORD processID) {
	//asking for process handle and information
	hProcess = OpenProcess(ProcessAccess, FALSE, processID);
	if (hProcess == NULL) {
		_tprintf(TEXT("OpenProcess failed (%d)\n"), GetLastError());
		return;
	}
	else {
		_tprintf(TEXT("Process handle is %p\n"), hProcess);
	}
}


void Set_Process_Priority(DWORD priorityclass) {
	//sets the priority of the process to the desiered class
	SetPriorityClass(hProcess, priorityclass);

}