
//模拟实现的SGI_STL中的Vector的部分功能
//无空间配置器

#ifndef __VECTOR_H__
#define __VECTOR_H__
#define TEST 0

#ifndef __IOSTREAM__
#define __IOSTREAM__
#include <iostream>
#endif

using namespace std;

#ifndef __ASSERT__
#define __ASSERT__
#include <assert.h>
#endif

template<typename T>
class Vector
{
public:
	typedef T&			Ref;
	typedef const T&	ConstRef;

	typedef T*			Ptr;
	typedef const T*	ConstPtr;

	typedef T*			Iterator;
	typedef const T*	ConstIterator;
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
	//Vector(Iterator first, Iterator last)
	//{
	//	_RangeFillInitialize(first, last);
	//}

	Ref operator[](size_t n)
	{
		assert(n >= 0);
		return _start[n];
	}
	ConstRef operator[](size_t n)const
	{
		assert(n >= 0);
		return *(Begin()+n);
	}

	Ref Back()
	{
		return *(End()-1);
	}

	ConstRef Back()const
	{
		return *(End() - 1);
	}

	Ref Front()
	{
		return *Begin();
	}

	ConstRef Front()const
	{
		return *Begin();
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
	//capacity
	size_t Size()const
	{
		return (size_t)(End() - Begin());
	}
	size_t MaxSize()
	{
		return (size_t)(-1) / sizeof(T);
	}
	size_t Capacity()const
	{
		return (size_t)(_endOfStorage - Begin());
	}

	void Resize(size_t n, const T& value)
	{
		size_t size = Size();
		
		_Expand(n);
		for (size_t i = size; i < n; ++i)
			_start[i] = value;

		_finish = _start + n;
	}
	void Resize(size_t n)
	{
		Resize(n, T());
	}

	void Reverse(size_t n)
	{
		_Expand(n);
	}
	bool Empty()
	{
		return Begin() == End();
	}

	////////////////////////////////////
	//modifiers
	void PushBack(const T& value)
	{
		if (_finish == _endOfStorage){
			size_t size = Size();
			size_t newSize = (0 != size) ? (((MaxSize()-size)<size) ? 0 : size * 2) : 1;
			_Expand(newSize);
		}

		*_finish = value;
		++_finish;
	}
	void PopBack()
	{
		assert(_start != _finish);
		--_finish;
	}

	/////////////////////////////////////
	//deconstruct
	~Vector()
	{
		_Destroy();
	}
protected:
	//若n大于当前容量，则重新开辟一个大小为n的空间，并将数据拷贝过去，否则什么也不干
	void _Expand(size_t n)
	{
		if (n > Capacity()){
			T *temp = new T[n];

			size_t size = Size();
			for (size_t i = 0; i < size; ++i)
				temp[i] = _start[i];

			delete _start;

			_start = temp;
			_finish = _start + size;
			_endOfStorage = _start + n;
		}
	}
	void _Destroy()
	{
		if (_start){
			delete[] _start;
			_start = _finish = _endOfStorage = NULL;
		}
	}
	//开辟大小为n的空间，并用value初始化之
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
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};



#endif

#if TEST
#include <vector>
void PrintVector(const Vector<int>& V)
{
	Vector<int>::ConstIterator it = V.Begin();
	for (; it != V.End(); ++it)
		cout << *it << ' ';
	cout << endl;
	cout << "Size: " << V.Size() << endl;
	cout << "Capacity: " << V.Capacity() << endl;
}

void TestVector()
{
	Vector<int> v;
	//v.Resize(5, 10);
	//v.Resize(10, 20);
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);

	cout << v.Back() << endl;
	cout << v.Front() << endl;
	//v.PushBack(2);
	//v.PushBack(2);
	//v.PushBack(2);
	//v.PushBack(2);
	//PrintVector(v);
	//

	//v.Resize(3);
	//PrintVector(v);
	//v.Resize(5, 10);
	//PrintVector(v);
	//v.Resize(20);
	//PrintVector(v);

	//int a[] = {1, 2, 3, 4, 5};
	//Vector<int> v1(a, sizeof(a)/sizeof(a[0]));
	//PrintVector(v1);
	//v1.PopBack();
	//v1.PopBack();
	//PrintVector(v1);
	


	//Vector<int> v2;
	//v2.PushBack(1);
	//PrintVector(v2);
	//v2.PushBack(2);
	//PrintVector(v2);
	//v2.PushBack(3);
	//PrintVector(v2);
	//v2.PushBack(4);
	//PrintVector(v2);
	//v2.PushBack(5);
	//PrintVector(v2);
	//v2.PushBack(6);
	//PrintVector(v2);

	//cout << "MAxSize:" << v2.MaxSize() << endl;

	//Vector<int> v3(5, 10);
	//PrintVector(v3);

	//Vector<int> v4(10);
	//PrintVector(v4);

}


void PrintVector(const vector<int>& v)
{
	vector<int>::const_iterator it = v.begin();
	for (; it != v.end(); ++it)
		cout << *it << ' ';
	cout << endl;
	cout << "Size: " << v.size() << endl;
	cout << "Capacity: " << v.capacity() << endl;
}

void test_list()
{
	int a[4] = {1, 2, 3, 4};
	cout << *(a + 0) << ' ';
	cout << *(a + 1) << ' ';
	cout << *(a + 2) << ' ';
	cout << *(a + 3) << ' ' << endl;;

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	//cout << *(v + 1) << ' ';
 }

#endif