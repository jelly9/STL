/*
 *
 * 版权 : 		(c) 2017
 * 模块名 :		STL
 * 文件名 :		Hash.h
 * 作者 :		肖国栋
 *				1799553128@qq.com
 * 创建日期 :	2017-11-11
 * 版本 :		XXXX.XX.XX
 * 相关文件 :
 * 文件实现功能 :哈希表实现，处理冲突：1.开放定址法（闭散列法） 2.开链法
 * 备注 :
 * 修改记录 :
 *
 */

#ifndef __HASH_H__
#define __HASH_H__

//#include "D:\Github\STL\Vector.h"
//#include <string>
#include <iostream>
using namespace std;

#include <vector>
#include "D:\Github\STL\HashFunc.h"


//Hash表
////////////////////////////////////
enum TAG{ EMPTY, EXIST, DELETE };

template<class Value>
struct __HashTableNode
{
	Value _valueField;
	TAG _state;
	__HashTableNode()
		:_valueField(Value())
		,_state(EMPTY)
	{}
	__HashTableNode(const Value& valueField)
		:_valueField(valueField)
		, _state(EMPTY)
	{}
};

//素数表
static const unsigned long _primeList[28] = 
{
	53,			97,			193,			389,			769,
	1543,		3079,		6151,		12289,		24593,
	49157,		98317,		196613,		393241,		786433,
	1572869,		3145739,		6291469,		12582917,	25165843,
	50331653,	100663319,	201326611,	402653189,	805306457,
	1610612741,	3221225473,	4294967291
};

inline unsigned long __NextPrime(unsigned long n)
{

	size_t i = 0;
	for (; i < 28; ++i){
		if (_primeList[i] > n)
			break;
	}
	return i < 28 ? _primeList[i] : _primeList[27];
}

template<class Key, class Value, class _KeyOfValue, class _HashFunc >
class HashTable
{
	typedef __HashTableNode<Value> Node;
public:
	HashTable()
		:_size(0)
	{
}
	bool Insert(const Value& value)
	{
		_CheckCapacity();
		size_t hash = HashFunc(_KeyOfValue()(value));
		int idx = hash;
		while (EXIST == _table[idx]._state){
			if (value == _table[idx]._valueField)
				return false;
		
			++idx;
			idx %= _table.size();
		}

		_table[idx] = value;
		_table[idx]._state = EXIST;
		++_size;
		return true;
	}

	size_t Find(const Key& key)
	{
		size_t hash = HashFunc(key);
		size_t idx = hash;
		size_t size = _table.size();
		while (size--){
			if (_KeyOfValue()(_table[idx]._valueFeild) == key)
				return idx;
			++idx;
			idx %= _table.size();
		}
		return -1;
	}

	bool Earse(const Key& key)
	{
		size_t hash = HashFunc(key);
		size_t idx = hash;
		size_t size = _table.size();
		while (size--){
			if (_KeyOfValue()(_table[idx]._valueFeild) == key){
				_table[idx]._state = DELETE;
				return true;
			}

			++idx;
			idx %= _table.size();
		}
		return false;
	}
protected:
	size_t HashFunc(const Key& key)
	{
		//_HashFunc hf;
		//return hf(key) % _table.size();
		return _HashFunc()(key) % _table.size();
	}
	void _CheckCapacity()
	{
		if (0 == _table.size()){
			_table.resize(__NextPrime(1));
			return;
		}
		//负载因子超过0.7则扩容
		else if ((_size * 10) / _table.size() > 7){
			HashTable<Key, Value, _KeyOfValue, _HashFunc> newTable;
			newTable._table.resize(__NextPrime(_table.size()));
			for (size_t idx = 0; idx < _table.size(); ++idx){
				if (EXIST == _table[idx]._state)
					newTable.Insert(_table[idx]._valueField);
			}
			_table.swap(newTable._table);
		}
	}
protected:
	vector<Node> _table;
	size_t _size;
};

//Hash桶
/////////////////////////////////////
template<class Value>
struct __HashBucketNode
{
	Value _valueField;
	__HashBucketNode<Value> *_next;

	__HashBucketNode(const Value& valueField)
		:_valueField(valueField)
		, _next(NULL)
	{}
};

template<class Key, class Value, class _KeyOfValue, class _Hash>
class HashBucket;

