#ifndef __LIST_H__
#define __LIST_H__

#define TEST 1

#include <iostream>
using namespace std;

#include<assert.h>

//list节点
template<class T>
struct ListNode
{
	ListNode(const T& data)
	:_data(data)
	, _next(NULL)
	, _prev(NULL)
	{}
	T _data;
	ListNode<T> *_next;
	ListNode<T> *_prev;
};

//迭代器
template<class T, class Ref, class Ptr>
struct ListIterator
{
	typedef ListNode<T> Node;
	typedef ListIterator<T, Ref, Ptr> Self;
	ListIterator()//construction
		:_node(NULL)
	{}
	ListIterator(Node *node)
		:_node(node)
	{}
	ListIterator(Self& it)
		:_node(it._node)
	{}
	//operator
	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}
	Self operator++(int)
	{
		Self temp(_node);
		_node_ = _node->_next;
		return temp;
	}

	Self& operator--()
	{
		_node = _node->_prev;
		return *this;
	}
	Self operator--(int)
	{
		Self temp(_node);
		_node_ = _node->_prev;
		return temp;
	}

	bool operator==(const Self& it)
	{
		return _node == it._node;
	}
	bool operator!=(const Self& it)
	{
		return _node != it._node;
	}

	Node *_node;
};

//带头结点的双向链表类
template<class T>
class List
{
	typedef ListNode<T> Node;

public:
	typedef ListIterator<T, T&, T*> Iterator;
	//typedef ListIterator<const T, const T&, const T*> ConstIterator;
	/////////////////////////////////////
public:
	//construction
	List()//无参
	{
		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;
	}

	List(size_t n, const T& value)//用n个value初构造链表
	{
		assert(0 != n);

		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;
		for (size_t i = 0; i < n; ++i)
			PushBack(value);
	}

	List(Iterator first, Iterator last)//用first-->last区间的值构造链表
	{
		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;
		while (first != last){
			PushBack(*first);
			++first;
		}
	}
	List( List<T>& l)//拷贝构造
	{
		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;

		List<T>::Iterator it = l.Begin();
		while (it != l.End()){ 
			PushBack(*it);
			++it;
		}
	}
	/////////////////////////////////
	//Iterator
	Iterator Begin()
	{
		return _head->_next;
	}
	//ConstIterator CBegin()const
	//{
	//	return _head->_next;
	//}

	Iterator End()
	{
		return _head;
	}
	//ConstIterator CEnd()const
	//{
	//	return _head;
	//}
	/////////////////////////////////////////
	//modifiers
	void PushBack(const T& value)
	{
		Node *temp = new Node(value);

		temp->_next = _head;
		temp->_prev = _head->_prev;

		_head->_prev->_next = temp;
		_head->_prev = temp;
	}
	void PopBack()
	{}

	Iterator Insert(Iterator position, const T& value)
	{
		Node *cur = position._node;
		Node *next = cur->_next;
		Node *node = new Node(value);

		cur->_next = node;
		node->_prev = cur;

		node->_next = next;
		next->_prev = node;

		return node;
	}

	//迭代器失效？？？
	Iterator Erase(Iterator& position)
	{
		assert(position._node != _head);

		Node* del = position._node;
		Node* prev = del->_prev;
		
		prev->_next = del->_next;
		del->_next->_prev = prev;

		delete del;
		position = prev;
		return prev;
	}

	/////////////////////
	//operations
	void Remove(const T& value)
	{
		Iterator it = Begin();
		while (it != End()){
			if (*it == value){
				Node* prev = it._node->_prev;

				prev->_next = it._node->_next;
				it._node->_next->_prev = prev;

				delete it._node;
				it = prev;
			}
			++it;
		}
	}

	void Unique(const T& value)
	{
		Iterator it1 = Begin();
		Iterator it2 = ++Begin();
		while (it2 != End()){
			if (it1._node->_data == it2._node->_data){
				Erase(it2);
				++it2;
			}
			else{
				++it1;
				++it2;
			}
		}
	}

protected:
	Node *_head;
};

#if TEST

//？？？ConstIterator的问题？？？
void PrintList( List<int>& l)
{
	List<int>::Iterator it1 = l.Begin();
	while (it1 != l.End()){
		cout << *it1 << ' ';
		++it1;
	}
	cout << endl;
}
void TestList()
{
	List<int> l1;
	l1.PushBack(1);
	l1.PushBack(2);
	l1.PushBack(3);
	l1.PushBack(3);
	l1.PushBack(3);
	l1.PushBack(4);
	l1.PushBack(3);
	l1.PushBack(3);
	l1.PushBack(3);
	PrintList(l1);

	//l1.Unique(3);
	//l1.Remove(30);
	List<int>::Iterator it1 = l1.Begin();
	while (it1 != l1.End()){
		if (*it1 % 2 == 0)
			l1.Erase(it1);
		++it1;
	}
	PrintList(l1);

	List<int> l2(10, 5);
	PrintList(l2);


	List<int> l3(++l1.Begin(), l1.End());
	l3.Insert(l3.Begin(), 10);
	PrintList(l3);

	List<int> l4(l3);
	PrintList(l4);

}

#include <list>
void test_list()
{
	list<int> l;
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_back(4);

	list<int>::const_iterator it = l.cbegin();
	while (it != l.end()){
		cout << *it << ' ';
		++it;
	}
}
#endif

#endif


