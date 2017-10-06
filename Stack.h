#ifndef __MY_STACK_H__
#define __MY_STACK_H__

#define TESTSTACK 1

#include "D:\Github\STL\Vector.h"


//栈——不提供迭代器
template<class T, class Sequence = Vector<T>>
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

void TestStack()
{
	Stack<BB> s1;
}

//#include <stack>
//#include <vector>
//#include <list>
//
//void test_stack()
//{
//	stack<int> s;
//
//	s.push(1);
//	s.push(2);
//	s.push(3);
//	s.push(4);
//}
#endif