/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		
 * 文件名 :		Map.h
 * 作者 :		肖国栋
 *				1799553128@qq.com
 * 创建日期 :	2017-11-10
 * 版本 :		XXXX.XX.XX
 * 相关文件 :	
 * 文件实现功能 :	map相关操作
 * 备注 : 
 * 修改记录 :
 *
 */

#ifndef __MAP_H__
#define __MAP_H__

#include "D:\Github\STL\RBTree.h"

template<class Key, class Value>
class Map
{
	struct __KeyOfPair
	{
		Key operator()(const pair<Key, Value> kv){
			return kv.first;
		}
	};
	typedef pair<const Key, Value> ValueType;
	typedef RBTree<Key, ValueType, __KeyOfPair> RepType;//容器类型
public:
	typedef typename RepType::Iterator Iterator;
	typedef typename RepType::ConstIterator ConstIterator;
public:
	pair<Iterator, bool> Insert(const ValueType& kv)
	{
		return _t.Insert(kv);
	}

	Iterator Find(const Key& key)
	{
		_t.Find(key);
	}

	Value& operator[](const Key& key)
	{
		return (_t.Insert(make_pair(key, Value())).first)->second;
	}

	//Iterator
	Iterator Begin()
	{
		return _t.Begin();
	}
	ConstIterator Begin()const
	{
		_t.Begin();
	}
	Iterator End()
	{
		return _t.End();
	}
	ConstIterator End()const
	{
		_t.End();
	}

private:
	RepType _t;
};
#endif


#if 1

#include "D:\Github\STL\Function.h"
#include <string>
#include <map>

void test_map()
{
	map<int, string> m;
	m.insert(make_pair(0, "insert"));//调用make_pair()函数，返回一个pair对象
	m.insert(make_pair(1, "delete"));
	m.insert(pair<int, string>(2, "modify"));//用pair结构体构造一个匿名对象作为参数
	m.insert(map<int,string>::value_type(3, "find"));//用map内部typedef出来的类型构造，和上面(87行)的方法类似

	cout << m[0] << endl;
	cout << m[1] << endl;
	cout << m[2] << endl;
	cout << m[3] << endl;

	map<int, string> a;//和数组一样，只不过访问数据的时间复杂度为 O(logN)
	a[0] = "INSERT";
	a[1] = "DELETE";
	a[2] = "MODIFY";
	a[3] = "FIND";
	cout << a[0] << endl;
	cout << a[1] << endl;
	cout << a[2] << endl;
	cout << a[3] << endl;
}

void TestMap()
{
	//int a[100];
	//RandomArrayUnique(a, sizeof(a)/sizeof(int));
	//

	//Map<int, int> m;
	//for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
	//	m.Insert(make_pair(a[i], i));

	//Map<int, int>::Iterator it = m.Begin();
	//while (it != m.End()){
	//	cout << it->first << "->" << it->second << endl;
	//	++it;
	//}

	//Map<int, string> m;
	//m[1] = "insert";
	//m[2] = "delete";
	//m[3] = "modify";
	//m[4] = "find";

	//Map<int, string>::Iterator it = m.Begin();
	//while (it != m.End()){
	//	cout << (*it).first << "->" << it->second.c_str() << endl;
	//	++it;
	//}





	Map<string, string> m;
	m["insert"] = "INSERT";
	m["delete"] = "DELETE";
	m["modify"] = "MODIFY";
	m["find"] = "MODIFY";

	Map<string, string>::Iterator it = m.Begin();
	while (it != m.End()){
		cout << (*it).first << "->" << it->second << endl;
		++it;
	}

	m["find"] = "FIND";

	cout << endl;
	it = m.Begin();
	while (it != m.End()){
		cout << (*it).first << "->" << it->second << endl;
		++it;
	}







	//PrintContainer(m);
	//字符类的输出运算符重载于头文件<string>中
	//string s("Hello world!");
	//cout << s << endl;
}
#endif
