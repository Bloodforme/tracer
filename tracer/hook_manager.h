#pragma once
#include <unordered_map>
#include <mutex>
#include "hook_impl.h"
#include "singleton.hpp"
#include "../google/gtest/gtest_prod.h"

namespace tracer {

/*!
\brief ���ӹ�����

������� InstallImpl �� RemoveImpl �İ�װ, ���¼�����Ѱ�װ�Ĺ��Ӳ���������ʱ�Ƴ�����.
*/
class HookManager : public Singleton<HookManager> {
	/*!
	key��ָ�����庯����ָ���ָ��, value���滻������ָ��
	*/
	std::unordered_map<void**, void*> hooks_;
	std::mutex lock_;

private:
	HookManager() {}
	HookManager(const HookManager&);
	HookManager &operator = (const HookManager &);
	~HookManager() { Clear(); }
	friend class Singleton<HookManager>;

	LONG Install_(void **target, void *detour);
	LONG Remove_(void **target);

public:
	/*!
	\brief ��װ����
	
	��������ֵͬ InstallImpl , Ψһ�Ĳ�ͬ�Ƿ���`ERROR_INVALID_HANDLE`���п�������Ϊ
	`target`�Ѿ�װ�й�����
	*/
	template<typename T>
	LONG Install(T **target, T *detour) {
		return Install_(reinterpret_cast<void**>(target), 
			reinterpret_cast<void*>(detour));
	}

	/*!
	\brief �Ƴ�����
	\param [in, out] target ����ʱӦָ�����庯��, ���غ�ָ��ԭʼ����
	\return ͬ RemoveImpl , Ψһ�Ĳ�ͬ�Ƿ���`ERROR_INVALID_HANDLE`���п�������Ϊ
	`target`��û�а�װ����
	*/
	template<typename T>
	LONG Remove(T **target) {
		return Remove_(reinterpret_cast<void**>(target));
	}

	//! �Ƴ����й���
	void Clear();

	FRIEND_TEST(HookManagerTest, NormalFuncTest);
	FRIEND_TEST(HookManagerTest, DtorTest);
	FRIEND_TEST(HookManagerTest, APITest);
};

}	// namespace tracer
