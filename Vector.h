
//模拟实现的SGI_STL中的Vector的部分功能
//无空间配置器

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
using namespace std;

#include <assert.h>

template<class T>
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
	Ref operator=(ConstRef v);
	Vector(ConstRef v);
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
		_start = new T[n];

		for (size_t i = 0; i < n; ++i)
			_start[i] = a[i];

		_finish = _start + n;
		_endOfStorage = _finish;
	}
	Vector(const Vector<T>& v)
	{
		_Expand(v.Capacity());
		for (size_t i = 0; i < v.Size(); ++i)
			_start[i] = v[i];

		_finish = _start + v.Size();
		_endOfStorage = _start + v.Capacity();
	}

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
	//vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
	//	if (&x != this) {
	//		if (x.size() > capacity()) {
	//			iterator tmp = allocate_and_copy(x.end() - x.begin(),
	//				x.begin(), x.end());
	//			destroy(start, finish);
	//			deallocate();
	//			start = tmp;
	//			end_of_storage = start + (x.end() - x.begin());
	//		}
	//		else if (size() >= x.size()) {
	//			iterator i = copy(x.begin(), x.end(), begin());
	//			destroy(i, finish);
	//		}
	//		else {
	//			copy(x.begin(), x.begin() + size(), start);
	//			uninitialized_copy(x.begin() + size(), x.end(), finish);
	//		}
	//		finish = start + x.size();
	//	}
	//	return *this;
	//}
	bool operator==(const Vector<T>& v)
	{
		return _start == v._start && _finish == v._finish && _endOfStorage == v._endOfStorage;
	}

	bool operator!=(const Vector<T>& v)
	{
		return _start != v._start && _finish != v._finish && _endOfStorage != v._endOfStorage;
	}

	Vector<T>& operator=(const Vector<T>& v)
	{
		if (this != &v){
			_Destroy();
			Vector(v);
		}
		return *this;
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
	void Swap(Vector<T>& v)
	{
		std::swap(_start, v._start);
		std::swap(_finish, v._finish);
		std::swap(_endOfStorage, v._endOfStorage);
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

			if(NULL != _start)
				delete[] _start;

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


protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};



#if 1

#include <iostream>
using namespace std;

#include <string>

void PrintVector(const Vector<string>& V)
{
	Vector<string>::ConstIterator it = V.Begin();
	for (; it != V.End(); ++it)
		cout << *it << ' ';
	cout << endl;
	cout << "Size: " << V.Size() << endl;
	cout << "Capacity: " << V.Capacity() << endl;
}

//#include "D:\Github\STL\Function.h"

void TestVector()
{
	Vector<string> v;
	//v.Resize(5, 10);
	//v.Resize(10, 20);
	v.PushBack("你好");
	v.PushBack("2222");
	v.PushBack("3333");
	v.PushBack("4444");
	PrintVector(v);


	Vector<string> v2;
	v2.PushBack("11");
	v2.PushBack("22");
	v2.PushBack("33");
	v2.PushBack("44");
	PrintVector(v2);

	v2.Swap(v);
	PrintVector(v);
	PrintVector(v2);



	//cout << v.Back() << endl;
	//cout << v.Front() << endl;
	
	
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

//void PrintVector(const vector<int>& v)
//{
//	vector<int>::const_iterator it = v.begin();
//	for (; it != v.end(); ++it)
//		cout << *it << ' ';
//	cout << endl;
//	cout << "Size: " << v.size() << endl;
//	cout << "Capacity: " << v.capacity() << endl;
//}
//
//void test_list()
//{
//	//vector<int> v;
//	//v.push_back(1);
//	//v.push_back(2);
//	//v.push_back(3);
//	//v.push_back(4);
//
//	//vector<int> v2(v);
//	//cout << *(v + 1) << ' ';
//
//	vector<string> v;
//	v.push_back("1111");
//	v.push_back("2222");
//	v.push_back("3333");
//	v.push_back("4444");
//
//	vector<string> v2(v);
//
//	v[2] = v2[1];
// }
#endif

#endif
