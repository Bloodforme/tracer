#pragma once

namespace tracer {

/*!
\brief ��������

�����Ҫ�� A ʵ�ֵ���ģʽ, ����Ҫ�̳��� Singleton<A> . ����� A �Ĺ��캯�����ǹ�����, ����Ҫ
�ټ�һ�� friend class Singleton<A>;
*/
template <class T> 
class Singleton { 
public: 
	static T& Instance() { 
		static T _instance; 
		return _instance; 
	}
protected: 
	Singleton(void) {} 
	virtual ~Singleton(void) {} 
	Singleton(const Singleton<T>&);
	Singleton<T>& operator= (const Singleton<T> &);
}; 

}	// namespace agt