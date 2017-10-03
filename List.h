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
	//construction
	ListIterator()
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

	bool operator==(Self& it)
	{
		return _node == it._node;
	}
	bool operator!=(Self& it)
	{
		return _node != it._node;
	}

	Node *_node;
};

//带头结点的双向链表类
template<class T>
class List
{
public:
	typedef ListNode<T>		Node;
	typedef T&				Ref;
	typedef const T&		ConstRef;
	typedef ListIterator<T, T&, T*>				Iterator;
	typedef ListIterator<T, const T&, const T*> ConstIterator;
	/////////////////////////////////////
public:
	//construction
	List()//无参
	{
		_size = 0;
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
		for (; first != last; ++first)
			PushBack(*first);
	}
	List( List<T>& l)//拷贝构造
	{
		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;

		List<T>::Iterator it = l.Begin();
		for (; it != l.End(); ++it)
			PushBack(*it);
	}

	////////////////////////////////
	Ref Back()
	{
		return *(--End());
	}

	ConstRef Back()const
	{
		return *(--End());
	}

	Ref Front()
	{
		return *Begin();
	}

	ConstRef Front()const
	{
		return *Begin();
	}

	/////////////////////////////////
	//Iterator
	Iterator Begin()//(link_type)((*node).next);
	{
		return (Node*)(*_head)._next;
	}
	ConstIterator Begin()const
	{
		return (Node*)(*_head)._next;
	}

	Iterator End()
	{
		return _head;
	}
	ConstIterator End()const
	{
		return _head;
	}
	/////////////////////////////////////////
	//modifiers
	template<class InputIterator>
	void Assign(InputIterator first, InputIterator last)//先清空链表在用first——last区间的值构造链表
	{
		Clear();
		List(first, last);
	}
	void Assign(size_t n, const T& value)//先清空链表，用n个value填充链表
	{
		Clear();
		List(n, value);
	}
	
	//代码复用——>效率问题
	void PushBack(const T& value)
	{
		Insert(End(), value);
	}
	void PopBack()
	{
		Erase(Iterator(_head->_prev));
	}
	void PushFront(const T& value)
	{
		Insert(Begin(), value);
	}
	void PopFront()
	{
		Erase(Begin());
	}

	//在position之前插入value
	Iterator Insert(Iterator position, const T& value)
	{
		Node *cur = position._node;
		Node *prev = cur->_prev;
		Node *newNode = new Node(value);

		prev->_next = newNode;
		newNode->_prev = prev;

		newNode->_next = cur;
		cur->_prev = newNode;

		++_size;

		return newNode;
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
		--_size;

		position = prev;
		return prev;
	}

	/////////////////////
	//operations
	//删掉所有value
	void Remove(const T& value)
	{
		Iterator it = Begin();
		for (; it != End(); ++it)
			if (*it == value)
				Erase(it);
	}

	//去掉连续的重复的data
	void Unique(const T& value)
	{
		Iterator it1 = Begin();
		Iterator it2 = ++Begin();
		for (; it2 != End(); ++it2)
			if (it1._node->_data == it2._node->_data)
				Erase(it2);
			else
				++it1;
	}

	//清空链表
	void Clear()
	{
		while (_head != _head->_next)
			PopFront();
	}

	size_t Size()
	{
		return _size;
	}

	bool Empty()
	{
		return Begin() == End();
	}
protected:
	size_t _size;
	Node *_head;
};

#endif

#if TEST

void PrintList(const List<int>& l)
{
	List<int>::ConstIterator it1 = l.Begin();
	for (; it1 != l.End(); ++it1)
		cout << *it1 << ' ';
	cout << endl;
}

void TestList()
{
	List<int> l1;
	l1.PushBack(1);
	l1.PushBack(2);
	l1.PushBack(3);
	l1.PushBack(4);
	l1.PushFront(5);
	l1.PushFront(6);
	l1.PushFront(7);
	l1.PushFront(8);
	
	cout << l1.Back() << endl;
	cout << l1.Front() << endl;
	
	//PrintList(l1);

	//l1.PopBack();
	//l1.PopFront();

	//PrintList(l1);

	//List<int> l2;
	//l2.Assign(++l1.Begin(), --l1.End());
	//PrintList(l2);

	//l2.Assign(5, 10);
//	PrintList(l2);
	
	//l1.Clear();
	//PrintList(l1);

	//l1.PopBack();
	//l1.PopBack();
	//l1.PopBack();
	//l1.PopBack();
	//PrintList(l1);

	//l1.PopFront();
	//l1.PopFront();
	//l1.PopFront();
	//l1.PopFront();
	//PrintList(l1);

	//l1.PopFront();

	//PrintList(l1);

	////l1.Unique(3);
	////l1.Remove(30);
	//List<int>::Iterator it1 = l1.Begin();
	//while (it1 != l1.End()){
	//	if (*it1 % 2 == 0)
	//		l1.Erase(it1);
	//	++it1;
	//}
	//PrintList(l1);

	//List<int> l2(10, 5);
	//PrintList(l2);


	//List<int> l3(++l1.Begin(), l1.End());
//	l3.Insert(l3.Begin(), 10);
	//PrintList(l3);

	//List<int> l4(l3);
	//PrintList(l4);

}

#include <list>
void test_list()
{
	list<int> l;
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_back(3);
	l.push_back(3);
	l.push_back(4);
	l.push_back(4);
	l.push_back(4);
	l.push_back(4);

	list<int>::iterator it = l.begin();
	while (it != l.end()){
		cout << *it << ' ';
		++it;
	}cout << endl;

	l.unique();

	it = l.begin();
	while (it != l.end()){
		cout << *it << ' ';
		++it;
	}

	//list<int> l2;
	//l2.push_back(1);
	//l2.push_back(2);
	//l2.push_back(3);
	//l2.push_back(4);

	//l2.insert(++l2.begin(), ++l.begin(), --l.end());


	//list<int>::iterator it = l2.begin();
	//while (it != l2.end()){
	//	cout << *it << ' ';
	//	++it;
	//}
}

#endif