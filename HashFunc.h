/*
*
* 版权 : 		(c) 2017
* 模块名 :
* 文件名 :		HashFunc.h
* 作者 :		肖国栋
*				1799553128@qq.com
* 创建日期 :	2017-11-4
* 版本 :		XXXX.XX.XX
* 相关文件 :
* 文件实现功能 : 常用Hash函数
* 备注 :
* 修改记录 :
*
*/

#ifndef __HASHFUNC_H__
#define __HASHFUNC_H__
#include <string>
#include <iostream>
using namespace std;

template<class T>
struct __HashFunc
{};
template<>
struct __HashFunc<string>
{
	size_t HashString(const char* str)
	{
		unsigned long h = 0;
		for (; *str; ++str)
			h = 5 * h + *str;

		return size_t(h);
	}
	size_t operator()(const string& str)
	{
		return HashString(str.c_str());
	}
};

template<>
struct __HashFunc<char*>
{
	size_t _HashString(const char* str)
	{
		unsigned long h = 0;
		for (; *str; ++str)
			h = 5 * h + *str;

		return size_t(h);
	}
	size_t operator()(const char* str)
	{
		return _HashString(str);
	}
};
template<>
struct __HashFunc<const char*>
{
	size_t HashString(const char* str)
	{
		unsigned long h = 0;
		for (; *str; ++str)
			h = 5 * h + *str;

		return size_t(h);
	}
	size_t operator()(const char* str)
	{
		return HashString(str);
	}
};
template<>
struct __HashFunc<int>
{
	size_t operator()(const int& x)
	{
		return (size_t)x;
	}
};
template<>
struct __HashFunc<char>
{
	size_t operator()(const char& x)
	{
		return (size_t)x;
	}
};
template<>
struct __HashFunc<long>
{
	size_t operator()(const long& x)
	{
		return (size_t)x;
	}
};
template<>
struct __HashFunc<size_t>
{
	size_t operator()(const size_t& x)
	{
		return (size_t)x;
	}
};

#endif