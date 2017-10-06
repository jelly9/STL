#ifndef _MAZE_
#define _MAZE_

#include <iostream>
using namespace std;

//#include "D:\Github\STL\stack.h"
#include <stack>

struct Pos
{
	Pos(){}
	Pos(int x, int y):_row(x),_col(y){}
	int _row;
	int _col;
};


template<size_t ROW, size_t COL>
class Maze
{
public:
	Maze(int arr[ROW][COL])
	{
		for (size_t i = 0; i < ROW; i++){
			for (size_t j = 0; j < COL; j++){
				_map[i][j] = arr[i][j];
			}
		}
	}

	void Show()
	{
		for (size_t i = 0; i < ROW; i++){
			for (size_t j = 0; j < COL; j++){
				cout << _map[i][j] << ' ';
			}
			cout << endl;
		}
	}
/////////////////////////////////////
	//非递归找到一个出口，非最短路径
	bool GetPath(Pos& entry)
	{
		stack<Pos> path;

		_map[entry._row][entry._col] = 2;
		path.push(entry);
		
		Pos next = path.Top();

		//将走过的点保存在栈中
		while (!path.empty())
		{
			if (_IsOut(path.Top())){
				cout << "出口:" << '(' << path.Top()._row << ',' << path.Top()._col << ')' << endl;
				break;
			}

			//上
			next = path.Top();
			next._row -= 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}
			//右
			next = path.Top();
			next._col += 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}

			//左
			next = path.Top();
			next._col -= 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}

			//下
			next = path.Top();
			next._row += 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}
			//走到这儿说明该点上下左右都走不通，则把该点pop掉，当循环再次开始时判断前一个点
			path.pop();
		}

		//栈为空则说明没有找到出口
		if (path.empty()){
			cout << "没有出口！" << endl;
			return false;
		}
		return true;
	}

////////////////////////////////////////////////
	//递归方法找到多个出口
	void GetPath_R(Pos& entry)
	{
		_map[entry._row][entry._col] = 2;

		if (_IsOut(entry)){
			cout << "出口:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			return;
		}
		//Show();
		//上
		Pos next = entry;
		next._row -= 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//右
		next = entry;
		next._col += 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//下
		next = entry;
		next._row += 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//左
		next = entry;
		next._col -= 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}
	}

//////////////////////////////////////////
	//使用“递归求二叉树高度”的思想求最短路径,
	//找到出口则返回最短路径的长度，若没有通路则返回整型的最大值。
	int GetShortPath(Pos& entry, int cur = 2)
	{
		_map[entry._row][entry._col] = cur;

		
		if (_IsOut(entry)){
			cout << "出口:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			return 1;
		}
		else{
			int up = INT_MAX-1;
			int down = INT_MAX-1;
			int left = INT_MAX-1;
			int right = INT_MAX-1;

			//上
			Pos next = entry;
			next._row -= 1;
			if (_IsAccess(next, entry))
				up = GetShortPath(next, cur+1) + 1;

			//右
			next = entry;
			next._col += 1;
			if (_IsAccess(next, entry))
				right = GetShortPath(next, cur+1) + 1;

			//下
			next = entry;
			next._row += 1;
			if (_IsAccess(next, entry))
				down = GetShortPath(next, cur+1) + 1;

			//左
			next = entry;
			next._col -= 1;
			if (_IsAccess(next, entry))
				left = GetShortPath(next, cur+1) + 1;

			return _GetMin(up, right, down, left);
		}
	}

///////////////////////////////////
//使用两个栈保存路径，求得最短路径
	void GetShortPath(Pos& entry, stack<Pos>& shortPath, stack<Pos>& path)
	{
		_GetShortPath(entry, shortPath, path, 2);
	}

protected:
	void _GetShortPath(Pos& entry, stack<Pos>& shortPath, stack<Pos>& path, int cur)
	{
		_map[entry._row][entry._col] = cur;
		path.push(entry);
		//cout << "前进"<<'(' << entry._row << ',' << entry._col << ')' << endl;;


		if (_IsOut(entry)){
			cout << "出口:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			if (shortPath.empty() || path.size() < shortPath.size()){
				shortPath = path;
			}	
		}

		//上
		Pos next = entry;
		next._row -= 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}

		//右
		next = entry;
		next._col += 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}


		//下
		next = entry;
		next._row += 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}

		//左
		next = entry;
		next._col -= 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}
		
		path.pop();
		//cout << "后退" << '(' << entry._row << ',' << entry._col << ')'<<endl;;

	}

protected:
	//求四个方向的最小值
	int _GetMin(int up, int right, int down, int left)
	{
		int min = up;
		if (min > right)
			min = right;

		if (min > down)
			min = down;

		if (min > left)
			min = left;

		return min;
	}

	//判断出口
	bool _IsOut(Pos& p)
	{
		if (ROW - 1 == p._row)
			return true;
		return false;
	}

	//判断能否走通
	bool _IsAccess(Pos& p)
	{
		if (p._col >= 0 && p._col < 10 && p._row >= 0 && p._row < 10 \
			&& 0 == _map[p._row][p._col])
			return true;
		return false;
	}

	//判断能否走通——最终版
	//参数一为可能要走的点，参数二为当前点
	bool _IsAccess(Pos& p, Pos& pre)
	{
		if (p._col >= 0 && p._col < 10 && p._row >= 0 && p._row < 10 \
			&& (0 == _map[p._row][p._col] || _map[p._row][p._col]  > _map[pre._row][pre._col]))
			return true;
		return false;
	}

private:
	int _map[ROW][COL];
};

#endif

void TestMaze()
{
	int arr[10][10] =
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	};
	//int arr[10][10] =
	//{
	//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//};

	Maze<10, 10> m(arr);
	stack<Pos> shortPath;
	stack<Pos> path;

	m.Show();
	m.GetShortPath(Pos(2,0), shortPath, path);
	std::cout << shortPath.size() << endl;
	//m.GetPath(Pos(2,0));
	//m.GetPath_R(Pos(2,0));
	//cout <<"最短路径的长度：" << m.GetShortPath(Pos(2, 0)) << endl;
	m.Show();
}


