#ifndef __MY_STACK_H__
#define __MY_STACK_H__

#define TESTSTACK 1

#include "D:\Github\STL\Deque.h"


//栈——不提供迭代器
template<class T, class Sequence = Deque<T>>
class Stack
{
private:
	typedef typename Sequence::Ref			Ref;
	typedef typename Sequence::ConstRef		ConstRef;
public:
	void Push(const T& value){_s.PushBack(value);}
	void Pop(){_s.PopBack();}
	Ref Top(){return _s.Back();}
	ConstRef Top()const{return _s.Back();}
	bool Empty(){return _s.Empty();}
	size_t Size(){return _s.Size();	}
protected:
	Sequence _s;
};



#endif

#if TESTSTACK
#include "D:\Github\STL\List.h"
#include <iostream>
using namespace std;

class AA
{
public:
	AA()
	{ 
		i = new int[10];
	}

	~AA()
	{
		delete[] i;
	}
	int *i;
};

class BB{
	AA a;
};
#if 1
void TestStack()
{
	Stack<int> s1;
	s1.Push(11);
	cout << s1.Top() << endl;

	s1.Push(12);
	cout << s1.Top() << endl;

	s1.Push(21);
	cout << s1.Top() << endl;

	s1.Push(22);
	cout << s1.Top() << endl;

	cout << s1.Top() << endl;
	s1.Pop();
	cout << s1.Top() << endl;

	s1.Pop();
	cout << s1.Top() << endl;

	s1.Pop();
	cout << s1.Top() << endl;

	s1.Pop();

}

#include <stack>
#include <vector>
#include <list>

void test_stack()
{
	stack<int> s1;
	s1.push(11);
	cout << s1.top() << endl;

	s1.push(12);
	cout << s1.top() << endl;

	stack<int, list<int>> s2;
	s2.push(21);
	cout << s2.top() << endl;
	
	s2.push(22);
	cout << s2.top() << endl;

}

#endif
#endif