template<class Key, class Value, class _KeyOfValue, class _Hash>
struct __HashBucketIterater
{
	typedef __HashBucketNode<Value> Node;
	typedef __HashBucketIterater<Key, Value, _KeyOfValue, _Hash> Iterator;
	typedef HashBucket<Key, Value, _KeyOfValue, _Hash>  HB;

	typedef Value& Ref;
	typedef Value* Ptr;

	Node *_node;
	HB *_hb;

	__HashBucketIterater()
		:_node(NULL)
		,_hb(NULL)
	{}

	__HashBucketIterater(const Iterator& self)
		:_node(self._node)
		, _hb(self._hb)
	{}

	__HashBucketIterater(Node *node, HB *hb)
		:_node(node)
		, _hb(hb)
	{}

	Ref operator*()
	{
		return _node->_valueField;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	bool operator==(const Iterator& it)
	{
		return _node == it._node;
	}

	bool operator!=(const Iterator& it)
	{
		return _node != it._node;
	}

	Iterator& operator++()//前置
	{
		_Increment();
		return *this;
	}

	Iterator operator++(int)//后置
	{
		Iterator tmp(_node, _hb);
		_Increment();
		return tmp;
	}

protected:
	void _Increment()
	{
		if(_node->_next)//不是最后一个节点
			_node = _node->_next;
		else{//是最后一个节点，则跳到下一个不为空的桶里
			size_t hash = _Hash()(_KeyOfValue()(_node->_valueField)) % _hb->_bucket.size();
			size_t i = hash + 1;
			for (; i < _hb->_bucket.size(); ++i){
				if (_hb->_bucket[i]){
					_node = _hb->_bucket[i];
					return;
				}
			}

			_node = NULL;
			_hb = NULL;
		}
	}
};

template<class Key, class Value, class _KeyOfValue, class _Hash>
class HashBucket
{
	typedef __HashBucketNode<Value> Node;
	friend struct __HashBucketIterater<Key, Value, _KeyOfValue, _Hash>;
public:
	typedef __HashBucketIterater<Key, Value, _KeyOfValue, _Hash> Iterator;
public:
	HashBucket()
		:_size(0)
	{}

	//iterator
	Iterator Begin()
	{
		Node *cur = NULL;
		for (size_t i = 0; i < _bucket.size(); ++i){
			if (_bucket[i]){
				cur = _bucket[i];
				break;
			}
		}
		return Iterator(cur, this);
	}

	Iterator End()
	{
		return Iterator();
	}

	//modify
	Iterator Find(const Key& key)
	{
		if (0 == _size)
			return Iterator();
		size_t hash = _Hash()(key) % _bucket.size();
		Node *cur = _bucket[hash];
		while (cur){
			if (key == _KeyOfValue()(cur->_valueField))
				return Iterator(cur, this);
			cur = cur->_next;
		}
		return Iterator();
	}

	pair<Iterator, bool> InsertUnique(const Value& value)
	{
		//如果找到相同的就不插了
		_CheckCapacity();
		size_t hash = _Hash()(_KeyOfValue()(value)) % _bucket.size();
		Node *cur = _bucket[hash];
		while (cur){
			if (_KeyOfValue()(cur->_valueField) == _KeyOfValue()(value))
				return make_pair(Iterator(), false);
			cur = cur->_next;
		}

		Node *head = _bucket[hash];
		_bucket[hash] = new Node(value);
		_bucket[hash]->_next = head;
		++_size;
	
		return make_pair(Iterator(_bucket[hash], this), false);
	}

	pair<Iterator, bool> InsertEqual(const Value& value)
	{
		//如果找到相同的就不插了
		_CheckCapacity();
		size_t hash = _Hash()(_KeyOfValue()(value)) % _bucket.size();
		Node *head = _bucket[hash];
		_bucket[hash] = new Node(value);
		_bucket[hash]->_next = head;
		++_size;
		return make_pair(Iterator(_bucket[hash], this), true);

	}

	bool Erase(const Key& key)
	{
		if (0 == _size)
			return false;

		Iterator it = Find(key);

		//没找到
		if (Iterator() == it)
			return false;

		Node *cur = it._node;

		if (cur->_next){
			cur->_valueField = cur->_next->_valueField;
			Node *del = cur->_next;
			cur->_next = del->_next;

			delete del;
			del = NULL;
			--_size;
			return true;
		}
		else{
			size_t hash = _Hash()(key) % _bucket.size();
			Node *head = _bucket[hash];
			cur->_valueField = head->_valueField;
			_bucket[hash] = head->_next;

			delete head;
			head = NULL;
			--_size;
			return true;
		}

		return false;
	}
	//capacity
	size_t Size()
	{
		return _size;
	}
	size_t Capacity()
	{
		return _bucket.size();
	}

