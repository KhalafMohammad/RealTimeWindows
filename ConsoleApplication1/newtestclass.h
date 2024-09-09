#pragma once

//#include <stdio.h>
//#include <TlHelp32.h>
//#include <iostream>
//#include <windows.h>
//#include <processthreadsapi.h>
//#include <wdm.h>
//#include <Ntddk.h>
//#include <Ntifs.h>
//#include <tchar.h>
//
//
//int priority = THREAD_PRIORITY_ABOVE_NORMAL;
//namespace ConsoleApplication1 {
//    class newtestclass
//    {
//
//        void testfunc() {
//
//            if (NtSetInformationThread(GetCurrentThread(), 14, &priority, ULONG(sizeof(int))) == STATUS_SUCCESS) {
//                _tprintf(TEXT("Thread information is SET!!\n"));
//            }
//            else if (NtSetInformationThread(GetCurrentThread(), 14, &priority, ULONG(sizeof(int))) == STATUS_INFO_LENGTH_MISMATCH) {
//                _tprintf(TEXT("Thread information is mismatched!!\n"));
//            }
//        }
//
//    };
//}