#ifndef __MY_DEQUE_H__
#define __MY_DEQUE_H__

#include <iostream>
using namespace std;
#include <assert.h>

template<class T, class Ref, class Ptr, size_t BuffSize = 8>
struct __DequeIterator
{
	typedef T**										MapPointer;
	typedef __DequeIterator<T, T&, T*, BuffSize>	Iterator;
	typedef __DequeIterator<T, Ref, Ptr, BuffSize>	Self;

	//typedef __DequeIterator Self;

	//构造函数
	__DequeIterator()
	:_cur(NULL)
	,_first(NULL)
	,_last(NULL)
	,_node(NULL)
	{}

	__DequeIterator(T* cur, MapPointer node)
		:_cur(cur)
		,_first(*node)
		,_last(_first + BuffSize)
		,_node(node)
	{}
	
	__DequeIterator(const Iterator& s)
		:_cur(s._cur)
		,_first(s._first)
		,_last(s._last)
		,_node(s._node)
	{}

	//自增增减
	Self& operator++()
	{
		++_cur;
		if (_cur == _last){
			SetNode(_node + 1);
			_cur = _first;
		}
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		++(*this);
		return temp;
	}

	Self& operator--()
	{
		if (_cur == _first){
			SetNode(_node - 1);
			_cur = _last;
		}
		--_cur;
		
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		--(*this);
		return temp;
	}

	//比较
	bool operator!=(const Self& s)const
	{
		return _cur != s._cur;
	}

	bool operator==(const Self& s)const
	{
		return !(operator!=(s));
	}

	//解引用和取址
	Ref operator *()const
	{
		return *_cur;
	}

	Ptr operator->()const
	{
		return &(operator*());
	}


	//设置位置
	void SetNode(MapPointer newNode)
	{
		_node = newNode;
		_first = *newNode;
		_last = *newNode + BuffSize;
	}

	T* _cur;
	T* _first;
	T* _last;
	MapPointer _node;
};

template<class T, size_t BuffSize = 8>
class Deque
{
	typedef T** MapPointer;
public:
	typedef __DequeIterator<T, T&, T*, BuffSize>				Iterator;
	typedef __DequeIterator<T, const T&, const T*, BuffSize>	ConstIterator;
	typedef T& Ref;
	typedef const T& ConstRef;
public:
	//construction
	Deque()
		:_map(NULL)
		,_mapSize(0)
		,_size(0)
	{
		assert(BuffSize > 2);
	}

	void PushBack(const T& value)//尾插
	{
		if (NULL == _map || _finish._cur == _finish._last - 1)	//如果_map为空（第一次配置）或当前缓冲区只剩一个空间，
			_PushBackAux(value);								//则使用备用尾插方法
		else{
			*(_finish._cur) = value;//当前缓冲区至少有两个空间
			++_finish._cur;
			++_size;
		}
	}

	void PushFront(const T& value)//头插
	{
		if (NULL == _map || _start._cur == _start._first)
			_PushFrontAux(value);
		else{
			*(_start._cur-1) = value;
			--_start._cur;
			++_size;
		}
	}

	void PopBack()
	{
		--_finish;
		if (_finish._cur == _finish._last){
			delete[] * (_finish._node - 1);
			*(_finish._node - 1) = NULL;
		}

		if (_finish == _start){
			delete[] _map;
			_map = NULL;
		}
		--_size;
	}

	void PopFront()
	{
		++_start;
		if (_start._cur == _start._first){
			delete[] * (_start._node - 1);
			*(_start._node - 1) = NULL;
		}

		if (_finish == _start){
			delete _map;
			_map = NULL;
		}
		--_size;
	}

	//Iteration
	Iterator Begin()
	{
		return _start;
	}

	ConstIterator Begin()const
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ConstIterator End()const
	{
		return _finish;
	}

	//capacity
	size_t Size()
	{
		return _size;
	}

	bool Empty()
	{
		return _start == _finish;
	}

	T& Back()
	{
		assert(0 != _size);
		if (_finish._cur != _finish._first)
			return *(_finish._cur - 1);
		else{
			Iterator it(_finish);
			--it;
			return *(it._cur);
		}
	}

