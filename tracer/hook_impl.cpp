#include "hook_impl.h"
#include "detours/detours.h"
#include <TlHelp32.h>

namespace tracer {
namespace {

/*!
DetourUpdateThread ���������������ύ֮ǰ����ָ���߳�, 
���������ö�������̲߳�һһ���� DetourUpdateThread �������.
��Ϊ�����ύʱ���д����޸ĵĲ���, ��������������߳�, ���п�����
�߳���Ϊ�պ�ִ�е��޸���һ��Ĵ��������.
*/
void DetourUpdateAllThreads()
{
	HANDLE snap_handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());
	if (INVALID_HANDLE_VALUE != snap_handle)
	{
		THREADENTRY32 te = {sizeof(te)};
		BOOL ok = Thread32First(snap_handle, &te);
		for (; ok; ok = Thread32Next(snap_handle, &te))
		{
			if (GetCurrentProcessId() == te.th32OwnerProcessID && GetCurrentThreadId() != te.th32ThreadID)
			{
				HANDLE thread_handle = OpenThread(THREAD_ALL_ACCESS, 
					FALSE, te.th32ThreadID);
				if (thread_handle)
				{
					DetourUpdateThread(thread_handle);
					CloseHandle(thread_handle);
				}
			}
		}
		CloseHandle(snap_handle);
	}
}

}

LONG InstallImpl( void **target, void *detour ) {
	DetourTransactionBegin();
	DetourUpdateAllThreads();
	DetourAttach(target, detour);
	return DetourTransactionCommit();
}

LONG RemoveImpl( void **target, void *detour ) {
	DetourTransactionBegin();
	DetourUpdateAllThreads();
	DetourDetach(target, detour);
	return DetourTransactionCommit();
}

}	// namespace tracer