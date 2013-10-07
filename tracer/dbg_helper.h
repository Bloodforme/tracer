#pragma once
#include <windows.h>
#include <mutex>
#include <vector>
#include <DbgHelp.h>
#include "singleton.hpp"

namespace tracer {

class StackWalkIterator;

//! ��װ��DbgHelpe.h��һЩ����
class DbgHelper : public Singleton<DbgHelper> {
	std::mutex lock_;
	PSYMBOL_INFO sym_buffer_;
	IMAGEHLP_LINE64 line_info_;

private:
	//! ƴ�ӷ����ļ�������ַ
	std::string GetSymSearchPath_();
	//! ����ģ����Ϣ
	void LoadModules_();
	//! ��ʼ�����Է���
	void Init_();
	//! ������Է�����Դ
	bool Cleanup_();
	//! ����ָ����С�ķ��Žṹ
	void AllocSymBuffer_(std::size_t size);
	DbgHelper();
	~DbgHelper();
	DbgHelper(const DbgHelper&);
	const DbgHelper& operator = (const DbgHelper&);
	friend class Singleton<DbgHelper>;
	friend class StackWalkIterator;

public:
	//! ��������ջ
	StackWalkIterator StackWalk(CONTEXT *context = nullptr);
	//! ���ݵ�ַ��ȡ������
	std::string GetSymbolName(DWORD64 addr);
	//! ȥ������������
	std::string UnDecorateSymbolName(const std::string symbol_name, DWORD flag = UNDNAME_COMPLETE);
	//! ���ݵ�ַ��ȡ���ŵ��ļ���
	std::string GetFileName(DWORD64 addr);
	//! ���ݵ�ַ��ȡ���ŵ��к�
	DWORD GetLine(DWORD64 addr);
};

//! ��������ջ�ĵ�����
class StackWalkIterator {
	STACKFRAME64 sf_;
	CONTEXT *context_;
	std::mutex &lock_;

	typedef void (StackWalkIterator::*Bool_)() const;
	void CannotCompare_() const {}

public:
	StackWalkIterator(CONTEXT *context, std::mutex &lock);
	StackWalkIterator &operator ++ ();
	StackWalkIterator operator ++ (int);
	const STACKFRAME64 &operator * () const;
	const STACKFRAME64 *operator -> () const;
	operator Bool_() const;
};

}	// namespace tracer