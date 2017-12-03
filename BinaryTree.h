#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <iostream>
using namespace std;

#include <stack>
#include <queue>
#include <assert.h>


//二叉树节点
template<class T>
struct __BinaryTreeNode
{
	__BinaryTreeNode(const T& value)
		:_value(value)
		,_left(NULL)
		,_right(NULL)
	{}

	T _value;
	__BinaryTreeNode<T> *_left;
	__BinaryTreeNode<T> *_right;
};

//二叉树
template<class T>
class BinaryTree
{
	typedef __BinaryTreeNode<T> Node;
public:
	//构造函数
	BinaryTree()
	{}

	//参数一为构建树所需的数组，参数二为数组大小，参数三为非法值
	BinaryTree(const T a[], const size_t& size, const T& invalid)
	{
		size_t index = 0;
		_root = _ConstructTree(a, size, index, invalid);
	}

	//前序遍历
	void PreOrder()
	{
		_PreOrder(_root);
		cout << endl;
	}

	//中序遍历
	void  InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//后序遍历
	void PostOrder()
	{
		_PostOrder(_root);
		cout << endl;
	}

	//非递归前序遍历
	void PreOrderNoR()
	{
		Node* cur = _root;
		stack<Node*> s;
		while (cur || !s.empty()){
			while (cur){//遍历左路节点
				s.push(cur);
				cout << cur->_value << ' ';
				cur = cur->_left;
			}
			//遍历右子树
			cur = s.top();
			s.pop();
			cur = cur->_right;
		}
		cout << endl;
	}

	//非递归中序遍历
	void InOrderNoR()
	{
		Node* cur = _root;
		stack<Node*> s;
		while (cur || !s.empty()){
			while (cur){
				s.push(cur);
				cur = cur->_left;
			}
			cur = s.top();
			cout << cur->_value << ' ';
			s.pop();
			cur = cur->_right;
		}
		cout << endl;
	}

	//非递归后序遍历
	void PostOrderNoR()
	{
		Node* cur = _root;
		Node* pre = NULL;
		stack<Node*> s;
		while (cur || !s.empty()){

			while (cur){
				s.push(cur);
				cur = cur->_left;
			}
			//左子树已经为空
			Node* top = s.top();
			//1，如果右子树为空则可直接访问该节点
			//2，如果右子树被访问过则也可直接访问该节点
			if (NULL == top->_right || pre == top->_right){
				cout << top->_value << ' ';
				pre = top;
				s.pop();
			}
			else
				cur = top->_right;
		}
		cout << endl;
	}

	//层序遍历
	void LevelOrder()
	{
		if (NULL == _root)
			return;

		Node* cur = _root;
		queue<Node*> q;
		q.push(cur);
		while (!q.empty()){
			cur = q.front();
			cout << cur->_value << ' ';
			q.pop();
			if (cur->_left)
				q.push(cur->_left);
			if (cur->_right)
				q.push(cur->_right);
		}
		cout << endl;
	}

	//总节点个数
	size_t Size()
	{
		return _Size(_root);
	}
	//树的高度
	size_t Height()
	{
		return _Height(_root);
	}
	//叶子节点个数
	size_t LeafNode()
	{
		return _LeafNode(_root);
	}
	//第K层节点个数
	size_t LevelKNode(int k)
	{
		assert(k > 0);
		return _LevelKNode(_root, k);
	}

	Node* Find(const T& value)
	{
		return _Find(_root, value);
	}
	
	////////////////////
	//获取最近公共祖先
	Node* GetCommenAncestor(Node *x1, Node *x2)
	{
		stack<Node*> path1;
		stack<Node*> path2;

		_GetPath(_root, x1, path1);
		_GetPath(_root, x2, path2);

		if (path1.empty() || path2.empty())
			return NULL;

		Node* ancestor = path1.top();
		while (!path1.empty() && !path2.empty()){
			ancestor = path1.top();

			path1.pop();
			path2.pop();

			if (!path1.empty() && !path2.empty()
				&& path1.top() != path2.top())
				break;
		}

		return ancestor;
	}

	//获取最远节点距离
	int MaxLen()
	{
		int maxLen = 0;
		_MaxLen(_root, maxLen);
		return maxLen;
	}


	/////////////////////////////

	//析构函数
	~BinaryTree()
	{
		_DestroyTree(_root);
	}
protected:
	int _MaxLen(Node *root, int& maxLen)
	{
		if (NULL == root)
			return 0;
		int leftLen = _MaxLen(root->_left, maxLen);
		int rightLen = _MaxLen(root->_right, maxLen);

		if (maxLen < (leftLen + rightLen))
			maxLen = leftLen + rightLen;

		return leftLen > rightLen ? leftLen + 1 : rightLen + 1;
	}

	Node* _Find(Node *root, const T& value)
	{
		if (NULL == root)
			return NULL;

		if (value == root->_value)
			return root;

		Node* retL = _Find(root->_left, value);

		if (retL)
			return retL;
		else
			return _Find(root->_right, value);
	}

	bool _GetPath(Node *root, Node* cur, stack<Node*>& path)
	{
		if (NULL == root)
			return false;

		if (root == cur){
			path.push(root);
			return true;
		}

		if (_GetPath(root->_left, cur, path) || _GetPath(root->_right, cur, path)){
			path.push(root);
			return true;
		}

		return false;
	}
	size_t _LevelKNode(Node* root, int k)
	{
		if (NULL == root)
			return 0;

		if (1 == k)
			return 1;

		return _LevelKNode(root->_left, k-1) + _LevelKNode(root->_right, k-1);
	}

