#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

#define TESTQUEUE 0

#include "STL\List.h"

template<class T, class Container = List<T>>
class Queue
{
private:
	typedef T&			Ref;
	typedef const T&	ConstRef;
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
	Container _q;
};


#endif

#if TESTQUEUE

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

#endif