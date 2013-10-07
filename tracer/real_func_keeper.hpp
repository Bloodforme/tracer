#pragma once

namespace tracer {

/*!
\brief ����ָ�����庯����ָ��.

ģ�����`T`Ӧ����һ����Ϊ`Signature`��typedef, ����������ͨ������ǩ�������ǳ�Ա����ָ��, 
`RealFuncKeeper<>`�Ĺ�����̬���ݳ�Ա`real`�Ǵ����͵ĺ���ָ��.
*/
template<typename T>
struct RealFuncKeeper {
	template<typename S>
	static S *Pointer_(typename ::std::enable_if<
		!::std::is_member_function_pointer<S>::value>::type*);
	template<typename S>
	static S Pointer_(typename ::std::enable_if<
		::std::is_member_function_pointer<S>::value>::type*);
	typedef decltype(Pointer_<typename T::Signature>(0)) Pointer;
	static Pointer real;
};
template<typename T>
typename RealFuncKeeper<T>::Pointer RealFuncKeeper<T>::real;

}	// namespace tracer