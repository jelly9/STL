#ifndef _MY_VECTOR_
#define _MY_VECTOR_

#include <iostream>
using namespace std;

#include <assert.h>

template<typename T>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
public:
	/////////////////////////////////////
	//construct
	Vector()
		:_start(NULL)
		, _finish(NULL)
		, _endOfStorage(NULL)
	{}
	//用大小为n的数组构造Vector
	Vector(const T *a, size_t n)
	{
		//_RangeFillInitialize(&a, (&a)+n);
		_start = new T[n];

		for (size_t i = 0; i < n; ++i)
			_start[i] = a[i];

		_finish = _start + n;
		_endOfStorage = _finish;
	}
	//用n个value构造Vector
	Vector(size_t n, const T& value)
	{
		_FillInitialize(n, value);
	}
	//构造大小为n的Vector
	Vector(size_t n)
	{
		_FillInitialize(n, T());
	}
	//用first ——last区间的值构造Vector
	Vector(Iterator first, Iterator last)
	{
		_RangeFillInitialize(first, last);
	}

	///////////////////////////////////
	//iterator
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
	////////////////////////////////////
	//modifiers
	void PushBack(const T& value)
	{
		if (_finish == _endOfStorage)
			_Expand();
		*_finish = value;
		++_finish;
	}
	void PopBack()
	{
		assert(_start != _finish);
		--_finish;
	}
	~Vector()
	{
		_Destroy();
	}
protected:
	void _Expand()
	{
		size_t size = _endOfStorage - _start;
		size_t newSize = (0 == size) ? 1 : size * 2;
		T *temp = new T[newSize];

		for (size_t i = 0; i < size; ++i)
			temp[i] = _start[i];

		delete _start;
			
		_start = temp;
		_finish = _start + size;
		_endOfStorage = _start + newSize;
		
	}
	void _Destroy()
	{
		delete[] _start;
		_start = NULL;
		_finish = NULL;
		_endOfStorage = NULL;
	}
	void _FillInitialize(size_t n, const T& value)
	{
		_start = new T[n];
		for (size_t i = 0; i < n; ++i)
			_start[i] = value;

		_finish = _start + n;
		_endOfStorage = _finish;
	}

	void _RangeFillInitialize(Iterator first, Iterator last)
	{
		size_t size = last - first;
		_start = new T[size];
		for (size_t i = 0; i < size; ++i, ++first)
			_start[i] = *first;
	}
protected:
	T *_start;
	T *_finish;
	T *_endOfStorage;
};



#endif

#include <vector>

void PrintVector(const Vector<int>& V)
{
	Vector<int>::ConstIterator it = V.Begin();
	for (; it != V.End(); ++it)
		cout << *it << ' ';
	cout << endl;
}
void PrintVector(const vector<int>& v)
{
	vector<int>::const_iterator it = v.begin();
	for (; it != v.end(); ++it)
		cout << *it << ' ';
	cout << endl;
}
void TestVector()
{
	int a[] = {1, 2, 3, 4, 5};
	Vector<int> v1(a, sizeof(a)/sizeof(a[0]));
	PrintVector(v1);
	v1.PopBack();
	v1.PopBack();
	PrintVector(v1);


	//Vector<int> v2(v1.Begin(), v1.End());

	//Vector<int> v2;
	//v2.PushBack(1);
	//v2.PushBack(2);
	//v2.PushBack(3);
	//v2.PushBack(4);
	//v2.PushBack(5);
	//v2.PushBack(6);
	//PrintVector(v2);

	//Vector<int> v3(5, 10);
	//PrintVector(v3);

	//Vector<int> v4(10);
	//PrintVector(v4);

}

void test_list()
{
	vector<int> v1(5, 10);
	PrintVector(v1);

	vector<int> v2;
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	PrintVector(v2);

	vector<int> v3(++v2.begin(), --v2.end());
	PrintVector(v3);

	vector<int> v4(10);
	PrintVector(v4);

}