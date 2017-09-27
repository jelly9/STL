#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <iostream>
using namespace std;

#include <stack>
#include <queue>
#include <assert.h>

//二叉树节点
template<class T>
struct BinaryTreeNode
{
	BinaryTreeNode(const T& value)
		:_value(value)
		,_left(NULL)
		,_right(NULL)
	{}

	T _value;
	BinaryTreeNode<T> *_left;
	BinaryTreeNode<T> *_right;
};

//二叉树
template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
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
			if(cur->_left)
				q.push(cur->_left);
			if(cur->_right)
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
	

	//析构函数
	~BinaryTree()
	{
		_DestroyTree(_root);
	}


protected:
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
	t.PreOrder();
	t.InOrder();
	t.PostOrder();
	t.PreOrderNoR();
	t.InOrderNoR();
	t.PostOrderNoR();
	t.LevelOrder();
	cout << "size: " << t.Size() << endl;
	cout << "height: " << t.Height() << endl;
	cout << "leafnode: " << t.LeafNode() << endl;
	cout << "levelKNode: " << t.LevelKNode(13) << endl;
}