	T& Front()
	{
		assert(0 != _size);
		return *(_start._cur);
	}
	~Deque()//释放空间
	{
		if (_map){
			T** cur = _start._node;
			for (; cur != _finish._node; ++cur){
				if (*cur){
					delete[] * cur;
					*cur = NULL;
				}
			}

			if (*cur){
				delete[] * cur;
				*cur = NULL;
			}
			delete[] _map;
			_map = NULL;
		}
	}

protected:
	void _PushBackAux(const T& value)
	{
		if (NULL == _map || _map + _mapSize - 1 == _finish._node){//如果中控器容量不足则给中控器扩容
			size_t newSize = _mapSize == 0 ? 2 : _mapSize * 2;
			MapPointer temp = new T*[newSize];//申请新空间
			for (size_t i = 0; i < newSize; ++i)
				*(temp + i) = NULL;
			
			size_t addToNode = _mapSize / 2;	//0
			for (size_t i = 0; i < _mapSize; ++i)//拷贝旧中控器
				temp[addToNode + i] = _map[i];

			size_t oldStartNode = _start._node - _map;//0
			size_t oldFinishNode = _finish._node - _map;//0

			if (_map)//释放旧的中控器
				delete[] _map;

			_map = temp;//修改新的中控器

			if (NULL != _finish._cur){
				_start.SetNode(temp + addToNode + oldStartNode);
				_finish.SetNode(temp + addToNode + oldFinishNode);
			}
			else{//说明是第一次增容（在此之前，没有空间）
				*(_map) = new T[BuffSize];

				_finish.SetNode(temp);
				_start.SetNode(temp);
				_finish._cur = *(_map) + BuffSize / 2;
				_start._cur = *(_map) + BuffSize / 2;
				*(_finish._cur++) = value;
				++_size;
				_mapSize = newSize;
				return;
			}
			_mapSize = newSize;
		}

		*(_finish._cur) = value;
		*(_finish._node + 1) = new T[BuffSize];
		_finish.SetNode(_finish._node + 1);
		_finish._cur = _finish._first;
		++_size;
	}
	void _PushFrontAux(const T& value)
	{
		if (NULL == _map || _map == _start._node){//如果中控器容量不足则给中控器扩容
			size_t newSize = _mapSize == 0 ? 2 : _mapSize * 2;
			MapPointer temp = new T*[newSize];//申请新空间

			size_t addToNode = _mapSize / 2;
			for (size_t i = 0; i < _mapSize; ++i)//拷贝旧中控器
				temp[addToNode + i] = _map[i];

			size_t oldStartNode = _start._node - _map;
			size_t oldFinishNode = _finish._node - _map;

			if (_map)//释放旧的中控器
				delete[] _map;

			_map = temp;//修改新的中控器

			if (NULL != _start._cur){
				_start.SetNode(temp + addToNode + oldStartNode);
				_finish.SetNode(temp + addToNode + oldFinishNode);
			}
			else{
				*(_map) = new T[BuffSize];

				_start.SetNode(_map);
				_finish.SetNode(_map);

				_start._cur = *(_map)+BuffSize / 2;
				_finish._cur = *(_map)+BuffSize / 2;

				*(_start._cur - 1) = value;
				--_start._cur;
				++_size;
				_mapSize = newSize;
				return;
			}
			_mapSize = newSize;
		}

		*(_start._node - 1) = new T[BuffSize];
		_start.SetNode(_start._node - 1);
		_start._cur = _start._last - 1;
		*(_start._cur) = value;
		++_size;
	}
protected:
	MapPointer _map;//管理中控器的指针
	Iterator _start;//指向被管理的第一个数据
	Iterator _finish;//指向被管理的最后一个数据的下一个位置
	size_t _mapSize;//有多少个中控器
	size_t _size;//有多少个数据
};
#if 1

#include "D:\Github\STL\Function.h"

void PrintDeque(const Deque<int>& d)
{
	Deque<int>::ConstIterator it = d.Begin();
	for (; it != d.End(); ++it)
		cout << *it << ' ';
	cout << endl;
}


void TestDeque2()
{
	const int N = 1000;
	int a[N];
	RandomArray(a, N);

	Deque<int> d;

	size_t i = 0;
	for (; i < N/2; ++i)
		d.PushBack(a[i]);
	//PrintDeque(d);
	cout << d.Size() << ' ';

	for (; i < N; ++i)
		d.PushFront(a[i]);
	//PrintDeque(d);
	cout << d.Size() << ' ';

	i = 0;
	for (; i < N / 2; ++i)
		d.PopFront();
	//PrintDeque(d);
	cout << d.Size() << ' ';

	for (; i < N; ++i)
		d.PopBack();
	//PrintDeque(d);
	cout << d.Size() << endl;

}
void TestDeque()
{
	Deque<int> d;
	d.PushBack(1);
	d.PushBack(2);
	d.PushBack(3);

	int i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	d.PushBack(4);
	d.PushBack(5);
	d.PushFront(6);
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	d.PushFront(7);
	d.PushFront(8);
	d.PushFront(9);
	d.PushFront(10);
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	PrintDeque(d);

	Deque<int>::Iterator it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;


	d.PopBack();
	d.PopBack();
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;
	it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;

	d.PopFront();
	d.PopFront();
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;
	it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

}
#include <stack>
#include <queue>
#include <deque>
stack<int> s;
queue<int> q;

void test_deque()
{
	deque<int> d;
	d.push_back(1);
	d.push_back(2);
	d.push_back(3);
	d.push_back(4);
	d.push_back(5);
	d.push_front(6);
	d.push_front(7);
	d.push_front(8);
	d.push_front(9);
	d.push_front(10);

	deque<int>::iterator it = d.begin();
	while (it != d.end()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;

}
#endif

#endif