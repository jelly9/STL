/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :
 * 文件名 :	HashMap.h
 * 作者 :		肖国栋
 *			1799553128@qq.com
 * 创建日期 :	2017-11-14
 * 版本 :		XXXX.XX.XX
 * 相关文件 :
 * 文件实现功能 :
 * 备注 :
 * 修改记录 :
 *
 */

#ifndef __HASHSET_H__
#define __HASHSET_H__

#include "D:\Github\STL\HashFunc.h"
#include "D:\Github\STL\Hash.h"


template<class Value, class _Hash = __HashFunc<Value>>
class HashSet
{
	typedef Value Key;

	struct _KeyOfValue{
		Key operator()(const Value& value){
			return value;
		}
	};

	typedef HashBucket<Key, Value, _KeyOfValue, _Hash> HB;
	typedef typename HashBucket<Key, Value, _KeyOfValue, _Hash>::Iterator Iterator;

public:
	HashSet(){}

	pair<Iterator, bool> Insert(const Key& key)
	{
		return _hb.InsertUnique(key);
	}

	bool Erase(const Key& key)
	{
		return _hb.Erase(key);
	}
	
	Iterator Find(const Key& key)
	{
		return _hb.Find(key);
	}

protected:
	HB _hb;
};

#endif

#if 1
//测试
void Test()
{
	HashSet<int> hs;
	hs.Insert(1);
	hs.Insert(2);
	hs.Insert(3);
	hs.Insert(4);
	hs.Insert(54);
}

#endif