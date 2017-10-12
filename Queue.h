#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

#define TESTQUEUE 1

#include "STL\List.h"

template<class T, class Sequence = List<T>>
class Queue
{
private:
	typedef typename Sequence::Ref			Ref;
	typedef typename Sequence::ConstRef		ConstRef;
public:
	void Push(const T& value)
	{
		_q.PushBack(value);
	}
	void Pop()
	{
		_q.PopFront();
	}

	Ref Front()
	{
		return _q.Front();
	}

	ConstRef Front()const
	{
		return _q.Front();
	}

	Ref Back()
	{
		return _q.Back();
	}

	ConstRef Back()const
	{
		return _q.Back();
	}

	bool Empty()
	{
		return _q.Empty();
	}

	size_t Size()
	{
		return _q.Size();
	}

protected:
	Sequence _q;
};


#endif

#if TESTQUEUE

#include <iostream>
using namespace std;

void TestQueue()
{
	Queue<int> q;
	q.Push(1);
	q.Push(2);
	cout << q.Front() << endl;//1
	cout << q.Back() << endl;//2

	q.Push(3);
	q.Push(4);
	cout << q.Back() << endl;//4

	q.Pop();
	cout << q.Front() << endl;//2
	cout << q.Back() << endl;//4


}

#include <queue>
#include <list>
#include <vector>

void test_queue()
{
	queue<int, list<int>> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	cout << q.front() << endl;
	cout << q.back() << endl << endl;

	q.pop();
	cout << q.front() << endl;
	cout << q.back() << endl << endl;

	q.pop();
	cout << q.front() << endl;
	cout << q.back() << endl << endl;

	q.pop();
	cout << q.front() << endl;
	cout << q.back() << endl << endl;

	q.pop();
}

#endif