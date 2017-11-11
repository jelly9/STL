#ifndef __MY_AVLTREE_H__
#define __MY_AVLTREE_H__

#include <iostream>
using namespace std;
#include <assert.h>

template<class K, class V>
struct AVLTreeNode//AVLTrees树节点，三叉链
{
	AVLTreeNode(const K& key, const V& value)
	:_bf(0)
	,_key(key)
	, _value(value)
	, _left(NULL)
	, _right(NULL)
	, _parent(NULL)
	{}
	int _bf;//平衡因子 = 右高度 - 左高度
	K _key;
	V _value;
	AVLTreeNode<K, V> *_left;
	AVLTreeNode<K, V> *_right;
	AVLTreeNode<K, V> *_parent;
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (NULL == _root){
			_root = new Node(key, value);
			return true;
		}

		Node *cur = _root;
		Node *parent = NULL;

		//找插入位置
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

		//插入结点
		cur = new Node(key, value);
		if (key < parent->_key)
			parent->_left = cur;
		else
			parent->_right = cur;
		cur->_parent = parent;

		//调整平衡因子
		while (parent){
			if (cur == parent->_left)
				--parent->_bf;
			else
				++parent->_bf;

			if (0 == parent->_bf)
				break;
			else if (1 == parent->_bf || -1 == parent->_bf){//	+/-1		继续向上调整
				cur = parent;
				parent = parent->_parent;
			}
			else{//	+/-2		可以旋转了
				if (2 == parent->_bf){//	2
					if (1 == cur->_bf)//	2	1	右-右
						_RotateL(parent);
					else//	2	-1		右-左
						_RotateRL(parent);
				}
				else{//	-2
					if (-1 == cur->_bf)//-2	-1	左-左
						_RotateR(parent);
					else//-2	1	左-右
						_RotateLR(parent);
				}
				break;
			}
		}
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool IsBlance()
	{
		int height = 0;
		return _IsBlance(_root, height);
	}

	~AVLTree()
	{
		_Destroy(_root);
		_root = NULL;
	}
protected:
	bool _IsBlance(Node *root, int& height)
	{
		if (NULL == root){
			height = 0;
			return true;
		}

		int leftHeight = 0;
		int rightHeight = 0;

		if (!_IsBlance(root->_left, leftHeight) 
			|| !_IsBlance(root->_right, rightHeight))
			return false;

		if ((rightHeight - leftHeight) > 1
			|| (rightHeight - leftHeight) < -1){
			cout << "平衡因子出错！"<< root->_key <<"->"<<root->_bf<<'('<<rightHeight-leftHeight<<')'<< endl;
			return false;
		}
		height = (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
		return true;
	}
	void _RotateL(Node *parent)//左旋
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		Node* pparent = parent->_parent;
		subR->_parent = pparent;
		
		if (NULL == pparent){
			_root = subR;
			subR->_parent = NULL;
		}
		else if (parent == pparent->_left)
			pparent->_left = subR;
		else
			pparent->_right = subR;

		subR->_left = parent;
		parent->_parent = subR;

		subR->_bf = 0;
		parent->_bf = 0;
	}
	void _RotateR(Node *parent)//右旋
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node *pparent = parent->_parent;
		subL->_parent = pparent;

		if (NULL == pparent){
			_root = subL;
			subL->_parent = NULL;
		}
		else if (parent == pparent->_left)
			pparent->_left = subL;
		else
			pparent->_right = subL;

		subL->_right = parent;
		parent->_parent = subL;

		subL->_bf = 0;
		parent->_bf = 0;
	}
	void _RotateLR(Node *parent)//左右双旋
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		int bf = subLR->_bf;

		_RotateL(parent->_left);
		_RotateR(parent);

		if (1 == bf)
			subL->_bf = -1;
		else if(-1 == bf)
			parent->_bf = 1;
	}
	void _RotateRL(Node *parent)//右左双旋
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		int bf = subRL->_bf;

		_RotateR(parent->_right);
		_RotateL(parent);
	
		if (1 == bf)
			parent->_bf = -1;
		else if(-1 == bf)
			subR->_bf = 1;
	}
	void _InOrder(Node *root)
	{
		if (NULL == root)
			return;
		_InOrder(root->_left);
		cout << root->_key << "->" << root->_value <<"->"<< root->_bf<< endl;
		_InOrder(root->_right);
	}
	void _Destroy(Node *root)
	{
		if (NULL == root)
			return;

		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
	}

protected:
	Node *_root;
};

#if 0

#include "D:\Github\STL\Function.h"

void TestAVLTree()
{
	int arr[100];
	RandomArrayUnique(arr, sizeof(arr)/sizeof(int), 0, 1000000);
	AVLTree<int, int> t;

	for (size_t i = 0; i < sizeof(arr) / sizeof(int); ++i){
		t.Insert(arr[i], i);
		cout << "IsBlance? " << t.IsBlance() << endl;;
	}

	t.InOrder();
}

#endif
#endif