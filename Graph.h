
#ifndef __GRAPH_H__
#define __GRAPH_H__

#pragma once

#include <iostream>
using namespace std;

#include <vector>
#include <queue>
#include <map>
#include <assert.h>
#include <D:\Github\STL\UnionFindSet.h>

//邻接矩阵表示，空间浪费大
template <class V, class W, bool digraph = false>
class GraphMartix
{
public:
	GraphMartix(const V *vertex, size_t n)
	{
		_vertex.resize(n, V());
		for (size_t i = 0; i < n; ++i){
			_vertex[i] = vertex[i];
			_index[_vertex[i]] = i;
		}

		_martixEdge = new W*[n];
		for (size_t i = 0; i < n; ++i){
			_martixEdge[i] = new W[n];
			memset(_martixEdge[i], W(), sizeof(W)*n);
		}
	}

	void AddEdge(const V& src, const V& dst, const W& w)
	{
		size_t srcIndex = _GetIndex(src);
		size_t dstIndex = _GetIndex(dst);

		_martixEdge[srcIndex][dstIndex] = w;
		if (false == digraph)
			_martixEdge[dstIndex][srcIndex] = w;
	}
	W& GetLink(const V& src, const V& dst)
	{
		size_t srcIndex = _GetIndex(src);
		size_t dstIndex = _GetIndex(dst);

		return _martixEdge[srcIndex][dstIndex];
	}
private:
	size_t _GetIndex(const V& v)
	{
		assert(1 == _index.count(v));
		return _index[v];
	}
private:
	map<V, size_t> _index;//通过顶点找下标
	vector<V> _vertex;//通过下标找顶点
	W** _martixEdge;//二维矩阵
};

template<class V, class W>
struct __LinkNode
{
	__LinkNode()
	:_src(-1)
	, _dst(-1)
	, _w(W())
	, _next(NULL)
	{}

	__LinkNode(size_t src, size_t dst, const W& w)
		:_src(src)
		, _dst(dst)
		, _w(w)
		, _next(NULL)
	{}
	size_t _src;
	size_t _dst;
	W _w;
	__LinkNode* _next;
};

//邻接表
template <class V, class W, bool digraph = false>//false 表示是无向图
class GraphLink
{
	typedef __LinkNode<V, W> Link;
public:
	GraphLink(const V *vertex, size_t n)
		:_edgeSize(0)
	{
		_vertex.resize(n, V());
		_linkEdge.resize(n, NULL);
		for (size_t i = 0; i < n; ++i){
			_vertex[i] = vertex[i];
			_index[_vertex[i]] = i;
		}

	}

	//添加边
	void AddEdge(const V& src, const V& dst, const W& w)
	{
		size_t srcIndex = _GetIndex(src);
		size_t dstIndex = _GetIndex(dst);
		
		_AddEdge(srcIndex, dstIndex, w);
	}
	//获取边的权值
	Link* GetLink(const V& src, const V& dst)
	{
		size_t srcIndex = _GetIndex(src);
		size_t dstIndex = _GetIndex(dst);

		Link* edge = _linkEdge[srcIndex];
		while (edge){
			if (edge->_dst == dstIndex)
				return edge;
			edge = edge->_next;
		}

		Link invalid;
		return &invalid;
		//return NULL;
	}

	//广度优先遍历
	void GFS(const V& start)
	{
		assert(false == digraph);
		cout << "GFS: ";
		size_t startIndex = _GetIndex(start);

		vector<bool> visited;
		visited.resize(_vertex.size(), false);

		queue<size_t> q;
		q.push(startIndex);

		while (!q.empty()){
			size_t index = q.front();
			q.pop();
			if (false == visited[index]){
				cout << index << ':' << _vertex[index].c_str() << "-> ";
				visited[index] = true;
			}

			Link* edge = _linkEdge[index];
			while (edge){
				if (false == visited[edge->_dst])
					q.push(edge->_dst);
				edge = edge->_next;
			}
		}
		cout << "NULL" << endl;
	}

	//深度优先遍历
	void DFS(const V& start)
	{
		assert(false == digraph);
		cout << "DFS: ";
		size_t index = _GetIndex(start);
		vector<bool> visited;
		visited.resize(_vertex.size(), false);
		_DFS(index, visited);
		cout << "NULL" << endl;
	}

	//最小生成树
	bool MinTree(GraphLink& gp)
	{
		return _Kruskal(gp);
		//return _Prim(gp);
		//return _Dijkstra(gp);
	}

private:
	void _AddEdge(size_t srcIndex, size_t dstIndex, W w)
	{
		Link* newEdge = new Link(srcIndex, dstIndex, w);
		newEdge->_next = _linkEdge[srcIndex];
		_linkEdge[srcIndex] = newEdge;
		++_edgeSize;

		if (false == digraph){
			newEdge = new Link(dstIndex, srcIndex, w);
			newEdge->_next = _linkEdge[dstIndex];
			_linkEdge[dstIndex] = newEdge;
			++_edgeSize;
		}
	}

