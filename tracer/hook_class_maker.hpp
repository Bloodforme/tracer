/*!
\file 
����������Hook��װ��ĺ�
*/
#pragma once
#include <boost/preprocessor.hpp>
#include <type_traits>
#include "hook_manager.h"
#include "function_type.hpp"
#include "signal.hpp"

#define TRACER_HOOK_NORMAL 0
#define TRACER_HOOK_COM 1
#ifndef TRACER_HOOK_ARG_LIMIT
#define TRACER_HOOK_ARG_LIMIT 15
#endif

#define TRACER_HOOK_FAKE_FUNC_PARAMS(z, n, _)									\
	BOOST_PP_CAT(P, n) BOOST_PP_CAT(p, n)

#define TRACER_HOOK_FULL_NAME(scope, var)										\
	BOOST_PP_CAT(scope, BOOST_PP_CAT(::, var))

//! `TRACER_HOOK_FAKE_FUNCS`�ĸ�����, ���������з���ֵ���޷���ֵ������
#define TRACER_HOOK_FAKE_FUNCS__(n, cc)											\
	template<typename R															\
		BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P)>				\
	static auto fake(R cc (BOOST_PP_ENUM_PARAMS(n, P)))	->						\
	typename ::std::enable_if<!::std::is_void<R>::value,						\
		R (cc *)(BOOST_PP_ENUM_PARAMS(n, P))>::type {							\
		return [] (BOOST_PP_ENUM(n, TRACER_HOOK_FAKE_FUNC_PARAMS, _)) {			\
			bool call_ori = true;												\
			R ret;																\
			::tracer::ForwardToFoldedParameters(::std::ref(before_signal),		\
				call_ori BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p));		\
			if (call_ori)														\
				ret = real(BOOST_PP_ENUM_PARAMS(n, p));							\
			::tracer::ForwardToFoldedParameters(::std::ref(after_signal),		\
				call_ori, ret BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p));	\
			return ret;															\
		};																		\
	}																			\
	template<typename R															\
		BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P)>				\
	static auto fake(R cc (BOOST_PP_ENUM_PARAMS(n, P)))	->						\
	typename ::std::enable_if<::std::is_void<R>::value,							\
	R (cc *)(BOOST_PP_ENUM_PARAMS(n, P))>::type {								\
		return [] (BOOST_PP_ENUM(n, TRACER_HOOK_FAKE_FUNC_PARAMS, _)) {			\
			bool call_ori = true;												\
			::tracer::ForwardToFoldedParameters(::std::ref(before_signal),		\
				call_ori BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p));		\
			if (call_ori)														\
				real(BOOST_PP_ENUM_PARAMS(n, p));								\
			::tracer::ForwardToFoldedParameters(::std::ref(after_signal),		\
				call_ori BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, p));		\
		};																		\
	}

//! `TRACER_HOOK_FAKE_FUNCS`�ĸ�����, ��������Ĭ�ϵ���Լ����`__stdcall`������
#define TRACER_HOOK_FAKE_FUNCS_(z, n, _)										\
	TRACER_HOOK_FAKE_FUNCS__(n, BOOST_PP_EMPTY())								\
	TRACER_HOOK_FAKE_FUNCS__(n, __stdcall)											

/*!
\brief ����fake����

��Ϊ�ں��в�֪��ԭʼ������ǩ��, ����û��ֱ�����ɺ���ǩ��һ�µ�fake����, 
��������ɵ�ʵ������һ�������. �������ض����յĶ���һ������������ͬ�ĺ���ָ��, 
ÿ���������������ķ�����, �ֱ��Ӧ Ĭ�ϵ���Լ�� ��`__stdcall`�Լ�����`void`�����.
ÿ�����ض��᷵��һ������ǩ��һ�µ�lambdaת�������ĵĺ���ָ��.
*/
#define TRACER_HOOK_FAKE_FUNCS													\
	BOOST_PP_REPEAT(TRACER_HOOK_ARG_LIMIT, TRACER_HOOK_FAKE_FUNCS_, _)

