/*
*
* 版权 : 		(c) 2017
* 模块名 :		DS
* 文件名 :		BloomFilter.h
* 作者 :		肖国栋
*				1799553128@qq.com
* 创建日期 :	2017-11-19
* 版本 :		XXXX.XX.XX
* 相关文件 :
* 文件实现功能 : 布隆过滤器实现大数的查找问题，时间复杂度O(1)
* 备注 :
* 修改记录 :
*
*/

#ifndef __BLOOMFILTER_H__
#define __BLOOMFILTER_H__

#pragma once

#include"D:\Github\STL\BitMap.h"

struct __HashFunc_1
{
	size_t HashString(const char* str)
	{
		long seed = 131; // 31 131 1313 13131 131313 etc..  
		unsigned long h = 0;
		for (; *str; ++str)
			h = seed * h + *str;

		return size_t(h);
	}
	size_t operator()(const string& str)
	{
		return HashString(str.c_str());
	}
};

struct __HashFunc_2
{
	size_t HashString(const char* str)
	{
		long seed = 1313; // 31 131 1313 13131 131313 etc..  
		unsigned long h = 0;
		for (; *str; ++str)
			h = seed * h + *str;

		return size_t(h);
	}
	size_t operator()(const string& str)
	{
		return HashString(str.c_str());
	}
};

struct __HashFunc_3
{
	size_t HashString(const char* str)
	{
		long seed = 13131; // 31 131 1313 13131 131313 etc..  
		unsigned long h = 0;
		for (; *str; ++str)
			h = seed * h + *str;

		return size_t(h);
	}
	size_t operator()(const string& str)
	{
		return HashString(str.c_str());
	}
};

template<class Key
	, class _Hash_1 = __HashFunc_1
	, class _Hash_2 = __HashFunc_2
	, class _Hash_3 = __HashFunc_3
>
class BloomFilter
{
public:
	BloomFilter(const size_t n)
		:_bm(n * 10)
		, _size(n * 10)
	{}
	void Set(const Key& key)
	{
		_bm.Set(_Hash_1()(key)%_size);
		_bm.Set(_Hash_2()(key) % _size);
		_bm.Set(_Hash_3()(key) % _size);
	}

	//不在是准确的
	//在是不准确的
	bool Test(const Key& key)
	{
		if (_bm.Test(_Hash_1()(key) % _size)
			&& _bm.Test(_Hash_2()(key) % _size)
			&& _bm.Test(_Hash_3()(key) % _size)
			)
			return true;
		else
			return false;
	}
protected:
	BitMap _bm;
	size_t _size;
};
#endif

#if 0
//测试
void TestBloomFilter()
{
	BloomFilter<string> bf(10);
	bf.Set("hello");
	bf.Set("world");
	bf.Set("bloom");
	bf.Set("filter");

	cout << bf.Test("hello") << endl;
	cout << bf.Test("world") << endl;
	cout << bf.Test("bloom") << endl;
	cout << bf.Test("filter") << endl;

	cout << bf.Test("left") << endl;
	cout << bf.Test("right") << endl;
}

#endif