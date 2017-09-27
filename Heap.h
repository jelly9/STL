#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
using namespace std;

#include <vector>
#include <assert.h>

//比较器
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

		for (int i = (_a.size()-2)/2; i > 0; --i)
			_AdjustDown(i);
	}

	void Push(T value)
	{
		_a.push_back(value);
		_AdjustUp(_a.size()-1);
	}

	void Pop()
	{
		assert(!_a.empty());

		swap(_a[0], _a[_a.size()-1]);
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

protected:
	//向下调整
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
	//向上调整
	void _AdjustUp(int child)
	{
		int parent = (child - 1) / 2;

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

void TestHeap()
{
	int a[] = {1, 4, 6, 2, 8, 43, 30, 28};
	Heap<int> h(a, sizeof(a)/sizeof(a[0]), -1);
	cout << h.Top() << endl;;
	h.Push(3);
	cout << h.Top() << endl;;
	h.Pop();
	cout << h.Top() << endl;;
}

//求最大的K个用小堆
//求最小的K个用大堆
void TopK(const vector<int>& a, int K)
{

	assert(a.size() > K);
	Heap<int, Less<int>> h;
	for (size_t i = 0; i < K; ++i)
		h.Push(a[i]);

	for (size_t i = K; i < a.size(); ++i){
		if (Greater<int>()(a[i],h.Top())){
			h.Pop();
			h.Push(a[i]);
		}
	}

	size_t size = h.Size();
	for (size_t i = 0; i < size; ++i){
		cout << h.Top() << ' ';
		h.Pop();
	}
}

void TestTopK()
{
	vector<int> v(100, 20);
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

	TopK(v, 5);
}

//堆排序
vector<int> HeapSort(int *a, size_t size)
{
	vector<int> v;
	v.resize(size);
	for (size_t i = 0; i < size; ++i)
		v[i] = a[i];

	//建堆
	//降序——小堆
	//升序——大堆
	for (int i = (size-2)/2; i > 0; --i){
		int p = i;//父亲
		int c = p * 2 + 1;//孩子
		while (c < size){
			if (c+1 < size && v[c+1] < v[c])//小堆
				++c;
			if (v[c] < v[p])
				swap(v[c], v[p]);
			p = c;
			c = p * 2 + 1;
		}
	}

	//排序
	int end = size;//未排序的右区间
	while (end > 0){
		swap(v[0], v[end-1]);
		--end;
		//重新调整堆
		int p = 0;
		int c = p * 2 + 1;//孩子
		while (c < end){
			if (c + 1 < end && v[c+1] < v[c])
				++c;
			if (v[c] < v[p])
				swap(v[c], v[p]);
			p = c;
			c = p * 2 + 1;
		}
	}
	return v;
}


void TestHeapSort()
{
	int a[] = {1,3,5,7,9,10,8,6,4,2};
	vector<int> v = HeapSort(a, sizeof(a)/ sizeof(a[0]));
}