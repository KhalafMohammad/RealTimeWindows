#pragma once


//#include <processthreadsapi.h>
//#include <wdm.h>
//#include <Ntddk.h>
//#include <Ntifs.h>
//#include <tchar.h>

//namespace ConsoleApplication1 {
//    class newtestclass
//    {
//
//        void testfunc() {
//
//            if (NtSetInformationThread(GetCurrentThread(), ThreadBasePriority, (int*)realtime_tvalue, ULONG(sizeof(int))) == STATUS_SUCCESS) {
//                _tprintf(TEXT("Thread information is SET!!\n"));
//            }
//            else if (NtSetInformationThread(GetCurrentThread(), ThreadBasePriority, (int*)realtime_tvalue, ULONG(sizeof(int))) == STATUS_INFO_LENGTH_MISMATCH) {
//                _tprintf(TEXT("Thread information is mismatched!!\n"));
//            }
//        }
//
//    };
//}