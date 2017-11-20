/*
*
* 版权 : 		(c) 2017
* 模块名 :		STL
* 文件名 :		BTree.h
* 作者 :		肖国栋
*				1799553128@qq.com
* 创建日期 :	2017-11-20
* 版本 :		XXXX.XX.XX
* 相关文件 :
* 文件实现功能 :B树、B+树、B*树的实现
* 备注 :
* 修改记录 :
*
*/

#ifndef __BTREE_H__
#define __BTREE_H__

#pragma once

#include <iostream>
using namespace std;

template<class Value, size_t N>
struct __BTreeNode
{
	typedef __BTreeNode<Value, N> Node;
	__BTreeNode()
	:_size(0)
	,_parent(NULL)
	{
		size_t i = 0;
		for (; i < N; ++i){
			_valueField[i] = Value();
			_subs[i] = NULL;
		}
		_subs[i] = NULL;
	}
	Value _valueField[N];
	Node* _subs[N+1];
	Node* _parent;
	size_t _size;
};

#if 1
template<class Value>
struct __KeyOfValue
{
	Value operator()(const Value& value)
	{
		return value;
	}
};

#endif

template<class Key, class Value, class _KeyOfValue = __KeyOfValue<Value>, size_t N = 3>
class BTree
{
	typedef __BTreeNode<Value, N> Node;
public:
	BTree()
		:_root(NULL)
	{}

	pair<Node*, int> Find(const Key& key)
	{
		Node *cur = _root;
		Node *parent = NULL;
		while (cur){
			size_t i = 0;
			for (; i < cur->_size; ++i){
				if (key < _KeyOfValue()(cur->_valueField[i]))
					break;
				else if (key == _KeyOfValue()(cur->_valueField[i]))
					return make_pair(cur, i);
			}
			parent = cur;
			cur = cur->_subs[i];
		}

		return make_pair(parent, -1);
	}

	bool Insert(const Value& value)
	{
		//空树，直接插入
		if (NULL == _root)
		{
			_root = new Node();
			_root->_valueField[_root->_size] = value;
			++_root->_size;
			++_size;
			return true;
		}

		//不空则先查找
		pair<Node*, int> p = Find(_KeyOfValue()(value));
		//如果找到了，则退出
		if (-1 != p.second)
			return false;

		//没找到，插入
		Node *cur = p.first;
		_InsertValue(cur, value, NULL);

		//如果饱和了就分裂
		while (cur->_size >= N){
			int mid = cur->_size / 2;
			Node *newNode = new Node;

			//将后一半的数据移动到新节点
			size_t i = mid + 1, j = 0;
			size_t curSize = cur->_size;
			for (; i < curSize; ++j, ++i){
				newNode->_valueField[j] = cur->_valueField[i];
				cur->_valueField[i] = Value();
				--cur->_size;

				newNode->_subs[j] = cur->_subs[i];
				if (newNode->_subs[j])
					newNode->_subs[j]->_parent = newNode;
				++newNode->_size;
				cur->_subs[i] = NULL;
			}

			newNode->_subs[j] = cur->_subs[i];
			if (newNode->_subs[j])
				newNode->_subs[j]->_parent = newNode;
			cur->_subs[i] = NULL;

			//将中间的节点插到父节点中
			if (_root == cur){
				_root = new Node;
				_InsertValue(_root, cur->_valueField[mid], newNode);
				_root->_subs[0] = cur;
				cur->_parent = _root;
				newNode->_parent = _root;
			}
			else{
				Node *parent = cur->_parent;
				_InsertValue(parent, cur->_valueField[mid], newNode);
			}

			cur->_valueField[mid] = Value();
			--cur->_size;
			cur = cur->_parent;
		}

		++_size;
		return true;
	}

	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_root);
		cout << endl;
	}
protected:
	void _InOrder(Node *root)
	{
		if (NULL == root)
			return;

		size_t i = 0;
		for (; i < root->_size; ++i){
			_InOrder(root->_subs[i]);
			cout << root->_valueField[i] << ' ';
		}

		_InOrder(root->_subs[i]);
	}

	void _InsertValue(Node *cur, const Value& value, Node *sub)
	{
		int end = cur->_size;
		for (; end > 0; --end){
			if (_KeyOfValue()(value) < _KeyOfValue()(cur->_valueField[end - 1])){
				cur->_valueField[end] = cur->_valueField[end - 1];
				cur->_subs[end + 1] = cur->_subs[end];
			}
			else
				break;
		}

		cur->_valueField[end] = value;
		cur->_subs[end + 1] = sub;

		if(sub)
			sub->_parent = cur;

		++cur->_size;
	}
protected:
	Node *_root;
	size_t _size;
};

#endif

#if 0
//测试
#include "D:\Github\STL\Function.h"
void TestBTree()
{
#if 1
	const int N = 200;
	int a[N];
	RandomArrayUnique(a, N);
	BTree<int, int> bt;
	for (size_t i = 0; i < N; ++i)
		bt.Insert(a[i]);
	
	bt.InOrder();
	

#else
	BTree<int, int> bt;
	bt.Insert(53);
	bt.Insert(75);
	bt.Insert(139);
	bt.Insert(49);
	bt.Insert(145);
	bt.Insert(36);
	bt.Insert(101);
	bt.InOrder();
	//bt.Insert(10);
	//bt.Insert(30);
	//bt.Insert(20);
	//bt.Insert(12);
	//bt.Insert(15);
	//bt.Insert(32);
	//bt.Insert(25);
	//bt.InOrder();

#endif

}

#endif