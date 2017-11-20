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

#endif

#if 1
//测试

const int N = 10;

void TestSort()
{

#if 0
	int a[N] = {2, 5, 4, 9, 3, 6, 8, 7, 1 ,0};
#else
	int a[N];
	RandomArrayUnique(a, N, 0, 100);
#endif
	//InsertSort(a, N);
	//ShellSort(a, N);
	//SelectSort(a, N);
	HeapSort(a, N);
	PrintArray(a, N);
	IsIncresing(a, N);
}

#endif