//! ����Before signal��ص������Լ�signal��������
#define TRACER_HOOK_BEFORE_SIGNAL_DECL											\
	typedef																		\
		::tracer::FoldParameters<												\
			::tracer::AllParamsToRef<											\
				::tracer::PrependParameter<										\
					::tracer::SetResultType<Signature, void>::type,				\
					bool														\
				>::type															\
			>::type																\
		>::type BeforeCallbackSignature;										\
	typedef ::std::function<BeforeCallbackSignature> BeforeCallback;			\
	typedef ::tracer::Signal<BeforeCallbackSignature> BeforeSignal;				\
	static BeforeSignal before_signal;

//! ����After signal��ص������Լ�signal��������
#define TRACER_HOOK_AFTER_SIGNAL_DECL											\
	typedef																		\
		::tracer::FoldParameters<												\
			::tracer::PrependParameter<											\
				::tracer::AllParamsToRef<										\
					::tracer::PrependParameter<									\
						::tracer::SetResultType<Signature, void>::type,			\
						::tracer::ResultType<Signature>::type					\
					>::type														\
				>::type,														\
				bool															\
			>::type																\
		>::type AfterCallbackSignature;											\
	typedef ::std::function<AfterCallbackSignature> AfterCallback;				\
	typedef ::tracer::Signal<AfterCallbackSignature> AfterSignal;				\
	static AfterSignal after_signal;

//! ������Ϣ�����ݳ�Ա����
#define TRACER_HOOK_INFO_CLASS_DEF(_, class_name, func_name)					\
	class_name::BOOST_PP_CAT(func_name, Info)::OriSignature *					\
		class_name::BOOST_PP_CAT(func_name, Info)::real = nullptr;				\
	class_name::BOOST_PP_CAT(func_name, Info)::BeforeSignal						\
		class_name::BOOST_PP_CAT(func_name, Info)::before_signal;				\
	class_name::BOOST_PP_CAT(func_name, Info)::AfterSignal						\
		class_name::BOOST_PP_CAT(func_name, Info)::after_signal;

//! ����Unhookʵ��
#define TRACER_HOOK_UNHOOK_FUNC(z, _, name)										\
	static LONG name() {														\
	return ::tracer::HookManager::Instance().Remove(							\
	&BOOST_PP_CAT(name, Info)::real);											\
	}

//! ����һ��After�ص���
#define TRACER_HOOK_AFTER_CLASS(z, _, name)										\
	struct name {																\
		static ::tracer::Connection Call(										\
		BOOST_PP_CAT(name, Info)::AfterCallback cb) {							\
			return BOOST_PP_CAT(name, Info)::after_signal.connect(cb);			\
		}																		\
		static ::tracer::Connection CallOnce(									\
		BOOST_PP_CAT(name, Info)::AfterCallback cb) {							\
			return BOOST_PP_CAT(name, Info)::after_signal.once(cb);				\
		}																		\
	};

//! ����һ��Before�ص���
#define TRACER_HOOK_BEFORE_CLASS(z, _, name)									\
	struct name {																\
		static ::tracer::Connection Call(										\
		BOOST_PP_CAT(name, Info)::BeforeCallback cb) {							\
			return BOOST_PP_CAT(name, Info)::before_signal.connect(cb);			\
		}																		\
		static ::tracer::Connection CallOnce(									\
		BOOST_PP_CAT(name, Info)::BeforeCallback cb) {							\
			return BOOST_PP_CAT(name, Info)::before_signal.once(cb);			\
		}																		\
	};