	//Kruscal
	bool _Kruskal(GraphLink& gp)
	{
		/*
		用并查集
		1. 先将所有边放到一个数组中，并排序
		2. 将所有顶点放到一个并查集中，刚开始各自为一个独立集合
		3. 依次从数组中取出一个边，如果边的两个顶点 不 在同一个集合中，
			则把这条边添加到最小生成树中，并这条边的两个顶点合并到同一个集合中
		4. 如果边的个数等于顶点个数-1，则完成
		*/

		//O(N)
		vector<Link*> edges;
		edges.reserve(_linkEdge.size());
		for (size_t i = 0; i < _linkEdge.size(); ++i){
			Link *edge = _linkEdge[i];
			while (edge){
				if(edge->_src < edge->_dst)
					edges.push_back(edge);
				edge = edge->_next;
			}
		}

		struct Less{
			bool operator()(const Link* l, const Link* r){
				return l->_w < r->_w;
			}
		};

		//O(NlogN)
		std::sort(edges.begin(), edges.end(), Less());//有序的边的数组
		
		UnionFindSet ufs(edges.size());
		int n = _vertex.size()-1;

		for (size_t i = 0; i < edges.size(); ++i){
			if (ufs.InOneSet(edges[i]->_src, edges[i]->_dst))
				continue;

			gp._AddEdge(edges[i]->_src, edges[i]->_dst, edges[i]->_w);//添加边
			ufs.Union(edges[i]->_src, edges[i]->_dst);//合并顶点
			if (--n == 0)
				return true;
		}

		return false;
	}
	//Prim
	bool _Prim(GraphLink& gp)
	{
		/*
		将所有顶点分成两个集合
		*/
		vector<bool> div;//区分是否在同一个集合中
		div.resize(_index.size(), false);

		struct Greater{
			bool operator()(const Link* l, const Link* r){
				return l->_w > r->_w;
			}
		};

		priority_queue<Link*, vector<Link*>, Greater> pq;
		div[0] = true;

		Link *edge = _linkEdge[0];
		while (edge){
			pq.push(edge);
			edge = edge->_next;
		}

		int n = _vertex.size() - 1;
		while (!pq.empty()){
			edge = pq.top();//每次取最短的边
			if (false == div[edge->_dst]){
				//gp._AddEdge(edge->_src, edge->_dst, edge->_w);
				gp.AddEdge(_vertex[edge->_src], _vertex[edge->_dst], edge->_w);
				div[edge->_dst] = true;
				if (--n == 0)
					return true;
			}
			pq.pop();

			edge = _linkEdge[edge->_dst];
			while (edge){
				if(false == div[edge->_dst])
					pq.push(edge);
				edge = edge->_next;
			}
		}
		return false;
	}

	//Dijkstra
	void _Dijkstra(const GraphLink& gp)
	{
		vector<W> dist;
		vector<Link*> path;
		dist.resize(_vertex.size(), -1);
		path.resize(_vertex.size(), NULL);

		for (size_t i = 0; i < _linkEdge.size(); ++i){
			Link* edge = _linkEdge[0];
			while (edge){
				if (dist[edge->_dst] == -1 || dist[edge->_dst] > edge->_w)
					dist[edge->_dst] = edge->_w;
			}
		}
	
	}

	//深度优先
	void _DFS(size_t index, vector<bool>& visited)
	{
		cout << index << ':' << _vertex[index].c_str() << "-> ";
		visited[index] = true;

		Link* edge = _linkEdge[index];
		while (edge){
			if(false == visited[edge->_dst])
				_DFS(edge->_dst, visited);
			edge = edge->_next;
		}
	}

	size_t _GetIndex(const V& v)
	{
		assert(1 == _index.count(v));
		return _index[v];
	}
private:
	map<V, size_t> _index;//通过顶点找下标
	vector<V> _vertex;//通过下标找顶点
	vector<Link*> _linkEdge;//邻接表
	size_t _edgeSize;//有多少条边
};

#endif

#if 1
//测试

//void TestGraphMartic()
//{
//	const int N = 6;
//	string strs[N] = { "西安", "宝鸡", "咸阳", "商洛", "延安", "汉中" };
//	GraphMartix<string, int> m(strs, N);
//
//	m.AddEdge("西安", "咸阳", 50);
//	m.AddEdge("西安", "延安", 200);
//	m.AddEdge("西安", "商洛", 100);
//	m.AddEdge("西安", "汉中", 180);
//	m.AddEdge("宝鸡", "咸阳", 120);
//
//	cout << m.GetLink("西安", "咸阳") << endl;
//	cout << m.GetLink("西安", "延安") << endl;
//	cout << m.GetLink("西安", "商洛") << endl;
//	cout << m.GetLink("宝鸡", "商洛") << endl;
//
//}

void TestGraphLink()
{
	const int N = 6;
	string strs[N] = { "西安", "宝鸡", "咸阳", "商洛", "延安", "汉中" };
	GraphLink<string, int> gl(strs, N);

	gl.AddEdge("西安", "延安", 200);
	gl.AddEdge("西安", "商洛", 100);
	gl.AddEdge("西安", "咸阳", 50);
	gl.AddEdge("宝鸡", "咸阳", 120);
	gl.AddEdge("宝鸡", "汉中", 140);
	gl.AddEdge("西安", "汉中", 180);


	GraphLink<string, int> minTree(strs, N);
	gl.MinTree(minTree);

	//cout << m.GetLink("西安", "咸阳")->_w << endl;
	//cout << m.GetLink("西安", "延安")->_w << endl;
	//cout << m.GetLink("西安", "商洛")->_w << endl;
	//cout << m.GetLink("宝鸡", "商洛")->_w << endl;

	//m.GFS("西安");
	//m.DFS("西安");
}

#endif
