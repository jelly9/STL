/*
内容：常用函数实现
时间：2017.10.27
*/

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <assert.h>

//声明
// 用随机数初始化数组
void RandomArray(int a[], size_t len, int begin, int end);
//打印数组
void PrintArray(int a[], size_t len);
// 打印容器
template<class Container>
void PrintContainer(const Container& l);

//检查是否有序
bool IsIncresing(int *a, size_t size);
bool IsIncresing(int *a, size_t size);


// 定义
void RandomArray(int a[], size_t size, int begin = 0, int end = INT_MAX)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;	//区间内数的个数
	int randNum;					//生成的随机数

	for (size_t i = 0; i < size; ++i){
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
}

void RandomArray_Equal(int a[], size_t size, int begin = 0, int end = INT_MAX)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;	//区间内数的个数
	int randNum;					//生成的随机数

	for (size_t i = 0; i < size; ++i){
		randNum = rand() % interval + begin;
		a[i] = randNum;
	}
}

void PrintArray(int *a, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		printf("%d ", a[i]);
	printf("\n");
}

bool IsIncresing(int *a, size_t size)
{
	bool flag;
	for (size_t i = 1; i < size; ++i){
		if (a[i - 1] > a[i]){
			std::cout << "error!!!!!!!!!!!!!!!!!!!!!!!" <<std::endl  << "a[" << i - 1 << "]=" << a[i - 1]
				<< " a[" << i << "]=" << a[i] << std::endl;
			flag = false;
		}
	}
	return true;
}

bool IsDecresing(int *a, size_t size)
{
	bool flag = true;
	for (size_t i = 1; i < size; ++i){
		if (a[i - 1] < a[i]){
			std::cout << "error! " << "a[" << i - 1 << "]=" << a[i - 1]
				<< "a[" << i << "]=" << a[i] << std::endl;
			flag = false;
		}
	}

	return flag;
}

template<class Container>
void PrintContainer(const Container& l)
{
	Container::ConstIterator it = l.Begin();
	for (; it != l.End(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
}


//测试
///////////////////////////////////////////////////////////////////////

#if 0
void TestRandomArray()
{
	int a[100];
	RandomArrayEqual(a, 100, 0, 500);
	PrintArray(a, 100);
}

#endif

#endif