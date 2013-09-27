/*!
\file
����ļ����������� detours �ļ򵥷�װ����, Install �� Remove
*/
#pragma once
#include <windows.h>

namespace tracer {

LONG InstallImpl(void **target, void *detour);
LONG RemoveImpl(void **target, void *detour);

/*!
\file hook_impl.h
\param [in, out] target ����ʱ, ���ָ��Ӧ��ָ��Ҫ��hook�ĺ���; 
���غ�, ���ָ��ָ�����庯��, �������庯����Ч����ͬ�ڵ���δ��hook��ԭʼ����
\param [in] detour ָ���滻������ָ��
\return ����`NO_ERROR`��ʾ�ɹ�, ���򷵻�һ��������:

- **ERROR_INVALID_BLOCK**

	Ŀ�꺯��̫С, �����ڷŲ�������hook��jmpָ��

- **ERROR_INVALID_HANDLE**

	`target`Ϊ��

- **ERROR_NOT_ENOUGH_MEMORY**

	û���㹻���ڴ�������ɲ���
*/
template<typename T>
LONG Install(T **target, T *detour) {
	return InstallImpl(reinterpret_cast<void**>(target), 
		reinterpret_cast<void*>(detour));
}

/*!
\file hook_impl.h
\param [in, out] target ����ʱ, ���ָ��ָ�����庯��; 
���غ�, ���ָ��ָ�򱻽��hook�ĺ���
\param [in] detour ָ���滻������ָ��
\return ����`NO_ERROR`��ʾ�ɹ�, ���򷵻�һ��������:

- **ERROR_INVALID_BLOCK**

	Ŀ�꺯��̫С, �����ڷŲ�������hook��jmpָ��

- **ERROR_INVALID_HANDLE**

	`target`Ϊ��

- **ERROR_NOT_ENOUGH_MEMORY**

	û���㹻���ڴ�������ɲ���
*/
template<typename T>
LONG Remove(T **target, T *detour) {
	return RemoveImpl(reinterpret_cast<void**>(target), 
		reinterpret_cast<void*>(detour));
}

}	// namespace tracer