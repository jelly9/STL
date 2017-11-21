/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		STL
 * 文件名 :		Sort.h
 * 作者 :			肖国栋
 *				1799553128@qq.com
 * 创建日期 :		2017-11-20
 * 版本 :			XXXX.XX.XX
 * 相关文件 :
 * 文件实现功能 : insert_sort,shell_sort,select_sort,heap_sort,
 *				bubble_sort,quick_sort,merge_sort*
 * 备注 :
 * 修改记录 :
 *
 */

#ifndef __SORT_H__
#define __SORT_H__

#pragma once
#include "D:\Github\STL\Function.h"
#include <iostream>
using namespace std;

void InsertSort(int *a, size_t n)
{
	//把第i个数插入到[0, i-1]有序区间内，使插入后的序列人保持有序
	for (size_t i = 1; i < n; ++i){
		int key = a[i];//
		size_t end = i - 1;
		while (end >= 0 && key < a[end]){
			a[end + 1] = a[end];
			--end;
		}
		a[end+1] = key;
	}
}

void ShellSort(int *a, size_t n)
{
	//在插入排序的基础上分区间
	int gap = n;
	while (gap > 1){
		gap = gap / 3 + 1;
		for (size_t i = gap; i < n; ++i){
			int key = a[i];
			size_t end = i - gap;
			while (end >= 0 && key < a[end]){
				a[end + gap] = a[end];
				end -= gap;;
			}
			a[end + gap] = key;
		}
		//PrintArray(a, n);
	}
}

void SelectSort(int *a, size_t n)
{
	//在[start, end]区间内选一个最小的、一个最大的值， 分别于start、end交换， 缩小区间重复进行
	int end = n;
	for (size_t start = 0; start < end; ++start,--end){
		int minIdx = start;
		int maxIdx = start;
		for (size_t cur = start + 1; cur < end; ++cur){
			if (a[cur] < a[minIdx])
				minIdx = cur;

			if (a[cur] > a[maxIdx])
				maxIdx = cur;
		}
		swap(a[start], a[minIdx]);
		if (maxIdx == start)
			maxIdx = minIdx;
		swap(a[end-1], a[maxIdx]);
		PrintArray(a, n);
	}
}

//向下调整
static void __AdjustDown(int *a, size_t size, int n)
{
	int parent = n;
	int child = n * 2 + 1;
	while (child < size){
		if (child + 1 < size && a[child + 1] > a[child])
			++child;

		if (a[parent] < a[child]){
			swap(a[parent], a[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
			break;
	}
}
void HeapSort(int *a, size_t n)
{
	//建堆
	for (int i = n / 2 - 1; i >= 0; --i)
		__AdjustDown(a, n, i);

	//排序
	size_t i = n;
	while(i>1){
		swap(a[0], a[i-1]);
		--i;
		__AdjustDown(a, i, 0);
	}
}

void BubbleSort(int *a, size_t n)
{
	bool flag = false;//标记是否有过交换
	for (size_t i = 0; i < n - 1; ++i){
		for (size_t j = 0; j < n - i - 1; ++j){
			if (a[j] > a[j + 1]){
				swap(a[j], a[j + 1]);
				flag = true;
			}
		}

		if (!flag)//如果没交换过则已经有序，可以直接退出
			break;
	}
}

//左右指针法
static int __PartSort_1(int *a, int left, int right)
{
	int &key = a[right];
	while (left < right){
		while (left < right && a[left] <= key)//从前往后找到一个比key大的
			++left;
		while (left < right && a[right] >= key)//从后往前找到一个比key小的
			--right;
		swap(a[left],a[right]);//交换
	}
	swap(a[left], key);//将key放的正确位置
	return left;
}

//挖坑法
static int __PartSort_2(int *a, int left, int right)
{
	int key = a[right];
	while (left < right){
		while (left < right && a[left] <= key)
			++left;
		swap(a[left], a[right]);

		while (left < right && a[right] >= key)
			--right;
		swap(a[left], a[right]);
	}

	return left;
}

//前后指针法
static int __PartSort_3(int *a, int left, int right)
{
	int &key = a[right];
	int cur = 0;
	int prev = -1;
	while (cur < right){
		if (a[cur] <= key && ++prev != cur)
			swap(a[prev], a[cur]);
		++cur;
	}

	swap(a[++prev], key);
	return prev;
}

void QuickSort(int *a, int left, int right)
{
	if (left >= right)
		return;
	
	int mid = __PartSort_3(a, left, right);
	QuickSort(a, left, mid - 1);
	QuickSort(a, mid+1, right);
}

#endif

#if 1
//测试


void TestSort()
{

#if 0
	const int N = 10;
	//int a[N] = {2, 0, 4, 9, 3, 6, 8, 7, 1 ,5};
	int a[N] = {2, 0, 5, 9, 3, 5, 8, 7, 1 ,5};
#else
	const int N = 100;
	int a[N];
	RandomArrayUnique(a, N, 0, 100);
#endif
	//InsertSort(a, N);
	//ShellSort(a, N);
	//SelectSort(a, N);
	//HeapSort(a, N);
	//BubbleSort(a, N);
	PrintArray(a, N);
	QuickSort(a, 0, N-1);
	PrintArray(a, N);
	IsIncresing(a, N);
}

#endif