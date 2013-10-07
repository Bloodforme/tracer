#pragma once
#include <boost/preprocessor.hpp>

#define TRACER_FUNCTION_TYPE_LIMIT 15
namespace tracer {

/*!
\brief ���ݳ�Ա����ָ�����ɶ�Ӧ����ͨ����ǩ��.

�������`HRESULT (__stdcall C::*)(int, double)`, 
�������`HRESULT __stdcall type(C*, int, double)`.
*/
template<typename T>
struct MemberToNormal;

/*!
\brief �ڲ����б�ͷ������һ������

�������`<void __stdcall (int), int&>`, �������`void __stdcall (int&, int)`.  
����������`void`, �򱣳ֲ���
*/
template<typename T1, typename T2>
struct PrependParameter;

/*!
\brief �����в�������Ϊ��Ӧ����������

�������`void(char, int)`, �������`void(char&, int&)`
*/
template<typename T>
struct AllParamsToRef;

/*!
\brief ���ú���ǩ���з���ֵ������

�������`<int(char), void>`, �������`void(char)`
*/
template<typename T1, typename T2>
struct SetResultType;

/*!
\brief ��__stdcall�ĺ���ǩ����ΪĬ�ϵ���Լ��

�������`<int __stdcall (int)>`, �������`int(int)`
*/
template<typename T>
struct RemoveStdcall;

/*!
\brief ��ȡ����ĺ���ǩ���ķ���ֵ����

�������`int(int)`, �������`int`
*/
template<typename T>
struct ResultType;

/*!
\brief �۵�����

���������������7��, ��Ὣ��6���Ժ�Ĳ�������һ��`tuple`��Ϊ��7������.  
�������`int(int, int, int, int, int, char, short, int, long, long long)`, �������
`int(int, int, int, int, int, char, std::tuple<short, int, long, long long>)`
*/
template<typename T>
struct FoldParameters;

/*!
\brief ת��Ϊ�۵������ĵ���

����һ�����������۵��Ŀɵ��ö���`func`����Ҫ�������Ĳ���:

- ���`func`��Ҫ�Ĳ�������7��, �����κ��޸�, ֱ�ӽ�����ת������.
- ���`func`��Ҫ�Ĳ�������7��(������Ĳ��ֲ�Ϊһ��`tuple`��Ϊ��7������), ��ǰ6������ֱ��ת��
��`func`, ʣ�µĲ�����Ϊ`tuple`����`func`

\sa FoldParameters
*/
void ForwardToFoldedParameters();

#define BOOST_PP_ITERATION_LIMITS (0, TRACER_FUNCTION_TYPE_LIMIT)
#define BOOST_PP_FILENAME_1 "function_type.cpp"
#include BOOST_PP_ITERATE()

}	// namespace tracer