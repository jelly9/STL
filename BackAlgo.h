#ifndef __BACKAGLO_H__
#define __BACKAGLO_H__
#include <iostream>
using namespace std;

#define M 5 //五个进程
#define N 3 //三类资源
int Available[N];//系统可利用资源数目
int Max[M][N];//最大需求矩阵
int Allocation[M][N];//已经获取到资源数目的矩阵
int Need[M][N];//还需分配资源数目的矩阵
int Work[N];//当前系统中可以使用的资源数目
bool Finish[N];//进程执行完成情况

void Print();//打印资源分配信息
void Init();//初始化资源信息
bool Safe();//安全性检查

//输出当前资源分配信息
void Print(int max[M][N], int allo[M][N], int need[M][N], int work[N], int pnum)
{
	if (pnum == 0)
		cout << "      Max	  Allo     Need	         Work" << endl;

	//pnum号进程最大需求
	cout << "P" << pnum << "  ";
	for (size_t j = 0; j < N; ++j)
		cout << " " << max[pnum][j];

	//pnum号进程已经分配到的资源
	cout << "	";
	for (size_t j = 0; j < N; ++j)
		cout << " " << allo[pnum][j];

	//pnum号进程还需要分配的资源
	cout << "    ";
	for (size_t j = 0; j < N; ++j)
		cout << " " << need[pnum][j];

	//当前可用的资源数目
	cout << "	";
	for (size_t j = 0; j < N; ++j)
		cout << " " << work[j];
	cout << endl;
}

//初始化资源信息
void Init()
{
	//测试数据
	//	3 3 2
	cout << "请输入各类资源可用数目：" << endl;
	for (size_t i = 0; i < N; ++i)
		cin >> Available[i];

	//	{ 3, 3, 2 },
	//	{ 5, 3, 2 },
	//	{ 7, 4, 3 },
	//	{ 7, 4, 5 },
	//	{ 10, 4, 7 }
	cout << "请输入各进程最大需求：" << endl;
	for (size_t i = 0; i < M; ++i){
		cout << "P" << i << endl;
		for (size_t j = 0; j < N; ++j)
			cin >> Max[i][j];
	}

	//	{ 2, 0, 0 },
	//	{ 2, 1, 1 },
	//	{ 0, 0, 2 },
	//	{ 3, 0, 2 },
	//	{ 0, 1, 0 }
	cout << "请输入各进程已经分配到资源数目：" << endl;
	for (size_t i = 0; i < M; ++i){
		cout << "P" << i << endl;
		for (size_t j = 0; j < N; ++j)
			cin >> Allocation[i][j];
	}

	for (size_t i = 0; i < M; ++i){
		for (size_t j = 0; j < N; ++j)
			Need[i][j] = Max[i][j] - Allocation[i][j];
	}

	for (size_t i = 0; i<N; i++)
		Work[i] = Available[i];
}

//安全性检查
bool Safe()
{
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	bool flg = false;

	for (i = 0; i < M; ++i){
		//从所有进程中选出一个满足条件的执行
		for (j = i; j < M; ++j){
			if (Finish[j] == true)
				break;

			//遍历进程所需资源数是否超过当前可使用资源数
			for (k = 0; k < N; ++k){
				//如果超过则找下一个进程
				if (Need[i][k] > Work[k])
					break;
			}
			if (k < N){
				if (j == M)
					return false;
				else
					continue;
			}
			else{
				//找到一个满足条件可以执行的进程
				Finish[j] = true;
				for (size_t index = 0; index < N; ++index)
					Work[index] += Allocation[j][index];

				//模拟进程执行并打印进程信息
				Print(Max, Allocation, Need, Work, j);
				break;
			}
		}
	}

	return true;
}

void test()
{
	Init();
	bool ret = Safe();
	if (ret == true)
		cout << "安全！" << endl;
	else
		cout << "不安全！" << endl;
}

int main()
{
	test();
	return 0;
}
#endif