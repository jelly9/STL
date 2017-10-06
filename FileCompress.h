#define _CRT_SECURE_NO_WARNINGS

#ifndef __MY_FILE_COMPRESS_H__
#define __MY_FILE_COMPRESS_H__


#include <iostream>
#include "D:\Github\STL\HuffmanTree.h"

const int CHARSIZE = 256;

struct charInfor
{
	char _c;
	size_t _count;

	bool operator >(const charInfor& c)const
	{
		return _count > c._count;
	}
	bool operator <(const charInfor& c)const
	{
		return _count < c._count;
	}
	charInfor operator+(const charInfor& c)
	{
		charInfor temp;
		temp._count = _count + c._count;
		return temp;
	}
	bool operator == (const charInfor& c)const
	{
		return _count == c._count;
	}

	bool operator != (const charInfor& c)const
	{
		return _count != c._count;
	}
};

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i = 0; i < CHARSIZE; ++i){
			_infor[i]._c = i;
			_infor[i]._count = 0;
		}
	}

	void Compress(char *fileName)
	{
		//1. 统计字符个数
		FILE* in = fopen(fileName, "r");
		char c;
		while (!feof(in)){//
			c = getc(in);
			++_infor[c]._count;
		}

		//2. 构建赫夫曼树
		charInfor invalue;
		invalue._count = 0;
		HuffmanTree<charInfor> ht(_infor, CHARSIZE, invalue);

		//3. 获取霍夫曼编码


		//4. 压缩文件


		fclose(in);
	}
	void Decompress()
	{
		;
	}
	~FileCompress()
	{}
protected:
	charInfor _infor[CHARSIZE];
};



#endif

void TestFileCompress()
{
	FileCompress fc;

	fc.Compress("test.txt");
}