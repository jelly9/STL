/*
*
* 版权 : 		(c) 2017
* 模块名 :
* 文件名 :		BitMap.h
* 作者 :		肖国栋
*				1799553128@qq.com
* 创建日期 :	2017-11-19
* 版本 :		XXXX.XX.XX
* 相关文件 :
* 文件实现功能 :位图处理大数问题
* 备注 :
* 修改记录 :
*
*/

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <iostream>
using namespace std;
#include <vector>

class BitMap
{
public:
	BitMap(const size_t n)
	{
		_bits.resize(n/8+1, 0);
	}

	//将x位置置1
	void Set(const size_t x)
	{
		size_t index = x / 8;
		size_t pos = x % 8;
		_bits[index] |= (1 << pos);
	}

	//将x位置置0
	void ReSet(const size_t x)
	{
		size_t index = x / 8;
		size_t pos = x % 8;
		_bits[index] &= ~(1 << pos);
	}

	//检查x位置是否为1
	bool Test(const size_t x)
	{
		size_t index = x / 8;
		size_t pos = x % 8;
		if (_bits[index] & (1 << pos))
			return true;
		else
			return false;
	}
private:
	//防拷贝，没必要
	//BitMap(const BitMap& b);
	//BitMap& operator=(const BitMap& b);
private:
	vector<char> _bits;
};

class TwoBitMap
{
public:
	TwoBitMap(const size_t n)
	{
		_twoBits.resize(n/4+1, 0);
	}

	void Add(const size_t x)
	{
		size_t index = x / 4;//计算在数组中的位置
		size_t pos = (x % 4)*2;//计算在第几个位上

		if (_twoBits[index] & (1<< (pos+1)))// 10/11 -> 11
			_twoBits[index] |= (1 << pos);
		else if (_twoBits[index] & (1<< pos)){//	01 -> 10
			_twoBits[index] |= (1 << (pos+1));
			_twoBits[index] &= ~(1 << pos);
		}
		else// 00 -> 01
			_twoBits[index] |= (1 << pos);
	}

	int Get(const size_t x)
	{
		size_t index = x / 4;
		size_t pos = (x % 4)*2;

		if (_twoBits[index] & (1 << (pos + 1))){
			if (_twoBits[index] & (1 << pos))
				return INT_MAX;
			else
				return 2;
		}
		else{
			if (_twoBits[index] & (1 << pos))
				return 1;
			else
				return 0;
		}
	}

protected:
	vector<char> _twoBits;
};

#endif

#if 1
//测试
void TestTwoBitMap()
{
	TwoBitMap tm(1000);

	tm.Add(18);
	tm.Add(19);
	tm.Add(19);
	tm.Add(31);
	tm.Add(31);
	tm.Add(31);
	tm.Add(4);
	tm.Add(4);
	tm.Add(4);
	tm.Add(4);

	int i = 0;
	i = tm.Get(10);
	i = tm.Get(18);
	i = tm.Get(19);
	i = tm.Get(31);
	i = tm.Get(4);
}

void TestBitMap()
{
	BitMap bm(10000);
	bm.Set(0);
	bm.Set(7);
	bm.Set(8);
	bm.Set(15);
	bm.Set(16);

	cout << bm.Test(0) << endl;
	cout << bm.Test(1) << endl;
	cout << bm.Test(7) << endl;
	cout << bm.Test(9) << endl;
	cout << bm.Test(8) << endl;
	cout << bm.Test(10) << endl;
	cout << bm.Test(15) << endl;
	cout << bm.Test(17) << endl;
	cout << bm.Test(16) << endl;
	cout << bm.Test(18) << endl;


	bm.ReSet(0);
	bm.ReSet(7);
	bm.ReSet(8);
	bm.ReSet(15);
	bm.ReSet(16);
}

#endif