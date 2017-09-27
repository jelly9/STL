#ifndef __THREADBINARYTREE_H__
#define __THREADBINARYTREE_H__

#include <iostream>
using namespace std;

#include <assert.h>

#define POLYMORPHIC 1 //多态版本（1）

enum TAG
{
	THREAD, 
	LINK,
};
//线索二叉树的节点
template<class T>
struct ThreadBinaryTreeNode
{
	ThreadBinaryTreeNode(const T& value)
	:_value(value)
	,_left(NULL)
	,_right(NULL)
	, _leftTag(LINK)
	, _rightTag(LINK)
	{}
	T _value;
	ThreadBinaryTreeNode<T> *_left;
	ThreadBinaryTreeNode<T> *_right;
	TAG _leftTag;
	TAG _rightTag;
};
#if POLYMORPHIC
///////////////////////////////////////
//基类迭代器
template<class T, class Ref, class Ptr>
class BaseIterator
{
protected:
	typedef ThreadBinaryTreeNode<T> Node;
	typedef BaseIterator<T, Ref, Ptr> Self;
public:
	BaseIterator()
		:_node(NULL)
	{}
	BaseIterator(Node *node)
		:_node(node)
	{}
	BaseIterator(const Self& it)
		:_node(it._node)
	{}

	//基类的自增函数为虚函数，在派生类中重新实现且覆盖之
	virtual Self& operator++(){ return *this; }
	virtual Self operator++(int){ return *this; }

	Ref operator*()
	{
		return _node->_value;
	}

	const Ref operator*()const
	{
		return _node->_value;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	const Ptr operator->()const
	{
		return &(operator*());
	}

	bool operator==(Self& it)
	{
		return it->_node == _node;
	}

	bool operator!=(Self& it)
	{
		return it._node != _node;
	}


protected:
	Node *_node;
};
//前序迭代器
template<class T, class Ref, class Ptr>
class PreIterator :public BaseIterator<T, Ref, Ptr>
{
public:
	PreIterator()
	{}
	PreIterator(Node *node)
		:BaseIterator(node)
	{}
	PreIterator(Self& it)
		:BaseIterator(it)
	{}
	//只需实现自增操作，其它操作均继承自基类
	Self& operator++()
	{
		_Increase();
		return *this;
	}
	Self operator++(int)
	{
		Self temp(_node);
		_Increase();
		return temp;
	}
protected:
	void _Increase()
	{
		assert(_node);

		if (LINK == _node->_leftTag)
			_node = _node->_left;
		else
			_node = _node->_right;
	}
};
//中序迭代器
template<class T, class Ref, class Ptr>
class InIterator :public BaseIterator<T, Ref, Ptr>
{
public:
	InIterator()
	{}

	InIterator(Node *node)
		:BaseIterator(node)
	{}
	InIterator(Self& it)
		:BaseIterator(it)
	{}
	//只需实现自增操作，其它操作均继承自基类
	Self& operator++()
	{
		_Increase();
		return *this;
	}
	Self operator++(int)
	{
		Self temp(_node);
		_Increase();
		return temp;
	}
protected:
	void _Increase()
	{
		assert(_node);

		if (LINK == _node->_rightTag){
			_node = _node->_right;
			while (_node && LINK == _node->_leftTag)
				_node = _node->_left;
		}
		else
			_node = _node->_right;
	}
};
#else
////////////////////////////////////////////////////
//前序迭代器
template<class T, class Ref, class Ptr>
class PreIterator
{
	typedef ThreadBinaryTreeNode<T> Node;
	typedef PreIterator<T, T&, T*> Self;
public:
	PreIterator()
		:_node(NULL)
	{}
	PreIterator(Node *node)
		:_node(node)
	{}

	PreIterator(Self& it)
		:_node(it._node)
	{}

	Self& operator++()//前置++
	{
		_Increase();
		return *this;
	}

	Self operator++(int)//后置++
	{
		Self temp(_node);
		_Increase();
		return temp;
	}

	//运算符重载
	Ref operator*()
	{
		return _node->_value;
	}

	const Ref operator*()const
	{
		return _node->_value;
	}

	Ptr operator->()
	{
		return &(operator*());
	}
	
	const Ptr operator->()const 
	{
		return &(operator*());
	}

	bool operator==(const Self& it)
	{
		return it._node == _node;
	}

	bool operator!=(const Self& it)
	{
		return !(operator==(it));
	}

protected:
	void _Increase()
	{
		assert(_node);

		if (LINK == _node->_leftTag)
			_node = _node->_left;
		else
			_node = _node->_right;
	}
private:
	Node *_node;
};
////////////////////////////////////////////////////////
//中序迭代器
template<class T, class Ref, class Ptr>
class InIterator
{
	typedef ThreadBinaryTreeNode<T> Node;
	typedef InIterator<T, Ref, Ptr> Self;
public:

	InIterator()
		:_node(NULL)
	{}
	InIterator(Node *node)
		:_node(node)
	{}
	InIterator(Self& it)
		:_node(it._node)
	{}

	Self& operator++()
	{
		_Increase();
		return *this;
	}
	Self operator++(int)
	{
		Self temp(_node);
		_Increase();
		return temp;
	}

	Ref operator*()
	{
		return _node->_value;
	}

