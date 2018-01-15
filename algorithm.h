#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
using namespace std;

#include "D:\Github\STL\RandomFun.h"
#include "D:\Github\STL\Sort.h"


/**********************字符串处理**********************/
class str
{
public:
	/*******************字符串匹配问题*******************/
	/*暴力求解*/
	static void StrMatch_Violence(char *source, char *target)
	{
		if (strlen(source) < strlen(target))
			return ;
		int i = 0;
		int j = 0;
		vector<int> addr;
		for (; NULL != source[i]; ++i){
			for (j = 0; NULL != target[j]; ++j){
				if (source[i + j] != target[j])
					break;
			}
			if (NULL == target[j])
				addr.push_back(i);
		}

		cout << "addr: ";
		for (size_t i = 0; i < addr.size(); ++i)
			cout << addr[i] << " ";
		cout << endl;
	}

	static void  StrMacth_KMP(char* s, char* p)
	{
		int i = 0;
		int j = 0;
		int sLen = strlen(s);
		int pLen = strlen(p);
		vector<int> addr;

		int *next = new int[pLen];
		_GetNext(p, next);
		while (i < sLen){
			j = 0;
			while (i < sLen && j < pLen){
				//①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++      
				if (j == -1 || s[i] == p[j]){
					i++;
					j++;
				}
				else{
					//②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]      
					//next[j]即为j所对应的next值        
					j = next[j];
				}
			}
			if (j == pLen)
				addr.push_back(i - j);
		}
		delete[] next;

		cout << "addr: ";
		for (size_t i = 0; i < addr.size(); ++i)
			cout << addr[i] << " ";
		cout << endl;
	}

private:
	static void _GetNext(char* p, int next[])
	{
		int pLen = strlen(p);
		next[0] = -1;
		int k = -1;
		int j = 0;
		while (j < pLen - 1){
			//p[k]表示前缀，p[j]表示后缀  
			if (k == -1 || p[j] == p[k]){
				++k;
				++j;
				next[j] = k;
			}
			else{
				k = next[k];
			}
		}
	}
};

void TeststringMacth()
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
	QueryPerformanceFrequency(&nFreq);

	const int size = 100 * MB;
	char *source = new char[size];
	char *target = new char[3];

	RandStrBuff(source, size);
	RandStrBuff(target, 3);

#if 1
	QueryPerformanceCounter(&nBeginTime);
	str::StrMatch_Violence(source, target);
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "Violence:" << time <<endl;
#endif

	QueryPerformanceCounter(&nBeginTime);
	str::StrMacth_KMP(source, target);
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "KMP:" << time << endl;

	delete[] source;
	getchar();
}

/**********************数据查找问题查找**********************/
class Search
{
public:
	static int BinarySearch(int *a, int size, int val)
	{
		int left = 0;
		int right = size;
		int mid = left + (right - left) / 2;
		//int mid = (left & right) + (left ^ right)>>1;

		while (left <= right){
			mid = left + (right - left) / 2;
			if (val == a[mid])
				return mid;
			else if (val < a[mid])
				right = mid - 1;
			else left = mid + 1;
		}

		return -1;
	}

	static int BinarySearch_R(int *a, int size, int val)
	{
		return _BinarySearch_R(a, 0, size-1, val);
	}
private:
	static int _BinarySearch_R(int *a, int left, int right, int val)
	{
		if (left > right)
			return -1;

		int mid = left + (right - left) / 2;
		if (val == a[mid])
			return mid;
		else if (val < a[mid])
			return _BinarySearch_R(a, left, mid - 1, val);
		else
			return _BinarySearch_R(a, mid+1, right, val);
	}
};


void TestBinarySearch()
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
	QueryPerformanceFrequency(&nFreq);

	const size_t N = 10000000;
	int *a = new int[N];
	RandArray_E(a, N, 0x7fff);
	//PrintArray(a, N);
	QuickSortNoR(a, N);
	//PrintArray(a, N);
	//IsIncresing(a, N);

	QueryPerformanceCounter(&nBeginTime);
	//Sleep(1000*2);
	int ret = Search::BinarySearch_R(a, N, a[0]);
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "ret: " << ret << endl;
	cout << "time: " << time << endl;
	
	delete[] a;
}

