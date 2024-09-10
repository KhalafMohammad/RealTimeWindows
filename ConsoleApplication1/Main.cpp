#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <processthreadsapi.h>
#include <tchar.h>
#include <winbase.h>
#include "newtestclass.h"

#include <TlHelp32.h>






void Get_Process_Handle(DWORD ProcessAccessInfo, DWORD processID);
void Set_Process_Priority(HANDLE hprocess, DWORD priorityclass);
void Set_Process_Affinity(HANDLE process_handle, DWORD_PTR dwProcessAffinityMask);


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



DWORD dwError, dwPriClass;
DWORD_PTR dwProcessAffinityMask = 0x000000000000003; // 0xE (1110) for the last 3 cores reserved for real time
DWORD_PTR processAffinityMask;
DWORD_PTR systemAffinityMask;
HANDLE hProcess, hThreadSnap = INVALID_HANDLE_VALUE, hThread;
THREADENTRY32 te32;


int main(void)
{
	std::cout << "hello there....\n" << std::endl;

	Get_Process_Handle(PROCESS_ALL_ACCESS, ProcessID); //returns the hProcess handle and the processid


	Set_Process_Priority(hProcess, PriorityClass); //sets the priority of the process to the desiered class

	Set_Process_Affinity(hProcess, dwProcessAffinityMask); // set affinity mask (on how many cores do you want the procss to run)

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, ProcessID); //get the thread snapshot
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("CreateToolhelp32Snapshot (of threads) failed with %d\n"), GetLastError());
		return 1;
	}
	else { _tprintf(TEXT("CreateToolhelp32Snapshot (of threads) is successful\n")); }

	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32)) {
		_tprintf(TEXT("Thread32First failed with %d\n"), GetLastError());
		CloseHandle(hThreadSnap);
		return 1;
	}
	else {
		_tprintf(TEXT("Thread32First is successful\n"));
		do {
			if (te32.th32OwnerProcessID == ProcessID) {
				hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID); //open the thread and get the handle
				_tprintf(TEXT("  1   Thread handle   = %p\n"), hThread);
				SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);//sets the priority of the thread to the desiered class
				//SetThreadAffinityMask(hThread, dwProcessAffinityMask); // set affinity mask (on how many cores do you want the procss to run)
				_tprintf(TEXT("  2   THREAD ID      = %lu\n"), te32.th32ThreadID);
				_tprintf(TEXT("  3   Base priority  = %d\n"), te32.tpBasePri);
				_tprintf(TEXT("  4   Delta priority = %d\n"), te32.tpDeltaPri);
				//GetProcessAffinityMask(hThread, &processAffinityMask, &systemAffinityMask);
				//_tprintf(TEXT("     Thread affinity mask = %x\n"), processAffinityMask);
				_tprintf(TEXT("  5   Thread priority = %d\n"), GetThreadPriority(hThread));
				CloseHandle(hThread);
				if (hThread != NULL) {
					SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
				}
				else {
					_tprintf(TEXT("Failed to open thread with ID %lu\n"), te32.th32ThreadID);
				}
				printf("\n");
			}
		} while (Thread32Next(hThreadSnap, &te32));
	}
	// Close the handle to the thread snapshot
	CloseHandle(hThreadSnap);
	CloseHandle(hThread);
	CloseHandle(hProcess);
Cleanup:
	// Clean up
	CloseHandle(hProcess);
	CloseHandle(hThreadSnap);
	CloseHandle(hThread);
	return 0;
}

//Gets the process id + error handling
void Get_Process_Handle(DWORD ProcessAccessInfo, DWORD processID) {
	//asking for process handle and information
	hProcess = OpenProcess(ProcessAccessInfo, FALSE, processID);
	if (hProcess == NULL) {
		_tprintf(TEXT("OpenProcess failed (%d)\n"), GetLastError());
		return;
	}
	else {
		processID = GetProcessId(hProcess);
		_tprintf(TEXT("Current process ID is %lu\n"), processID);
		_tprintf(TEXT("Process handle is %p\n"), hProcess);
	}
}

//sets the priority of the process to the desiered class
void Set_Process_Priority(HANDLE process_handle, DWORD priorityclass) {

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

// set affinity mask (on how many cores do you want the procss to run)
void Set_Process_Affinity(HANDLE process_handle, DWORD_PTR dwProcessAffinityMask) {

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