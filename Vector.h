#ifndef _MY_VECTOR_
#define _MY_VECTOR_

#include <iostream>
using namespace std;

template<typename T>
class Vector
{
public:
	Vector()
	{}
	Vector(const T* a, const size_t& size)
	{
		_strat = new T[size];
	}
	~Vector()
	{}

protected:
	T* _start;
	T* _finish;
	T* _endOfStorage;
};



#endif


void Test()
{}