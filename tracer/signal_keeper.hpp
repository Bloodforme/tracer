#pragma once
#include "signal.hpp"

namespace tracer {

/*!
\brief ���������滻�������õ��� Signal ����

������൱��һ��ȫ�ֵĹ�������, �������� key, ȡ�����������͵� Signal ����.
*/
template<typename T>
struct SignalKeeper {
	static Signal<typename T::Signature> signal;
};
template<typename T>
Signal<typename T::Signature> SignalKeeper<T>::signal;

}	// namespace tracer