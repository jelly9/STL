/*
*
* 版权 : 		(c) 2017
* 模块名 :
* 文件名 :		UnionFindSet.h
* 作者 :		肖国栋
*				1799553128@qq.com
* 创建日期 :	2017-11-21
* 版本 :		XXXX.XX.XX
* 相关文件 :
* 文件实现功能 : 并查集解决朋友圈
* 备注 :
* 修改记录 :
*
*/

#ifndef __UNIONFINDSET_H__
#define __UNIONFINDSET_H__

#pragma once
#include <iostream>
using namespace std;
#include <vector>

class UnionFindSet
{
public:
	UnionFindSet(size_t n)
	{
		_sets.resize(n+1, -1);
	}

	void Union(size_t p1, size_t p2)
	{
		//找根节点的下标
		size_t root1 = _FindRoot(p1);
		size_t root2 = _FindRoot(p2);

		//根不同说明不在同一个朋友圈里，则要合并
		if (root1 != root2){
			_sets[root1] += _sets[root2];
			_sets[root2] = root1;
		}
	}

	bool InOneSet(size_t p1, size_t p2)
	{
		size_t root1 = _FindRoot(p1);
		size_t root2 = _FindRoot(p2);

		if (root1 == root2)
			return true;

		return false;
	}

	size_t SetSize()
	{
		size_t count = 0;
		for (size_t i= 0; i < _sets.size(); ++i)
		{
			//若果小于0， 则说明是根
			if (_sets[i] < 0)
				++count;
		}

		return count - 1;
	}
private:
	size_t _FindRoot(size_t x)
	{
		while (_sets[x] >= 0)
			x = _sets[x];

		return x;
	}
private:
	vector<int> _sets;
};

#endif

#if 0
//测试

void Friends(size_t n, size_t m, int r[][2])
{
	UnionFindSet s(n);

	for (size_t i = 0; i < m; ++i)
		s.Union(r[i][0], r[i][1]);

	cout << "SetSize(): "<< s.SetSize() << endl;
}
void Test()
{
	const  int n = 5;
	const int m = 5;

	int a[m][2] = { { 1, 2 }, { 2, 3 }, { 4, 5 }, { 1, 5 }, {3, 4} };
	Friends(n, m, a);

}
#endif