	size_t _LeafNode(Node* root)
	{
		if (NULL == root)
			return 0;

		if (NULL == root->_left && NULL == root->_right)
			return 1;

		return _LeafNode(root->_left) + _LeafNode(root->_right);
	}

	size_t _Height(Node* root)
	{
		if (NULL == root)
			return 0;

		size_t leftHeight = _Height(root->_left);
		size_t rightHeight = _Height(root->_right);
	
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	size_t _Size(Node* root)
	{
		if (NULL == root)
			return 0;

		return _Size(root->_left) + _Size(root->_right) + 1;
	}

	void _PreOrder(const Node* root)
	{
		if (NULL == root)
			return;
		cout << root->_value << ' ';
		_PreOrder(root->_left);
		_PreOrder(root->_right);
	}

	void _InOrder(const Node* root)
	{
		if (NULL == root)
			return;
		_InOrder(root->_left);
		cout << root->_value << ' ';
		_InOrder(root->_right);
	}

	void _PostOrder(const Node* root)
	{
		if (NULL == root)
			return;
		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_value << ' ';
	}

	Node* _ConstructTree(const T* a, size_t size, size_t& index, const T& invalid)
	{
		if (index >= size || a[index] == invalid)
			return NULL;
		
		Node* root = new Node(a[index]);
		root->_left = _ConstructTree(a, size, ++index, invalid);
		root->_right = _ConstructTree(a, size, ++index, invalid);

		return root;
	}

	void _DestroyTree(Node* root)
	{
		if (NULL == root)
			return;

		_DestroyTree(root->_left);
		_DestroyTree(root->_right);
		delete root;
	}
private:
	Node *_root;
};


#endif

typedef __BinaryTreeNode<int> Node;

static Node* __RebuildTree(vector<int> preOrder, vector<int> inOrder
	,int& preIndex, int begin, int end)
{
	if (begin > end)
		return NULL;

	Node* root = new Node(preOrder[preIndex]);

	int rootIndex = begin;
	while (rootIndex <= end){
		if (inOrder[rootIndex] == preOrder[preIndex])
			break;
		++rootIndex;
	}

	assert(rootIndex <= end);

	if (begin <= rootIndex - 1)
		root->_left = __RebuildTree(preOrder, inOrder, ++preIndex, begin, rootIndex - 1);
	else
		root->_left = NULL;

	if (rootIndex + 1 <= end)
		root->_right = __RebuildTree(preOrder, inOrder, ++preIndex, rootIndex + 1, end);
	else
		root->_right = NULL;

	return root;
}

Node* RebuildTree(const vector<int>& preOrder, const vector<int>& inOrder)
{
	assert(preOrder.size() == inOrder.size());
	int preIndex = 0;
	int begin = 0;
	int end = preOrder.size()-1;

	return __RebuildTree(preOrder, inOrder, preIndex, begin, end);
}

class Stack
{
public:
	void Push(int x)
	{
		_s.push(x);

		if (_minS.empty() || x < _minS.top())
			_minS.push(x);
		else
			_minS.push(_minS.top());
	}
	void Pop()
	{
		_s.pop();
		_minS.pop();
	}

	int& Min()
	{
		return _minS.top();
	}
private:
	stack<int> _s;
	stack<int> _minS;
};

void TestStack()
{

	Stack s;
	s.Push(6);
	s.Push(2);
	s.Push(1);
	s.Push(3);
	s.Push(10);
	s.Push(0);
}


void TestRebuilTree()
{
	vector<int> preOrder;
	preOrder.push_back(1);
	preOrder.push_back(2);
	preOrder.push_back(3);
	preOrder.push_back(4);
	preOrder.push_back(5);
	preOrder.push_back(6);

	vector<int> inOrder;
	inOrder.push_back(3);
	inOrder.push_back(2);
	inOrder.push_back(4);
	inOrder.push_back(1);
	inOrder.push_back(6);
	inOrder.push_back(5);

	Node* root = RebuildTree(preOrder, inOrder);
}


void TestBinaryTree()
{
	int a1[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	//BinaryTree<int> t(a1, sizeof(a1) / sizeof(a1[0]), '#');
	//t.PreOrder();
	//t.InOrder();
	//t.PostOrder();
	//t.PreOrderNoR();
	//t.InOrderNoR();
	//t.PostOrderNoR();
	//t.LevelOrder();
	//cout << "size: " <<t.Size() << endl;
	//cout << "height: " <<t.Height() << endl;
	//cout << "leafnode: " <<t.LeafNode() << endl;
	//cout << "levelKNode: " << t.LevelKNode(3) << endl;

	int a2[] = { 1, 2, '#', 3, '#', '#', 4, 5, '#', 6, '#', 7, '#', '#', 8 };
	BinaryTree<int> t(a2, sizeof(a2) / sizeof(a2[0]), '#');

	__BinaryTreeNode<int>* x1 = t.Find(2);
	__BinaryTreeNode<int>* x2 = t.Find(8);

	__BinaryTreeNode<int>* ancestor = t.GetCommenAncestor(x1, x2);
//	t.PreOrder();
//	t.InOrder();
//	t.PostOrder();
//	t.PreOrderNoR();
//	t.InOrderNoR();
//	t.PostOrderNoR();
//	t.LevelOrder();
//	cout << "size: " << t.Size() << endl;
//	cout << "height: " << t.Height() << endl;
//	cout << "leafnode: " << t.LeafNode() << endl;
//	cout << "levelKNode: " << t.LevelKNode(13) << endl;
}