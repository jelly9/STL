/*
内容：二叉搜索树
时间：2017.10.27
*/

#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#pragma once

#include <iostream>
using namespace std;
#include <assert.h>

template<class T>
struct __BinarySearchTreeNode
{
	__BinarySearchTreeNode(const T& key)
	:_key(key)
	,_parent(NULL)
	,_left(NULL)
	,_right(NULL)
	{}
	typedef __BinarySearchTreeNode<T> Node;
	T _key;
	Node *_left;
	Node *_right;
	Node *_parent;
};

template<class T>
class BinarySearchTree
{
	typedef __BinarySearchTreeNode<T> Node;
public:
	BinarySearchTree()
		:_root(NULL)
	{}

	//插入
	bool Insert(const T& key)
	{
		//如果树为空则直接构造一个Node给根
		if (NULL == _root){
			_root = new Node(key);
			return true;
		}
		//否则先找到插入位置，在构造节点插入
		else{
			Node *cur = _root;
			Node *parent = NULL;
			while (cur){
				if (key < cur->_key){
					parent = cur;
					cur = cur->_left;
				}
				else if (key > cur->_key){
					parent = cur;
					cur = cur->_right;
				}
				else
					return false;
			}

			//申请节点并插入
			cur = new Node(key);
			if (key < parent->_key)
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;

			return true;
		}
	}

	//查找
	Node* Find(const T& key)
	{
		Node *cur = _root;
		while (NULL != cur && key != cur->_key){
			if (key < cur->_key)
				cur = cur->_left;
			else
				cur = cur->_right;
		}

		return cur;
	}

	bool Remove(const T& key)
	{
		Node *cur = _root;
		Node *parent = NULL;

		/*
		1. 先找到要删除的节点
		2. 删除
			1) 待删除节点左右都不为空,用左子树最右节点（右子树最左节点）替换(特殊处理根节点)
			2) 待删除节点左为空，让父亲链像待删除节点右子树
			3）待删除节点右为空，让父亲链向待删除节点左子树
		*/
		while (cur){
			//找删除节点
			if (key < cur->_key){
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key){
				parent = cur;
				cur = cur->_right;
			}
			else{//找到了
				if (cur->_left && cur->_right){//左右都不为空
					Node *subLMostR = cur->_left;
					while (subLMostR->_right)//找左子树最右节点
						subLMostR = subLMostR->_right;

					cur->_key = subLMostR->_key;//替换

					if (subLMostR == subLMostR->_parent->_right)
						subLMostR->_parent->_right = subLMostR->_left;
					else
						subLMostR->_parent->_left = subLMostR->_left;

					if (subLMostR->_left)
						subLMostR->_left->_parent = subLMostR->_parent;

					delete subLMostR;
					subLMostR = NULL;
					return true;
				}
				else{// 左为空 或者 右为空 （或者左右都为空）
					Node* linkNode = NULL;
					if (NULL == cur->_left)//左为空
						linkNode = cur->_right;
					else if (NULL == cur->_right)//右为空
						linkNode = cur->_left;

					if (cur != _root){
						if (cur == cur->_parent->_right)
							cur->_parent->_right = linkNode;
						else
							cur->_parent->_left = linkNode;
					}
					else
						_root = linkNode;

					if(linkNode)//更新父节点
						linkNode->_parent = cur->_parent;

					delete cur;
					cur = NULL;
					return true;
				}
			}
		}
		return false;
	}

	//递归插入
	bool InsertR(const T& key)
	{
		if (NULL == _root){
			_root = new Node(key);
			return true;
		}
		else
			return _Insert(_root, NULL, key);
	}

	Node* FindR(const T& key)
	{
		return _Find(_root, key);
	}
	//递归删除
	bool RemoveR(const T& key)
	{
		if (NULL == _root)
			return false;

		return _Remove(_root, key);

	}

	Node* ToSortList()
	{
		Node *prev = NULL;

		_ToSortList(_root, prev);

		Node *head = _root;

		while (head && head->_left)
			head = head->_left;

		return head;
	}
	
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	~BinarySearchTree()
	{
		_Destroy(_root);
		_root = NULL;
	}
protected:

	void _ToSortList(Node *cur, Node *&prev)
	{
		if (NULL == cur)
			return;

		_ToSortList(cur->_left, prev);

		cur->_left = prev;
		if (NULL != prev)
			prev->_right = cur;

		prev = cur;

		_ToSortList(cur->_right, prev);
	}

	bool _Remove(Node* root, const T& key)
	{
		if (NULL == root)
			return false;

		if (key < root->_key)
			return _Remove(root->_left, key);
		else if (key > root->_key)
			return _Remove(root->_right, key);
		//走到这儿则说明当前节点即是待删除接节点
		else{
			//左右都不为空
			if (root->_left && root->_right){
				Node *subLMostR = root->_left;
				while (subLMostR->_right)//找左子树最右节点
					subLMostR = subLMostR->_right;

				root->_key = subLMostR->_key;//替换

				return _Remove(subLMostR, subLMostR->_key);
			}
			else{
				Node* linkNode = NULL;
				if (NULL == root->_left)//左为空
					linkNode = root->_right;
				else if (NULL == root->_right)//右为空
					linkNode = root->_left;

				if (root != _root){
					if (root == root->_parent->_right)
						root->_parent->_right = linkNode;
					else
						root->_parent->_left = linkNode;
				}
				else
					_root = linkNode;

				if (linkNode)//更新父节点
					linkNode->_parent = root->_parent;

				delete root;
				root = NULL;
				return true;
			}
		}
	}

	Node* _Find(Node* root, const T& key)
	{
		if (NULL == root || key == root->_key)
			return root;

		if (key < root->_key)
			return _Find(root->_left, key);
		else 
			return _Find(root->_right, key);
	}
	bool _Insert(Node *root, Node *parent, const T& key)
	{
		if (NULL == root){
			Node *cur = new Node(key);
			if (key < parent->_key)
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;

			return true;
		}

		if (key < root->_key)
			return _Insert(root->_left,root, key);
		else if (key > root->_key)
			return _Insert(root->_right, root, key);
		else
			return false;

	}
	void _Destroy(Node* root)
	{
		if (NULL == root)
			return;

		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
		root = NULL;
	}
	void _InOrder(Node* root)
	{
		if (NULL == root)
			return;

		_InOrder(root->_left);
		cout << root->_key << ' ';
		_InOrder(root->_right);
	}

protected:
	Node *_root;
};


#if 1

#include "D:\Github\STL\Function.h"
void TestBSTree()
{
#if 0
	int a[20];
	RandomArray(a, sizeof(a)/sizeof(int));//用随机数[0, 100）填充数组 a[]
#else
	int a[] = {5, 3, 4, 1, 7, 8, 2, 6, 0, 9};
#endif

	BinarySearchTree<int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
		t.InsertR(a[i]);

	__BinarySearchTreeNode<int> * head = t.ToSortList();
	__BinarySearchTreeNode<int> *cur = head;
	while (cur && cur->_right){
		cout << cur->_key << ' ';
		cur = cur->_right;
	}

	//cout << t.FindR(a[1])->_key << endl;
	//t.InOrder();
	//t.RemoveR(a[3]);
	//t.RemoveR(a[2]);
	//t.RemoveR(a[0]);
	//t.RemoveR(a[1]);
	//t.RemoveR(a[5]);
	//t.RemoveR(a[7]);
	//t.RemoveR(a[8]);
	//t.RemoveR(a[6]);
	//t.RemoveR(a[9]);
	//t.RemoveR(a[4]);
	//t.InOrder();

}

#endif

#endif