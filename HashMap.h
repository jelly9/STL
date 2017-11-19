/*
*
* 版权 : 		(c) 2017
* 模块名 :
* 文件名 :		HashMap.h
* 作者 :			肖国栋
*				1799553128@qq.com
* 创建日期 :		2017-11-14
* 版本 :			XXXX.XX.XX
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
#include "D:\Github\STL\Function.h"


template<class Key, class Value, class _Hash = __HashFunc<Key>>
class HashMap
{
	//从值域中取得key的方法
	struct _KeyOfPair{
		Key operator()(const pair<Key, Value>& p){
			return p.first;
		}
	};

	//容器类型
	typedef pair<Key, Value> ValueType;
	typedef HashBucket<Key, ValueType, _KeyOfPair, _Hash> HB;

public:
	typedef typename HB::Iterator Iterator;

public:
	HashMap(){}

	pair<Iterator, bool> Insert(const pair<Key, Value>& p)
	{
		return _hb.InsertUnique(p);
	}

	Value& operator[](const Key& key)
	{
		return (_hb.InsertUnique(make_pair(key, Value())).first)->second;
	}

	bool Erase(const Key& key)
	{
		return _hb.Erase(key);
	}

	Iterator Find(const Key& key)
	{
		return _hb.Find(key);
	}

	Iterator Begin()
	{
		return _hb.Begin();
	}

	Iterator End()
	{
		return _hb.End();
	} 

protected:
	HB _hb;
};

#endif

#if 1
//测试
void Test()
{
	//HashMap<int, int> hs;
	//int a[1000] = { 0 };
	//RandomArrayUnique(a, 1000);

	//for (int i = 0; i < 1000; ++i){
	//	hs.Insert(make_pair(a[i], i));
	//	cout << hs.Find(a[i])->second << endl;;
	//}

	//HashMap<int, int>::Iterator it = hs.Begin();
	//while (it != hs.End()){
	//	cout << it->first << "->" << it->second << endl;;
	//	++it;
	//}

	//for (int i = 0; i < 1000; ++i)
	//	hs.Erase(a[i]);

	//hs.Insert(make_pair(1, 1));
	//hs.Insert(make_pair(2, 2));
	//hs.Insert(make_pair(3, 3));
	//hs.Insert(make_pair(4, 4));
	//hs.Insert(make_pair(5, 5));

	HashMap<string, string> dict;

	dict["INSERT"] = "insert";
	dict["DELETE"] = "delete";
	dict["MODIFY"] = "modify";
	dict["FIND"] = "find";

}

#endif