/*!
\brief ��������Hook��ͨ��������Ϣ��

������������Hook�����������������, ȫ�����Ǿ�̬��, ����:

- ָ�����庯���ĺ���ָ��`real`
- �����滻ԭʼ�����ĳ�Ա����`fake`. ʵ���ϲ�ֻ�ǳ�Ա������ô��, Ϊ�˲�������δ֪��
����ǩ��(����ں���˵), ����һЩhack.
- ������ԭʼ��������ǰ�����Ļص���signal����`before_signal`
- ������ԭʼ�������ú󴥷��Ļص���signal����`after_signal`
*/
#define TRACER_HOOK_NORMAL_INFO_CLASS_DECL(_, scope, name)						\
	struct BOOST_PP_CAT(name, Info) {											\
		typedef decltype(TRACER_HOOK_FULL_NAME(scope, name)) OriSignature;		\
		typedef ::tracer::RemoveStdcall<OriSignature>::type Signature;			\
		static OriSignature *real;												\
		TRACER_HOOK_FAKE_FUNCS													\
		TRACER_HOOK_BEFORE_SIGNAL_DECL											\
		TRACER_HOOK_AFTER_SIGNAL_DECL											\
	};

//! ���������ͨ������Hookʵ��
#define TRACER_HOOK_NORMAL_HOOK_FUNC(_, scope, name)							\
	static LONG name() {														\
		if (!BOOST_PP_CAT(name, Info)::real)									\
			BOOST_PP_CAT(name, Info)::real = TRACER_HOOK_FULL_NAME(scope, name);\
		return ::tracer::HookManager::Instance().Install(						\
			&BOOST_PP_CAT(name, Info)::real,									\
			BOOST_PP_CAT(name, Info)::fake(										\
				reinterpret_cast<												\
					BOOST_PP_CAT(name, Info)::OriSignature*>(NULL)));			\
}

/*!
\brief ����Hook��ͨ�����ķ�װ�������
\param [in] class_name Ҫ���ɵ��������
\param [in] scope �������ڵ������ռ�, ����`foo::bar`.
\param [in] func_list ÿ��Ԫ��ӦΪһ��������
*/
#define TRACER_HOOK_NORMAL_DECL(class_name, scope, func_list)					\
	class class_name {															\
		BOOST_PP_SEQ_FOR_EACH(													\
			TRACER_HOOK_NORMAL_INFO_CLASS_DECL, scope, func_list)				\
	public:																		\
		struct Hook {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_NORMAL_HOOK_FUNC, scope, func_list)					\
		};																		\
		struct Unhook {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_UNHOOK_FUNC, _, func_list)							\
		};																		\
		struct Before {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_BEFORE_CLASS, _, func_list)							\
		};																		\
		struct After {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_AFTER_CLASS, _, func_list)							\
		};																		\
	};																			

//! ����Hook��ͨ�����ķ�װ��Ķ���
#define TRACER_HOOK_NORMAL_DEF(class_name, func_list)							\
	BOOST_PP_SEQ_FOR_EACH(TRACER_HOOK_INFO_CLASS_DEF, class_name, func_list)

/*!
\brief ����Hook COM��������Ϣ��Ķ���

����ͨ��������Ϣ�����, COM�����Ķ���һ��`index`��Ա, ��ʾ�������������е����
*/
#define TRACER_HOOK_COM_INFO_CLASS_DECL(_, scope, elem)							\
	struct BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, elem), Info) {				\
		typedef ::tracer::COMToNormal<decltype(&TRACER_HOOK_FULL_NAME(scope,	\
			BOOST_PP_TUPLE_ELEM(2, 0, elem)))>::type OriSignature;				\
		typedef ::tracer::RemoveStdcall<OriSignature>::type Signature;			\
		static OriSignature *real;												\
		static const ::std::size_t index = BOOST_PP_TUPLE_ELEM(2, 1, elem);		\
		TRACER_HOOK_FAKE_FUNCS													\
		TRACER_HOOK_BEFORE_SIGNAL_DECL											\
		TRACER_HOOK_AFTER_SIGNAL_DECL											\
	};

#define TRACER_HOOK_COM_HOOK_FUNC_(name)										\
	static LONG name() {														\
		if (!BOOST_PP_CAT(name, Info)::real)									\
			BOOST_PP_CAT(name, Info)::real = reinterpret_cast<decltype(			\
				BOOST_PP_CAT(name, Info)::real)>(								\
					vmt()[BOOST_PP_CAT(name, Info)::index]);					\
		return ::tracer::HookManager::Instance().Install(						\
			&BOOST_PP_CAT(name, Info)::real,									\
			BOOST_PP_CAT(name, Info)::fake(										\
				reinterpret_cast<												\
					BOOST_PP_CAT(name, Info)::OriSignature*>(NULL)));			\
	}
