#pragma once
#include <type_traits>

namespace tracer {

/*!
\brief ���ݳ�Ա����ָ�����ɶ�Ӧ����ͨ����ǩ��.

�������`HRESULT (__stdcall C::*)(int, double)`, 
�������`HRESULT __stdcall type(C*, int, double)`.
*/
template<typename T>
struct MemberToNormal;

template<typename R, typename C, typename... Args>
struct MemberToNormal<R(C::*)(Args...)> {
	typedef R type(C*, Args...);
};

template<typename R, typename C, typename... Args>
struct MemberToNormal<R(__stdcall C::*)(Args...)> {
	typedef R __stdcall type(C*, Args...);
};

/*!
\brief �ڲ����б�ͷ������һ������

�������`<void __stdcall (int), int&>`, �������`void __stdcall (int&, int)`.  
����������`void`, �򱣳ֲ���
*/
template<typename T1, typename T2>
struct PrependParameter;

template<typename Adding, typename R, typename... Args>
struct PrependParameter<R(Args...), Adding> {
	typedef R type(Adding, Args...);
};

template<typename Adding, typename R, typename... Args>
struct PrependParameter<R __stdcall(Args...), Adding> {
	typedef R __stdcall type(Adding, Args...);
};

template<typename R, typename... Args>
struct PrependParameter<R(Args...), void> {
	typedef R type(Args...);
};

template<typename R, typename... Args>
struct PrependParameter<R __stdcall(Args...), void> {
	typedef R __stdcall type(Args...);
};

/*!
\brief �����в�������Ϊ��Ӧ����������

�������`void(char, int)`, �������`void(char&, int&)`
*/
template<typename T>
struct AllParamsToRef;

template<typename R, typename... Args>
struct AllParamsToRef<R(Args...)> {
	typedef R type(typename std::add_lvalue_reference<Args>::type...);
};

template<typename R, typename... Args>
struct AllParamsToRef<R __stdcall(Args...)> {
	typedef R __stdcall type(typename std::add_lvalue_reference<Args>::type...);
};

/*!
\brief ���ú���ǩ���з���ֵ������

�������`<int(char), void>`, �������`void(char)`
*/
template<typename T1, typename T2>
struct SetResultType;

template<typename Setting, typename R, typename... Args> 
struct SetResultType<R(Args...), Setting> {
	typedef Setting type(Args...);
};

template<typename Setting, typename R, typename... Args>
struct SetResultType<R __stdcall(Args...), Setting> {
	typedef Setting __stdcall type(Args...);
};

/*!
\brief ��__stdcall�ĺ���ǩ����ΪĬ�ϵ���Լ��

�������`<int __stdcall (int)>`, �������`int(int)`
*/
template<typename T>
struct RemoveStdcall {
	typedef T type;
};

template<typename R, typename... Args>
struct RemoveStdcall<R __stdcall(Args...)> {
	typedef R type(Args...);
};

/*!
\brief ��ȡ����ĺ���ǩ���ķ���ֵ����

�������`int(int)`, �������`int`
*/
template<typename T>
struct ResultType;

template<typename R, typename... Args>
struct ResultType<R(Args...)> {
	typedef R type;
};

template<typename R, typename... Args>
struct ResultType<R __stdcall(Args...)> {
	typedef R type;
};

/*!
\brief ������ǩ��ת��Ϊ����ָ��

������ͨ������ǩ��, �� std::add_pointer �Ϳ���תΪ����ָ��, 
���������ģ��ͬʱ���ܱ�֤��Ա����ָ�벻�ᱻת����˫��ָ��.
*/
template<typename T, typename Enable = void> struct FunctionPointer;

template<typename T>
struct FunctionPointer<T, typename std::enable_if<std::is_function<T>::value>::type> {
	typedef typename std::add_pointer<T>::type type;
};

template<typename T>
struct FunctionPointer<T, 
	typename std::enable_if<std::is_member_function_pointer<T>::value>::type> {
	typedef T type;
};

}	// namespace tracer