	const Ref operator*()const
	{
		return _node->_value;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	const Ptr operator->()const
	{
		return &(operator*());
	}

	bool operator==(Self& it)
	{
		return it->_node == _node;
	}

	bool operator!=(Self& it)
	{
		return it._node != _node;
	}

protected:
	void _Increase()
	{
		assert(_node);

		if (LINK == _node->_rightTag){
			_node = _node->_right;
			while (_node && LINK == _node->_leftTag)
				_node = _node->_left;
		}
		else
			_node = _node->_right;
	}
private:
	Node *_node;
};
#endif

/////////////////////////////////////
//线索二叉树
template<class T>
class ThreadBinaryTree
{
	typedef ThreadBinaryTreeNode<T> Node;
public:
	typedef PreIterator<T, T&, T*> PreIterator;
	typedef InIterator<T, T&, T*> InIterator;

public:
#if POLYMORPHIC
	typedef BaseIterator<T, T&, T*> Iterator;
#endif

public:
	ThreadBinaryTree()
		:_root(NULL)
	{}
	//构造函数
	ThreadBinaryTree(const T a[], const size_t& size, const T& invalid)
	{
		int index = 0;
		_root = _ConstructTree(a, size, index, invalid);
	}

	//前序线索化
	void PreThread()
	{
		Node *pre = NULL;
		_PreThread(_root, pre);
	}

	PreIterator PreBegin()
	{
		return _root;
	}

	PreIterator PreEnd()
	{
		return NULL;
	}

	//中序线索化
	void InThread()
	{
		Node *pre = NULL;
		_InThread(_root, pre);
	}

	InIterator InBegin()
	{
		if (NULL == _root)
			return NULL;

		Node *cur = _root;
		while (LINK == cur->_leftTag)
			cur = cur->_left;
		return cur;
	}

	InIterator InEnd()
	{
		return NULL;
	}

	//后序线索化
	void PostThread()
	{
		Node *pre = NULL;
		_PostThread(_root, pre);
	}
private:
	//前序线索化
	//参数一为当前线索的节点，二为上一个线索的节点
	void _PreThread(Node *cur, Node* &pre)
	{
		if (NULL == cur)
			return;
		//线索化上一个节点的右孩子
		if (pre && NULL == pre->_right){
			pre->_right = cur;
			pre->_rightTag = THREAD;
		}
		//线索当前节点的左孩子 
		if (NULL == cur->_left){
			cur->_left = pre;
			cur->_leftTag = THREAD;
		}
		
		pre = cur;

		if(LINK == cur->_leftTag)
			_PreThread(cur->_left, pre);
		if(LINK == cur->_rightTag)
			_PreThread(cur->_right, pre);
	}

	//中序线索化
	void _InThread(Node *cur, Node *&pre)
	{
		if (NULL == cur)
			return;

		if (LINK == cur->_leftTag)
			_InThread(cur->_left, pre);
		
		//线索当前上一个节点的右
		if (pre && NULL == pre->_right){
			pre->_right = cur;
			pre->_rightTag = THREAD;
		}
		//线索当前节点的左
		if (NULL == cur->_left){
			cur->_left = pre;
			cur->_leftTag = THREAD;
		}

		pre = cur;

		if (LINK == cur->_rightTag)
			_InThread(cur->_right, pre);
	}

	//后序线索化
	void _PostThread(Node *cur, Node *&pre)
	{
		if (NULL == cur)
			return;

		if (LINK == cur->_leftTag)
			_PostThread(cur->_left, pre);

		if (LINK == cur->_rightTag)
			_PostThread(cur->_right, pre);

		if (pre && NULL == pre->_right){
			pre->_right = cur;
			pre->_rightTag = THREAD;
		}

		if (NULL == cur->_left){
			cur->_left = pre;
			cur->_leftTag = THREAD;
		}

		pre = cur;
	}

	//参数一为构建树所需数组，二为数组大小， 参数三为当前访问元素的下标，四为无效值
	Node* _ConstructTree(const T a[], const size_t& size, int& index, const T& invalid)
	{
		if (index >= size || invalid == a[index])
			return NULL;

		Node *root = new Node(a[index]);
		root->_left = _ConstructTree(a, size, ++index, invalid);
		root->_right = _ConstructTree(a, size, ++index, invalid);
		return root;
	}

private:
	Node *_root;
};


#endif

void TestThreadBinaryTree()
{
#if !(POLYMORPHIC)
	//普通版本
	int a1[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	ThreadBinaryTree<int> t1(a1, sizeof(a1)/sizeof(a1[0]), '#');
	t1.PreThread();
	ThreadBinaryTree<int>::PreIterator pit = t1.PreBegin();

	while (pit != t1.PreEnd()){
		cout << *pit << ' ';
		pit++;
	}
	cout << endl;

	int a2[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	ThreadBinaryTree<int> t2(a2, sizeof(a2) / sizeof(a2[0]), '#');
	t2.InThread();

	ThreadBinaryTree<int>::InIterator iit = t2.InBegin();
	while (iit != t2.InEnd()){
		cout << *iit << ' ';
		iit++;
	}
	cout << endl;

	int a3[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	ThreadBinaryTree<int> t3(a3, sizeof(a3) / sizeof(a3[0]), '#');
	t3.PostThread();
#else
	//多态版本
	int a4[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	ThreadBinaryTree<int> t4(a4, sizeof(a4) / sizeof(a4[0]), '#');
	t4.PreThread();
	ThreadBinaryTree<int>::Iterator& it4 = t4.PreBegin();
	while (it4 != t4.PreEnd())
	{
		cout << *it4 << " ";
		++it4; 
	}
	cout << endl;

	int a5[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	ThreadBinaryTree<int> t5(a5, sizeof(a5) / sizeof(a5[0]), '#');
	t5.InThread();
	ThreadBinaryTree<int>::Iterator& it5 = t5.InBegin();
	while (it5 != t5.InEnd())
	{
		cout << *it5 << " ";
		++it5;
	}
#endif
}