//! �������COM������Hookʵ��
#define TRACER_HOOK_COM_HOOK_FUNC(z, _, elem)									\
	TRACER_HOOK_COM_HOOK_FUNC_(BOOST_PP_TUPLE_ELEM(2, 0, elem))

#define TRACER_HOOK_COM_UNHOOK_FUNC(z, _, elem)									\
	TRACER_HOOK_UNHOOK_FUNC(z, _, BOOST_PP_TUPLE_ELEM(2, 0, elem))

#define TRACER_HOOK_COM_BEFORE_CLASS(z, _, elem)								\
	TRACER_HOOK_BEFORE_CLASS(z, _, BOOST_PP_TUPLE_ELEM(2, 0, elem))

#define TRACER_HOOK_COM_AFTER_CLASS(z, _, elem)									\
	TRACER_HOOK_AFTER_CLASS(z, _, BOOST_PP_TUPLE_ELEM(2, 0, elem))

/*!
\brief ����Hook COM�����ķ�װ������

Hook COM�����ķ�װ�����һ������ľ�̬����`vmt()`, ���ؽӿڵ����ָ��.
\param [in] class_name Ҫ���ɵ��������
\param [in] scope Ӧ�þ�ȷ����, ����`::IDirect3D9`. 
\param [in] func_list ����ÿ��Ԫ��Ӧ����һ��������Ԫ�ص�tuple, ���е�һ��Ԫ���Ǻ�����, 
�ڶ���Ԫ������������е����, ����`((func1, 0))((func2, 1))((func3, 2))`
*/
#define TRACER_HOOK_COM_DECL(class_name, scope, func_list)						\
	class class_name {															\
		static void **vmt_;														\
		static scope *GetInterfacePointer_();									\
		BOOST_PP_SEQ_FOR_EACH(													\
			TRACER_HOOK_COM_INFO_CLASS_DECL, scope, func_list)					\
	public:																		\
		static void **vmt() {													\
			if (!vmt_)															\
				vmt_ = *reinterpret_cast<void***>(GetInterfacePointer_());		\
			return vmt_;														\
		}																		\
		struct Hook {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_COM_HOOK_FUNC, _, func_list)						\
		};																		\
		struct Unhook {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_COM_UNHOOK_FUNC, _, func_list)						\
		};																		\
		struct Before {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_COM_BEFORE_CLASS, _, func_list)						\
		};																		\
		struct After {															\
			BOOST_PP_SEQ_FOR_EACH(												\
				TRACER_HOOK_COM_AFTER_CLASS, _, func_list)						\
		};																		\
	};																			

#define TRACER_HOOK_COM_INFO_CLASS_DEF(_, class_name, elem)						\
	TRACER_HOOK_INFO_CLASS_DEF(_, class_name, BOOST_PP_TUPLE_ELEM(2, 0, elem))

/*!
\brief ����Hook COM�����ķ�װ��Ķ���

��������Ӧ�ø�һ��������, ��������Ӧ����Ҫ�õ��Ľӿ�ָ��, ����

	TRACER_HOOK_COM_DEF(IDirect3D9Hook, ((GetDeviceCaps, 14))) { 
		return Direct3DCreate9(D3D_SDK_VERSION);
	}
*/
#define TRACER_HOOK_COM_DEF(class_name, func_list)								\
	BOOST_PP_SEQ_FOR_EACH(TRACER_HOOK_COM_INFO_CLASS_DEF, class_name, func_list)\
	void **class_name::vmt_ = nullptr;											\
	::std::result_of<decltype(&class_name::GetInterfacePointer_)()>::type		\
	class_name::GetInterfacePointer_()

//! ����δ֪��categoryʱ��չ�������
#define TRACER_HOOK_ERROR_DECL(a, b, c) static_assert(false, "δ֪Hook����! ����TRACER_HOOK_CLASS_DECL�ĵڶ�������");

