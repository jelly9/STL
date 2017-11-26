/*
 *
 * 版权 :		(c) 2017
 * 模块名 :		STL
 * 文件名 :		RBTree.h
 * 作者 :		肖国栋
 * 创建日期 :	2017-11-3
 * 版本 :		xxxx.xx.xx
 * 相关文件 :	xxxx
 * 文件实现功能 :	(带头结点)红黑树的插入、旋转相关
 * 备注 :
 * 修改记录 :	2017-11-8
 *				2017-11-9
 *
 */

#ifndef __RBTREE_H__
#define __RBTREE_H__

#define TESTRBTREE 1

#include <iostream>
using namespace std;

#include <assert.h>

//红黑树节点颜色
enum Colour{ RED, BLACK };

//红黑树节点
template<class Value>
struct __RBTreeNode
{
	typedef __RBTreeNode<Value> Node;

	Value _valueField;//值域
	Colour _color;//颜色
	Node *_left;//三叉链
	Node *_right;
	Node *_parent;

	__RBTreeNode(const Value& valueField)
	:_valueField(valueField)
	,_color(RED)
	,_left(NULL)
	,_right(NULL)
	,_parent(NULL)
	{}

	static Node* Maximum(Node* cur)
	{
		assert(cur);
		while (cur->_right)
			cur = cur->_right;
		return cur;
	}
	static Node* Minimum(Node* cur)
	{
		assert(cur);
		while (cur->_left)
			cur = cur->_left;
		return cur;
	}
};

template<class Value, class Ref, class Ptr>
struct __RBTreeIterator
{
	typedef __RBTreeNode<Value>	Node;
	typedef __RBTreeIterator<Value, Value&, Value*>	Iterator;
	typedef __RBTreeIterator<Value, Ref, Ptr>	Self;
	Node *_node;

	__RBTreeIterator()
		:_node(NULL)
	{}
	__RBTreeIterator(Node* node)
		:_node(node)
	{}
	__RBTreeIterator(const Self& it)
		:_node(it._node)
	{}

	Ref operator*()const
	{
		return _node->_valueField;
	}
	Ptr operator->()const
	{
		return &(_node->_valueField);
	}

	Self& operator++()
	{
		_Increment();
		return *this;
	}
	Self operator++(int)
	{
		Self tmp(*this);
		_Increment();
		return tmp;
	}
	Self& operator--()
	{
		_Decrement();
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		_Decrement();
		return tmp;
	}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}
	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}

	void _Increment()
	{
		/*
		1. 如果右不空，直接指向右
		2. 如果右为空， 则向上找到第一个未访问的父结点
			带头结点则出特殊处理
		*/
		if (NULL != _node->_right){
			_node = _node->_right;
			while (_node->_left)
				_node = _node->_left;
		}
		else{
			Node* parent = _node->_parent;
			while (_node == parent->_right){
				_node = parent;
				parent = parent->_parent;
			}
			if (parent != _node->_right)
				_node = parent;
		}
	}
	void _Decrement()
	{
		/*
		1. 如果左不空，则直接指向左
		2. 左空则向上找到第一个未被访问的父节点
			带头结点则需特殊处理
		*/
		if (NULL != _node->_left){
			_node = _node->_left;
			while (_node->_right)
				_node = _node->_right;
		}
		else{
			Node *parent = _node->_parent;
			while (_node == parent->_left){
				_node = parent;
				parent = parent->_parent;
			}
			if (parent != _node->_left)
				_node = parent;
		}
	
	}
};

#if TESTRBTREE
template<class Value>
struct __KeyOfValue
{
	Value operator()(const Value& v)
	{
		return v;
	}
};
template<class Key, class Value>
struct __KeyOfPair
{
	Key operator()(const pair<Key, Value>& kv)
	{
		return kv.first;
	}
};
#endif

//红黑树
template<class Key, class Value, class _KeyOfValue = __KeyOfValue<Value>>
class RBTree
{
	typedef __RBTreeNode<Value> Node;
public:
	typedef __RBTreeIterator<Value, Value&, Value*>				Iterator;
	typedef __RBTreeIterator<Value, const Value&, const Value*> ConstIterator;
public:
	RBTree()
		:_header(new Node(Value()))
	{
		_header->_left = _header;
		_header->_right = _header;
	}

	Iterator Find(const Key& key)
	{
		Node *root = _Root();
		if (NULL == root)
			return false;

		Node *cur = root;
		while (cur){
			if (key < _KeyOfValue()(cur->_valueField))
				cur = cur->_left;
			else if (key > _KeyOfValue()(cur->_valueField))
				cur = cur->_right;
			else
				return cur;
		}

		return NULL;
	}

