#ifndef __MY_STACK_H__
#define __MY_STACK_H__

#include "STL\Vector.h"

//栈——不提供迭代器
template<class T, class Squeue = Vector<T>>
class Stack
{
private:
	typedef T&			Ref;
	typedef const T&	ConstRef;
public:
	void Push(const T& value)
	{
		_s.PushBack(value);
	}
	void Pop()
	{
		_s.PopBack();
	}

	Ref Top()
	{
		return _s.Back();
	}

	ConstRef Top()const
	{
		return _s.Back();
	}
	bool Empty()
	{
		return _s.Empty();
	}

	bool Size()
	{
		return _s.Size();
	}
protected:
	Squeue _s;
};



#endif

#define TESTSTACK 1
#if TESTSTACK
#include "STL\List.h"

void TestStack()
{
	Stack<int> s1;
	cout << s1.Size() << endl;
	s1.Push(1);
	cout << s1.Top() << endl;
	s1.Push(2);
	cout << s1.Top() << endl;

	s1.Push(3);
	cout << s1.Top() << endl;

	s1.Push(4);
	cout << s1.Top() << endl;

	s1.Push(5);
	cout << s1.Top() << endl;

	s1.Push(6);
	cout << s1.Top() << endl;

	s1.Pop();
	cout << s1.Top() << endl;

	s1.Pop();
	cout << s1.Top() << endl;

}
#endif