/*!
\brief ����Hook��װ������

���ڱ�Hook�ĺ����Ĳ�ͬ����, ���ɵ���֮�����в��, �����������ĸ�������Ƕ����:

- Hook: ����ÿ����Hook�ĺ���, ������ж���һ��ͬ���ľ�̬����, ���������ɹ��Ϲ���
- Unhook: ����ÿ����Hook�ĺ���, ������ж���һ��ͬ���ľ�̬����, ����������ж�ع���
- Before: ����ÿ����Hook�ĺ���, ������ж���һ��������ͬ��Ƕ����, ������������̬����
	- Call: ����һ���ص�, �ص�������ԭʼ��������֮ǰ������; ����һ��
	`tracer::Connection`����, ��������ص����������. 
	- CallOnce: ��`Call`����, ���ǻص�������һ�κ�ͻ��Զ��Ͽ�������. 
- After: ����ÿ����Hook�ĺ���, ������ж���һ��������ͬ��Ƕ����, ������������̬����
	- Call: ����һ���ص�, �ص�������ԭʼ��������֮�󱻵���; ����һ��
	`tracer::Connection`����, ��������ص����������. 
	- CallOnce: ��`Call`����, ���ǻص�������һ�κ�ͻ��Զ��Ͽ�������. 

\param [in] class_name Ҫ���ɵķ�װ�������
\param [in] category ҪHook�ĺ�������:

- `TRACER_HOOK_NORMAL`��ʾҪHook�Ķ�����ͨ����, ʹ�� TRACER_HOOK_NORMAL_DECL ��������
- `TRACER_HOOK_COM`��ʾҪHook�Ķ���COM����, ʹ�� TRACER_HOOK_COM_DECL ��������

\param [in] scope ��������������, ����Ҫ�����������
\param [in] func_list ҪHook�ĺ����б�, ��һ��SEQ�ṹ(`(a)(b)(c)`), Ԫ���������������
\sa TRACER_HOOK_NORMAL_DECL, TRACER_HOOK_COM_DECL
*/
#define TRACER_HOOK_CLASS_DECL(class_name, category, scope, func_list)			\
	BOOST_PP_IIF(BOOST_PP_EQUAL(category, TRACER_HOOK_NORMAL),					\
		TRACER_HOOK_NORMAL_DECL,												\
		BOOST_PP_IIF(BOOST_PP_EQUAL(category, TRACER_HOOK_COM),					\
			TRACER_HOOK_COM_DECL,												\
			TRACER_HOOK_ERROR_DECL))(class_name, scope, func_list)

//! ����δ֪��categoryʱ��չ�������
#define TRACER_HOOK_ERROR_DEF(a, b) static_assert(false, "δ֪Hook����! ����TRACER_HOOK_CLASS_DECL�ĵڶ�������");

/*!
\brief ����Hook��װ�ඨ��
\param [in] class_name ��װ�������
\param [in] category Hook����������:

- `TRACER_HOOK_NORMAL`��ʾҪHook�Ķ�����ͨ����, ʹ�� TRACER_HOOK_NORMAL_DEF ���ɶ���
- `TRACER_HOOK_COM`��ʾҪHook�Ķ���COM����, ʹ�� TRACER_HOOK_COM_DEF ���ɶ���

\param [in] func_list ����`TRACER_HOOK_CLASS_DECL`�ĺ����б�
\sa TRACER_HOOK_NORMAL_DEF, TRACER_HOOK_COM_DEF
*/
#define TRACER_HOOK_CLASS_DEF(class_name, category, func_list)					\
	BOOST_PP_IIF(BOOST_PP_EQUAL(category, TRACER_HOOK_NORMAL),					\
		TRACER_HOOK_NORMAL_DEF,													\
		BOOST_PP_IIF(BOOST_PP_EQUAL(category, TRACER_HOOK_COM),					\
			TRACER_HOOK_COM_DEF,												\
			TRACER_HOOK_ERROR_DEF))(class_name, func_list)