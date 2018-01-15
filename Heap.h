#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
using namespace std;

#include <vector>
#include <assert.h>

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};
template<class T, class Com = Less<T>>
class Heap
{
public:
	Heap()
	{}
	Heap(const T* a, const size_t size, const T& invalid)
	{
		for (size_t i = 0; i < size; ++i){
			if (invalid != a[i])
				_a.push_back(a[i]);
		}

		for (int i = (int)(_a.size() - 2) / 2; i >= 0; --i)
			_AdjustDown(i);
	}

	void Push(T value)
	{
		_a.push_back(value);
		_AdjustUp(_a.size() - 1);
	}

	void Pop()
	{
		assert(!_a.empty());

		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}

	const T Top()
	{
		assert(!_a.empty());
		return _a[0];
	}

	size_t Size()
	{
		return _a.size();
	}

	bool IsHeap()
	{
		int parent = 0;
		int child = parent * 2 + 1;

		while(child < _a.size()){
			if (Com()(_a[child], _a[parent]) || 
				child + 1<_a.size() && Com()(_a[child+1], _a[parent]))
				return false;
			++parent;
			child = parent * 2 + 1;
		}
		return true;
	}

protected:
	void _AdjustDown(int parent)
	{
		int child = parent * 2 + 1;

		while (child < _a.size()){
			if (child + 1 < _a.size() && Com()(_a[child + 1], _a[child]))
				++child;

			if (Com()(_a[child], _a[parent]))
				swap(_a[child], _a[parent]);

			parent = child;
			child = parent * 2 + 1;
		}
	}
	void _AdjustUp(size_t child)
	{
		size_t parent = (child - 1) / 2;

		while (0 != child){
			if (Com()(_a[child], _a[parent])){
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}
protected:
	vector<T> _a;
};

#endif

#if 1

#include <queue>
#include <algorithm>
void test_heap()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(6);
	v.push_back(2);
	v.push_back(8);
	v.push_back(5);
	v.push_back(3);
	make_heap(v.begin(), v.end());
	priority_queue<int>  pq;

}
void TestHeap()
{
	int a[] = { 1, 4, 6, 2, 8, 43, 30, 28 };
	Heap<int, Greater<int>> h(a, sizeof(a) / sizeof(a[0]), -1);
	cout << "IsHeap? "<<h.IsHeap() << endl;
	cout << h.Top() << endl;
	h.Push(3);
	cout << h.Top() << endl;
	h.Pop();
	cout << h.Top() << endl;
}

void TopK_H(const vector<int>& a, size_t K)
{
	assert(a.size() > K);
	Heap<int, Less<int>> h;//最大的k个用小堆
	Greater<int> com;
	for (size_t i = 0; i < K; ++i)
		h.Push(a[i]);

	for (size_t i = K; i < a.size(); ++i){
		if (com(a[i], h.Top())){
			h.Pop();
			h.Push(a[i]);
		}
	}

	size_t size = h.Size();
	for (size_t i = 0; i < size; ++i){
		cout << h.Top() << ' ';
		h.Pop();
	}
	cout << endl;
}

//向下调整，小堆
void _AdjustDown(int *a, size_t n, int p)
{
	assert(a);

	int parent = p;
	int child = p * 2 + 1;
	while (child < n)
	{
		if ((child + 1) < n && a[child] > a[child + 1])
			++child;

		if (a[parent] > a[child]){
			swap(a[parent], a[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
			break;
	}
}


//最大的k个
void TopK(int *a, int size, int k)
{
	assert(a);

	int *tmp = new int[k];

	size_t i = 0;
	for (; i < k; ++i)
		tmp[i] = a[i];

	int parent = k/2-1;
	for(; parent >= 0; --parent)
		_AdjustDown(tmp, k, 0);//小堆

	i = k;
	for (; i < size; ++i){
		if (a[i] > tmp[0]){
			tmp[0] = a[i];
			_AdjustDown(tmp, k, 0);
		}
	}

	for (i = 0; i < k; ++i)
		cout << tmp[i] << " ";
	cout << endl;

	delete[] tmp;
}

void TestTopK()
{
	const int N = 100;
	const int K = 8;
#if 1
	vector<int> v(N, 20);
	v[10] = 11;
	v[11] = 13;
	v[12] = 15;
	v[13] = 17;
	v[14] = 19;
	v[20] = 110;
	v[21] = 120;
	v[22] = 130;
	v[23] = 140;
	v[24] = 150;

	TopK_H(v, K);
#endif

	int a[N] = {0};
	a[10] = 11;
	a[11] = 13;
	a[12] = 15;
	a[13] = 17;
	a[14] = 19;
	a[20] = 110;
	a[21] = 120;
	a[22] = 130;
	a[23] = 140;
	a[24] = 150;
	TopK(a, N, K);
}


void HeapSort(int *a, int size)
{

	for (int i = size / 2 - 1; i >= 0; --i)
		_AdjustDown(a, size, i);

	int end = size - 1;
	while(end > 0)	{
		swap(a[0], a[end]);
		--end;
		_AdjustDown(a, end, 0);
	}
}


void TestHeapSort()
{
	int a[] = { 1, 3, 5, 7, 9, 8, 6, 4, 2, 0};
	HeapSort(a, sizeof(a) / sizeof(a[0]));
}

#endif