	//modify
	pair<Iterator, bool> Insert(const Value& value)
	{
		Node *&root = _Root();

		//对于空树需特殊处理
		if (NULL == root){
			root = new Node(value);
			//_root = new Node(Value);
			root->_parent = _header;
			_header->_parent = root;
			root->_color = BLACK;
			_header->_left = __RBTreeNode<Value>::Minimum(root);
			_header->_right = __RBTreeNode<Value>::Maximum(root);
			++_nodeCount;
			return make_pair(Iterator(root), true);
		}

		/*
		1. 寻找插入位置
		2. 插入节点
		3. 调整颜色，可能需要旋转
		*/

		Node *cur = root;
		Node *parent = NULL;//_header;
		_KeyOfValue KeyOfValue;
		while (cur){
			/*stl
			parent = cur;
			comp = keyCompare(keyOfValue(value), _Key(cur));
			cur = comp ? cur->_left : cur->_right;*/
			parent = cur;
			if (KeyOfValue(value) < KeyOfValue(cur->_valueField))
				cur = cur->_left;
			else if (KeyOfValue(value) > KeyOfValue(cur->_valueField))
				cur = cur->_right;
			else
				return make_pair(Iterator(cur), false);
		}
		//cur = new Node(value);
		//comp ? parent->_left = cur : parent->_right;
		//cur->_parent = parent;
		cur = new Node(value);
		Node* newNode = cur;
		if (KeyOfValue(value) < KeyOfValue(parent->_valueField))
			parent->_left = cur;
		else
			parent->_right = cur;
		cur->_parent = parent;
		++_nodeCount;
		
		//调整颜色，旋转树
		while (cur != root && RED == parent->_color){
			Node* gParent = parent->_parent;
			//父亲是祖父的左
			if (parent == gParent->_left){
				Node* uncle = gParent->_right;
				//叔叔节点存在且为红色
				if (uncle && RED == uncle->_color){
					uncle->_color = BLACK;
					parent->_color = BLACK;
					gParent->_color = RED;
					cur = gParent;
					parent = cur->_parent;
				}
				//叔叔节点为黑色或者不存在，则需要旋转
				else{
					if (cur == parent->_right){
						//cout << value << " RotateL" << endl;
						_RotateL(parent);
						std::swap(cur, parent);
					}
					//cout << value << " RotateR" << endl;

					_RotateR(gParent);
					parent->_color = BLACK;
					gParent->_color = RED;
					break;
				}
			
			}
			//父亲是祖父的右
			else{
				Node* uncle = gParent->_left;
				//叔叔节点存在且为红色
				if (uncle && RED == uncle->_color){
					parent->_color = BLACK;
					uncle->_color = BLACK;
					gParent->_color = RED;
					cur = gParent;
					parent = cur->_parent;
				}
				//叔叔节点为黑色或者不存在，则需要旋转
				else{
					if (cur == parent->_left){
						//cout << value << " RotateR" << endl;
						_RotateR(parent);
						std::swap(cur, parent);
					}
					//cout << value << " RotateL" << endl;
					_RotateL(gParent);
					parent->_color = BLACK;
					gParent->_color = RED;
					break;
				}

			}
		}

		_header->_left = __RBTreeNode<Value>::Minimum(root);
		_header->_right = __RBTreeNode<Value>::Maximum(root);
		root->_color = BLACK;
		return make_pair(Iterator(newNode), true);
	}

	bool IsBlance()
	{
		Node *root = _Root();

		if (RED == root->_color)
			return false;

		Node *cur = root;
		size_t blackCount = 0;
		size_t size = 0;
		while (cur){
			if (BLACK == cur->_color)
				++size;
			cur = cur->_left;
		}

		return _IsBlance(_Root(), size, blackCount);
	}
	
	//Iterator
	Iterator Begin()
	{
		return _header->_left;
	}
	ConstIterator Begin()const
	{
		return _header->_left;
	}
	Iterator End()
	{
		return _header;
	}
	ConstIterator End()const
	{
		return _header;
	}
	
protected:
	//typedef rb_tree_node* link_type;
	//link_type& root() const { return (link_type&)header->parent; }
	Node*& _Root()
	{
		return _header->_parent;
	}

	//旋转
	void _RotateL(Node* node)
	{
		Node*& root = _Root();

		Node* subR = node->_right;
		Node* subRL = subR->_left;

		node->_right = subRL;
		if (subRL)
			subRL->_parent = node;

		subR->_parent = node->_parent;
		if (node == root)
			//_root = subR;
			_header->_parent = subR;
		else if (node == node->_parent->_left)
			node->_parent->_left = subR;
		else
			node->_parent->_right = subR;

		subR->_left = node;
		node->_parent = subR;
	}
	void _RotateR(Node* node)
	{
		Node*& root = _Root();

		Node *subL = node->_left;
		Node *subLR = subL->_right;

		node->_left = subLR;
		if (subLR)
			subLR->_parent = node;

		subL->_parent = node->_parent;
		if (node == root)
			node->_parent->_parent = subL;//_root = subL;
		else if (node == node->_parent->_left)
			node->_parent->_left = subL;
		else
			node->_parent->_right = subL;

		subL->_right = node;
		node->_parent = subL;
	}

	bool _IsBlance(Node *cur, const size_t size, size_t blackCount)
	{
		if (NULL == cur){
			if (blackCount != size){
				cout << "黑色节点个数不相等!" << endl;
				return false;
			}
			else
				return true;
		}

		if (RED == cur->_color && RED == cur->_parent->_color){
			cout << "出现两个连续的红色节点!" << endl;
			return false;
		}

		if (BLACK == cur->_color)
			++blackCount;

		return _IsBlance(cur->_left, size, blackCount)
			&& _IsBlance(cur->_right, size, blackCount);
	}
protected:
	Node *_header;
	size_t _nodeCount;
};

#endif

#if TESTRBTREE
#include <map>
#include <string>
#include "D:\Github\STL\Function.h"

void TestRBTree()
{
	const int N = 100;
	int a[N];
	RandomArray(a, N);

	//int a[] = {1, 3, 2, 4, 5, 6, 7};

	RBTree<int, int> rt;
	
	for (size_t i = 0; i < N; ++i){
		rt.Insert(a[i]);
		rt.Find(a[i]);
		cout << " IsBlance()? "<< rt.IsBlance() << endl;
	}
	PrintContainer(rt);
}

void TestRBTree2()
{
	RBTree<string, pair<string, string>, __KeyOfPair<string, string>> dict;


	dict.Insert(make_pair("sort", "排序"));
	dict.Insert(make_pair("string", "字符串"));
	dict.Insert(make_pair("left", "左边"));
	dict.Insert(make_pair("right", "右边"));
	dict.Insert(make_pair("left", "剩余"));

}

//#include <stl_rb_tree.h>
void test_rb_tree()
{


}
#endif