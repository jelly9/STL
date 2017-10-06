#ifndef __MY_HUFFMANTREE_H__
#define __MY_HUFFMANTREE_H__

#pragma once

#include "D:\Github\STL\Heap.h"

//ºÕ·òÂüÊ÷½Úµã
template<class W>
struct HuffmanTreeNode
{
	typedef HuffmanTreeNode Self;

	HuffmanTreeNode(const W& w)
		:_w(w)
		, _left(NULL)
		, _right(NULL)
	{}

	Self operator + (Self& h)const
	{
		Self temp;
		temp._w = _w + h._w;
		return temp;
	}

	bool operator !=(const Self& h)const
	{
		return _w != h._w;
	}
	bool operator ==(const Self& h)const
	{
		return _w == h._w;
	}
	W _w;
	HuffmanTreeNode* _left;
	HuffmanTreeNode* _right;
};
template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree(const W *arr, size_t size, const W& invalue)
	{
		struct NodePtrCom{
			bool operator()(const Node* l, const Node* r){
				return l->_w < r->_w;
			}
		};

		Heap<Node*, NodePtrCom> h;
		for (size_t i = 0; i < size; ++i){
			if (arr[i] != invalue)
				h.Push(new Node(arr[i]));
		}

		while (h.Size() > 1){
			Node *left = h.Top();
			h.Pop();
			Node *right = h.Top();
			h.Pop();

			Node *parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;

			h.Push(parent);
		}
		_root = h.Top();
	}

protected:
	Node *_root;
};


#endif

void TestHuffmanTree()
{
	int a[] = {1, 2, 3, 4};

	HuffmanTree<int> h(a, sizeof(a)/sizeof(a[0]), -1);
}
