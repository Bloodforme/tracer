#pragma once

namespace tracer {

/*!
\brief ����ָ�����庯����ָ��.

ģ�����`T`Ӧ����һ����Ϊ`Signature`��typedef, ����������ͨ������ǩ�������ǳ�Ա����ָ��, 
`RealFuncKeeper<>`�Ĺ�����̬���ݳ�Ա`real`�Ǵ����͵ĺ���ָ��.
*/
template<typename T>
struct RealFuncKeeper {
	typedef typename FunctionPointer<typename T::Signature>::type Pointer;
	static Pointer real;
};
template<typename T>
typename RealFuncKeeper<T>::Pointer RealFuncKeeper<T>::real;

}	// namespace tracer