	size_t Empty()
	{
		return 0 == _size;
	}
protected:
	void _CheckCapacity()
	{
		if (0 == _bucket.size()){
			_bucket.resize(__NextPrime(1));
			return;
		}
		//负载因子超过0.7则扩容
		else if (_size == _bucket.size()){
			vector<Node*> newBucket;
			size_t newSize = __NextPrime(_bucket.size());
			newBucket.resize(newSize);
			for (size_t idx = 0; idx < _bucket.size(); ++idx){
				while (_bucket[idx]){
					Node *cur = _bucket[idx];
					_bucket[idx] = cur->_next;
					size_t hash = _Hash()(_KeyOfValue()(cur->_valueField)) % newSize;
					cur->_next = newBucket[hash];
					newBucket[hash] = cur;
				}
			}
			_bucket.swap(newBucket);
		}
	}
protected:
	size_t _size;
	vector<Node*> _bucket;
};

#endif

#if 0
//测试
#include "D:\Github\STL\Function.h"
#include <vector>

struct KeyOfValue
{
	int operator()(const int k)
	{
		return k;
	}
};
struct KeyOfPair
{
	string operator()(const pair<string, string> kv)
	{
		return kv.first;
	}
};
void TestHashBucket()
{
	//HashBucket<int, int> hb;
	//hb.InsertEqual(1);
	//hb.InsertEqual(2);
	//hb.InsertEqual(3);
	//hb.InsertEqual(3);
	//hb.InsertEqual(3);
	//hb.InsertEqual(3);
	//hb.InsertEqual(54);

	HashBucket<string, pair<string, string>, KeyOfPair, __HashFunc<string>> hb;
	hb.InsertUnique(make_pair("INSERT", "insert"));
	hb.InsertUnique(make_pair("DELETE", "delete"));
	hb.InsertUnique(make_pair("MODIFY", "modify"));
	hb.InsertUnique(make_pair("SERACH", "search"));
	hb.InsertUnique(make_pair("LEFT", "left"));
	hb.InsertUnique(make_pair("RIGHT", "right"));
	hb.InsertUnique(make_pair("PARENT", "parent"));

	HashBucket<string, pair<string, string>, KeyOfPair, __HashFunc<string>>::Iterator it = hb.Begin();
	while (it != hb.End()){
		cout << it->first << ' ';
		++it;
	}

	//const int N = 1000;
	//int a[N];
	//
	//RandomArrayUnique(a, N);

	//HashBucket<int, int, KeyOfValue, __HashFunc<int>> hb;
	//for (size_t i = 0; i < N; ++i){
	//	hb.InsertUnique(a[i]);
	//	hb.Find(a[i]);
	//}


	//HashBucket<int, int, KeyOfValue, __HashFunc<int>>::Iterator it = hb.Begin();
	//while (it != hb.End()){
	//	cout << *it << ' ';
	//	++it;
	//}

	//for (size_t i = 0; i < N; ++i){
	//	hb.Erase(a[i]);
	//	hb.Find(a[i]);
	//}

	//it = hb.Begin();
	//while (it != hb.End()){
	//	cout << *it << ' ';
	//	++it;
	//}

	cout << endl;
}
void TestHashTable()
{
	//HashTable<int, int> ht;
	//ht.Insert(1);
	//ht.Insert(16);
	//ht.Insert(25);
	//ht.Insert(3);
	//ht.Insert(9);

	//HashTable<string, pair<string, string>, KeyOfValue> ht;
	//ht.Insert(make_pair("in", "insert"));
	//ht.Insert(make_pair("de", "delete"));
	//ht.Insert(make_pair("mo", "modify"));
	//ht.Insert(make_pair("fi", "find"));

	//HashTable<int, pair<int, string>, KeyOfValue> ht;
	//ht.Insert(make_pair(1, "insert"));
	//ht.Insert(make_pair(2, "delete"));
	//ht.Insert(make_pair(3, "modify"));
	//ht.Insert(make_pair(4, "find"));

	int a[100];
	RandomArrayUnique(a, sizeof(a)/sizeof(int));

	HashTable<int, int, KeyOfValue, __HashFunc<int>> ht;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
		ht.Insert(a[i]);


}

#endif
