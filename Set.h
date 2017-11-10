/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		
 * 文件名 :		.h
 * 作者 :		肖国栋
 *				1799553128@qq.com
 * 创建日期 :	2017-11-4
 * 版本 :		XXXX.XX.XX
 * 相关文件 :	
 * 文件实现功能 :	
 * 备注 : 
 * 修改记录 :
 *
 */

#ifndef __SET_H__
#define __SET_H__

#include "D:\Github\STL\RBTree.h"

template<class Key>
class Set
{
	struct _KeyOfValue
	{
		Key operator()(const Key& key)
		{
			return key;
		}
	};
	typedef RBTree<Key, Key, _KeyOfValue> RepType;
public:
	typedef typename RepType::Iterator Iterator;
	typedef typename RepType::ConstIterator ConstIterator;

	pair<Iterator, bool> Insert(const Key& key)
	{
		return _t.Insert(key);
	}

	Iterator Find(const Key& key)
	{
		return _t.Find(key);
	}

	Iterator Begin()
	{
		return _t.Begin();
	}

	ConstIterator Begin()const
	{
		return _t.Begin();
	}

	Iterator End()
	{
		return _t.End();
	}

	ConstIterator End()const
	{
		return _t.End();
	}
private:
	RepType _t;
};

#if 1
#include "D:\Github\STL\Function.h"
void TestSet()
{
	Set<int> st;

	st.Insert(3);
	st.Insert(1);
	st.Insert(2);
	st.Insert(4);

	PrintContainer(st);
}
#endif


#endif