/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		
 * 文件名 :		Set.h
 * 作者 :		肖国栋
 *				1799553128@qq.com
 * 创建日期 :	2017-11-10
 * 版本 :		XXXX.XX.XX
 * 相关文件 :	
 * 文件实现功能 :	Set的实现
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

//求并集	s1 并 s2
template<class InputIterator1, class InputIterator2>
void SetUnion(InputIterator1 first1, InputIterator1 last1, 
		InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2){
		if (*first1 < *first2){
			cout << *first1 << " ";
			++first1;
		}
		else if (*first1 > *first2){
			cout << *first2 << " ";
			++first2;
		}
		else{
			cout << *first1 << " ";
			++first1;
			++first2;
		}
	}

	while (first1 != last1){
		cout << *first1 << " ";
		++first1;
	}

	while (first2 != last2){
		cout << *first2 << " ";
		++first2;
	}
}

//求交集	s1 交 s2
template<class InputIterator1, class InputIterator2>
void SetIntersection(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2){
		if (*first1 < *first2)
			++first1;
		else if (*first1 > *first2)
			++first2;
		else{//交集只有相等才处理
			cout << *first1 << " ";
			++first1;
			++first2;
		}
	}
}

//求差集 s1 - s2
template<class InputIterator1, class InputIterator2>
void SetDifference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2){
		if (*first1 == *first2){
			++first1;
			++first2;
		}
		else if (*first1 > *first2)//
			++first2;
		else{//说明*first只在s1中出现，没有在s2中出现
			cout << *first1 << " ";
			++first1;
		}
	}
}


//求对称差集，出现在s1但不出现在s2中 + 出现在s2但不出现在s1 (s1-s2) 并 (s2 -s1)
template<class InputIterator1, class InputIterator2>
void SetSymmetricDifference(InputIterator1 first1, InputIterator1 last1,//set_symmetric_difference
	InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2){
		if (*first1 < *first2){
			cout << *first1 << " ";
			++first1;
		}
		else if (*first1 > *first2){
			cout << *first2 << " ";
			++first2;
		}
		else{//说明*first在s1和s2中都出现
			++first1;
			++first2;
		}
	}

	while (first1 != last1){
		cout << *first1 << " ";
		++first1;
	}

	while (first2 != last2){
		cout << *first2 << " ";
		++first2;
	}
}
#endif

#if 1
#include "D:\Github\STL\Function.h"
#include "D:\Github\STL\Vector.h"
#include <vector>
void TestSet()
{
	//Set<int> s1;//1 3 6 7 10
	//s1.Insert(10);
	//s1.Insert(3);
	//s1.Insert(7);
	//s1.Insert(1);
	//s1.Insert(6);

	//Set<int> s2;//2 3 6 7 15
	//s2.Insert(7);
	//s2.Insert(3);
	//s2.Insert(15);
	//s2.Insert(2);
	//s2.Insert(6);

	Vector<int> s1;//1 3 6 7 10
	s1.PushBack(1);
	s1.PushBack(3);
	s1.PushBack(6);
	s1.PushBack(7);
	s1.PushBack(10);

	Vector<int> s2;//2 3 6 7 15
	s2.PushBack(2);
	s2.PushBack(3);
	s2.PushBack(6);
	s2.PushBack(7);
	s2.PushBack(15);

	SetSymmetricDifference(s1.Begin(), s1.End(), s2.Begin(), s2.End());
	cout << endl;
	SetUnion(s1.Begin(), s1.End(), s2.Begin(), s2.End());
	cout << endl;
	SetIntersection(s1.Begin(), s1.End(), s2.Begin(), s2.End());
	cout << endl;
	SetDifference(s1.Begin(), s1.End(), s2.Begin(), s2.End());
	cout << endl;
}

#include <set>
#include <algorithm>
void test_set()
{
	//set<int> s1;//1 3 6 7 10
	//s1.insert(10);
	//s1.insert(3);
	//s1.insert(7);
	//s1.insert(1);
	//s1.insert(6);

	//set<int> s2;//2 3 6 7 15
	//s2.insert(7);
	//s2.insert(3);
	//s2.insert(15);
	//s2.insert(2);
	//s2.insert(6);

	vector<int> s1;//1 3 6 7 10
	s1.push_back(10);
	s1.push_back(3);
	s1.push_back(7);
	s1.push_back(1);
	s1.push_back(6);
	sort(s1.begin(), s1.end());

	vector<int> s2;//2 3 6 7 15
	s2.push_back(7);
	s2.push_back(3);
	s2.push_back(15);
	s2.push_back(2);
	s2.push_back(6);
	sort(s2.begin(), s2.end());

	set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));//参数三表示输出处理结果
	cout << endl;
	set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));//3 6 7
	cout << endl;
	set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));//1 10
	cout << endl;
	set_symmetric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));//1 2 10 15
	cout << endl;

	//while (it != s.end()){
	//	cout << *it << " ";
	//	++it;
	//}
}
#endif

