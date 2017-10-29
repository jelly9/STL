/*
内容：常用函数实现
时间：2017.10.27
*/

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//声明
//array相关
//给一个长度为 len 的数组 a，用 begin——end（左闭右开） 之间的（伪）随机数初始化它，
//默认情况下，用 0——100 之间的（伪）随机数初始化。
void RandArrayEqual(int a[], size_t len, int begin, int end);	//允许有相同的数
void RandArrayUnique(int a[], size_t len, int begin, int end);	//没有相同的数
//打印数组
void PrintArray(int a[], size_t len);

//比较相关仿函数
template<class T>
struct Less{
	bool operator()(const T& l, const T& r){ return l < r; }
};

template<class T>
struct Greater{
	bool operator()(const T& l, const T& r){ return l > r; }
};


//定义
///////////////////////////////////////////////////////////////////////

void RandArrayEqual(int a[], size_t len, int begin = 0, int end = 100)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;

	for (size_t i = 0; i < len; ++i)
		a[i] = rand() % interval + begin;

	PrintArray(a, len);

}

void RandArrayUnique(int a[], size_t len, int begin = 0, int end = 100)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;	//区间内数的个数
	int randNum;					//生成的随机数

	for (size_t i = 0; i < len; ++i){
		while (1){
			randNum = rand() % interval + begin;
			size_t j = 0;
			for (; j < i; ++j){
				if (randNum == a[j])
					break;
			}
			if (j == i)
				break;
		}

		a[i] = randNum;
	}

	PrintArray(a, len);
}

void PrintArray(int a[], size_t len)
{
	for (size_t i = 0; i < len; ++i)
		printf("%d ", a[i]);
	printf("\n");
}

//测试
///////////////////////////////////////////////////////////////////////

#if 1
void TestRandomRArray()
{
	int a[100];
	RandArrayEqual(a, 100, 0, 500);
	PrintArray(a, 100);
}

#endif

#endif