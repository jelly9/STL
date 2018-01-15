#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <windows.h>
#include <time.h>

#define TEST_RANDOMFUN 1

#ifndef INT_MAX
#define INT_MAX 0x7fff
#endif

const unsigned long maxshort = 65535L;
const unsigned long multiplier = 1194211693L;
const unsigned long adder = 12345L;

class Random
{
private:
	//当前种子
	unsigned long randSeed;
public:
	//构造函数，默认值0表示由系统自动产生种子
	Random(unsigned long s = 0)
	{
		if (s == 0)
			randSeed = time(0);   //用系统时间产生种子
		else
			randSeed = s;   //由用户提供种子
	}

	//产生0到n-1之间的随机整数
	unsigned int RandNum(unsigned short n = maxshort)
	{
		randSeed = multiplier * randSeed + adder;
		return (unsigned int)((randSeed >> 16) % n);
	}

	//产生[0,1)之间的随机实数
	double fRandNum()
	{
		return RandNum() / double(maxshort);
	}
};

//用随机数填充数组
void RandArray(int a[], size_t size, int n = INT_MAX)
{
	Random r;
	int randNum;					//生成的随机数

	for (size_t i = 0; i < size; ++i){
		while (1){
			randNum = r.RandNum() % n;
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

void RandArray_E(int a[], size_t size, int n = INT_MAX)
{
	Random r;
	int num;					//生成的随机数

	for (size_t i = 0; i < size; ++i){
		num = r.RandNum() % n;
		a[i] = num;
	}
}


#define KB 1024
#define MB 1024*KB
#define GB 1024*MB

void RandStrBuff(char *buff, size_t size)
{
	Random r;
	size_t i = 0;
	while (size--)
		buff[i++] = r.RandNum(52) + 'A';
}

void RandStrFile(const char *fileName, size_t size)
{
	FILE *fIn = fopen(fileName, "wb");
	Random r;
	const int BufSize = 1 * KB;

	char buf[BufSize];
	int i = 0;
	int count = BufSize;
	while (1){
		while (count--)
			buf[i++] = r.RandNum(52) + 'A';

		buf[1023] = '\n';
		fwrite(buf, 1, BufSize, fIn);

		i = 0;
		count = BufSize;
		if ((int)(size -= BufSize) < 0)
			break;
	}
	fclose(fIn);
}



#if TEST_RANDOMFUN
/**********************测试程序**********************/

void TestCreateStrFile()
{
	RandStrFile("Test.txt", 100 * MB);
}

int TossCoins(int numberCoins)
{ //随机抛硬币
	static Random coinToss;
	int  i, tosses = 0;
	for (i = 0; i < numberCoins; i++)
		tosses += coinToss.RandNum(2);
	return tosses;
}


void TestCoin()
{ // 模拟随机抛硬币事件
	const int NCOINS = 10;
	const long NTOSSES = 1000000L;

	std::cout << "ntosses: " << NTOSSES << std::endl;

	// heads[i]是得到i次正面的次数
	long i, heads[NCOINS + 1];
	int j, position;

	// 初始化数组heads
	for (j = 0; j< NCOINS + 1; j++)
		heads[j] = 0;

	// 重复50000次模拟事件
	for (i = 0; i< NTOSSES; i++)
		heads[TossCoins(NCOINS)]++;

	// 输出频率图
	for (i = 0; i <= NCOINS; i++)
	{
		position = int(float(heads[i]) / NTOSSES * 72);
		printf("%2d  %6d     ", i, heads[i]);
		for (j = 0; j<position - 1; j++)
			std::cout << " ";
		std::cout << "*" << std::endl;
	}

	getchar();
}

/**********************随机数法求PI**********************/
double Darts(int n)
{
	//随机数法求PI
	Random dart;
	int k = 0;
	for (int i = 0; i < n; ++i){
		double x = dart.fRandNum();
		double y = dart.fRandNum();

		if ((x*x + y*y) <= 1)
			++k;
	}

	return 4 * k / double(n);
}

void TestPI()
{
	std::cout << "PI: " << Darts(10000000) << std::endl;
	getchar();
}

#endif