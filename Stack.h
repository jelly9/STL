#ifndef __MY_STACK_H__
#define __MY_STACK_H__

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
	bool Empty()const{return _s.Empty();}
	size_t Size()const{return _s.Size();	}
protected:
	Sequence _s;
};

#endif

////////////////////////////////////////////////////////
#if 1
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

///////////////////////////////////////////////////////
/*
 * 有两组序列，一组为入栈顺序，另一组为出栈顺序，判断连个序列书否匹配
 */
bool StackOrder(int *inOrder, int *outOrder, size_t n)
{
	stack<int> s;
	size_t inIndex = 0;
	size_t outIndex = 0;

	while (inIndex < n){
		while (!s.empty() && s.top() == outOrder[outIndex]){
			s.pop();
			++outIndex;
		}

		s.push(inOrder[inIndex++]);
	}
	while (!s.empty() && s.top() == outOrder[outIndex]){
		s.pop();
		++outIndex;
	}

	if (outIndex == n)
		return true;
	else
		return false;
}

void TestStackOrder()
{
	const int N = 5;
	int in[N] = { 1, 2, 3, 4, 5 };
	//int out[N] = { 1, 2, 3, 4, 5 };
	//int out[N] = { 4, 5, 3, 2, 1 };
	//int out[N] = { 3, 2, 1, 4, 5 };
	//int out[N] = { 4, 5, 2, 3, 1 };
	int out[N] = { 1, 4, 5, 2, 3 };
	cout << StackOrder(in, out, N) << endl;;
}

//////////////////////////////////////////////////////
/*
 * 实现一个栈，它有这些功能，Push(), Pop(), Min()//取栈中最小值;
 */
class Stack_M
{
public:
	void Push(int x)
	{
		_s.push(x);

		if (_minS.empty() || x < _minS.top())
			_minS.push(x);
		else
			_minS.push(_minS.top());
	}
	
	void Pop()
	{
		_s.pop();
		_minS.pop();
	}

	int& Min()
	{
		return _minS.top();
	}
private:
	stack<int> _s;
	stack<int> _minS;
};

void TestStack_M()
{

	Stack_M s;
	s.Push(6);
	s.Push(2);
	s.Push(1);
	s.Push(3);
	s.Push(10);
	s.Push(0);
}

/////////////////////////////////
/*
 * 两个栈实现一个队列
 */
class QueueOfDoubleStack
{
public:
	void Push(const int& x)
	{
		_in.push(x);
	}

	void Pop()
	{
		if (!_out.empty())
			_out.pop();
		else if (!_in.empty()){
			while (!_in.empty()){
				_out.push(_in.top());
				_in.pop();
			}
			_out.pop();
		}
		else
			assert(false);
	}

	int& Top()
	{
		return _in.top();
	}
private:
	stack<int> _in;
	stack<int> _out;
};

void TestQueueOfDoubleStack()
{
	QueueOfDoubleStack q;
	q.Push(1);
	q.Push(2);
	q.Push(3);
	
	q.Pop();
	q.Pop();
	
	q.Push(5);
	q.Push(4);

}

//////////////////////////////////////////////////
/*
* 两个队列实现一个栈
*/
class StackOfDoubleQueue
{
public:
	void Push(const int& x)
	{
		if (!_q1.empty())
			_q1.push(x);
		else 
			_q2.push(x);
	}

	void Pop()
	{
		if (!_q1.empty()){
			size_t size = _q1.size();
			while (1 != size){
				_q2.push(_q1.front());
				_q1.pop();
				--size;
			}
			_q1.pop();
		}
		else{
			size_t size = _q2.size();
			while (1 != size){
				_q1.push(_q2.front());
				_q2.pop();
				--size;
			}
			_q2.pop();
		}
	}

	int& Front()
	{
		if (!_q1.empty())
			return _q1.front();
		else
			return _q2.front();
	}

	int& Back()
	{
		if (!_q1.empty())
			return _q1.back();
		else
			return _q2.back();
	}
private:
	queue<int> _q1;
	queue<int> _q2;
};

void TestStackOfDoubleQueue()
{
	StackOfDoubleQueue s;

	s.Push(1);
	s.Push(2);
	s.Push(3);
	s.Push(4);
	s.Push(5);

	cout << s.Front() << endl;
	cout << s.Back() << endl;

	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
	s.Pop();
	
	cout << s.Front() << endl;
	cout << s.Back() << endl;
}

/*
 * 共享栈：一个数组实现两个栈
 */

class DoubleStack
{
public:
	DoubleStack()
		:_a(NULL)
		,_top1(0)
		,_top2(0)
		,_size(0)
	{}
	void Push1(const int& x)
	{
		_CheckCapacity();
		_a[_top1++] = x;
	}

	void Pop1()
	{
		assert(0 != _top1);
		--_top1;
	}

	int& Top1()
	{
		assert(0 != _top1);
		return _a[_top1-1];
	}

	void Push2(const int& x)
	{
		_CheckCapacity();
		_a[_top2--] = x;
	}

	void Pop2()
	{
		assert(0 != _size);
		++_top2;
	}

	int& Top2()
	{
		assert(0 != _size);
		return _a[_top2 + 1];
	}
private:
	void _CheckCapacity()
	{
		if (0 == _size){
			_size = 8;
			_a = new int[_size];
			_top1 = 0;
			_top2 = _size - 1;
			return;
		}

		if (_top1 - 1 == _top2){
			size_t newSize = _size * 2;
			int *tmp = new int[newSize];
			for (size_t i = 0; i < _top1; ++i)
				tmp[i] = _a[i];

			size_t k = newSize - 1;
			for (size_t j = _size - 1; j > _top2; --j)
				tmp[k--] = _a[j];

			delete _a;
			_a = tmp;
			_top2 = _top2 + _size;
			_size = newSize;
		}
	}
private:
	int *_a;
	size_t _top1;
	size_t _top2;
	size_t _size;
};

void TestDoubleStack()
{
	DoubleStack ds;
	ds.Push1(1);
	ds.Push1(2);
	ds.Push1(3);
	cout << ds.Top1() << endl;

	ds.Push2(8);
	ds.Push2(7);
	ds.Push2(6);
	ds.Push2(5);
	ds.Push2(4);
	ds.Push2(3);
	cout << ds.Top2() << endl;
}
#endif