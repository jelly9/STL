/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		STL
 * 文件名 :		Hash.h
 * 作者 :		肖国栋
 *				1799553128@qq.com
 * 创建日期 :	2017-11-11
 * 版本 :		XXXX.XX.XX
 * 相关文件 :
 * 文件实现功能 :哈希表实现，处理冲突：1.开放定址法（闭散列法） 2.开链法
 * 备注 :
 * 修改记录 :
 *
 */

#ifndef __HASH_H__
#define __HASH_H__

//#include "D:\Github\STL\Vector.h"
//#include <string>
#include <iostream>
using namespace std;

#include <vector>


enum TAG{ EMPTY, EXIST, DELETE };

template<class Value>
struct __HashTableNode
{
	Value _valueField;
	TAG _state;
	__HashTableNode(const Value& valueField)
		:_valueField(valueField)
		: _state(EMPTY)
	{}
	__HashTableNode(){}
};


inline unsigned long __NextPrime(unsigned long n)
{
	//素数表
	static const unsigned long __PrimeList[28] = 
	{
		53,			97,			193,			389,			769,
		1543,		3079,		6151,		12289,		24593,
		49157,		98317,		196613,		393241,		786433,
		1572869,		3145739,		6291469,		12582917,	25165843,
		50331653,	100663319,	201326611,	402653189,	805306457,
		1610612741,	3221225473,	4294967291
	};
	size_t i = 0;
	for (; i < 28; ++i){
		if (__PrimeList[i] > n)
			break;
	}
	return i < 28 ? __PrimeList[i] : __PrimeList[27];
}

template<class T>
struct __Hash
{
	size_t operator()(const T& x)
	{
		return (size_t)x;
	}
};


struct __KeyOfValue
{
	int operator()(const int value){
		return value;
	}
};

template<class Key, class Value, class _KeyOfValue = __KeyOfValue, 
		class _Hash = __Hash<Key>>
class HashTable
{
	typedef __HashTableNode<Value> Node;
public:
	HashTable()
		:_size(0)
	{}
	bool Insert(const Value& value)
	{
		_CheckCapacity();
		size_t hash = HashFunc(_KeyOfValue()(value));
		int idx = hash;
		while (EXIST == _table[idx]._state){
			if (value == _table[idx]._valueField)
				return false;
		
			++idx;
			idx %= _table.size();
		}

		_table[idx]._valueField = value;
		_table[idx]._state = EXIST;
		++_size;
		return true;
	}
protected:
	size_t HashFunc(const Key& key)
	{
		return _Hash()(key) % _table.size();
	}
	void _CheckCapacity()
	{
		if (0 == _size){
			_table.resize(__NextPrime(1));
			return;
		}
		//负载因子超过0.7则扩容
		else if ((_size * 10) / _table.size() > 7){
			HashTable<Key, Value, _KeyOfValue, _Hash> newTable;
			newTable._table.resize(__NextPrime(_table.size()));
			for (size_t i = 0; i < _table.size(); ++i){
			if (EXIST == _table[i]._state)
				newTable.Insert(_table[i]._valueField);
			}
			_table.swap(newTable._table);
		}
		
	}
protected:
	vector<Node> _table;
	size_t _size;
};


#if 1
//测试
#include "D:\Github\STL\Function.h"
#include <vector>
void Test()
{
	HashTable<int, int> ht;
	ht.Insert(1);
	ht.Insert(16);
	ht.Insert(25);
	ht.Insert(3);
	ht.Insert(9);

	//int a[100];
	//RandomArrayUnique(a, sizeof(a)/sizeof(int));

	//HashTable<int, int> ht;
	//for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
	//	ht.Insert(a[i]);

}

#endif
#endif