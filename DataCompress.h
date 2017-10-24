//霍夫曼数据压缩算法
#define _CRT_SECURE_NO_WARNINGS

#ifndef __MY_DATA_COMPRESS_H__
#define __MY_DATA_COMPRESS_H__

#include <algorithm>
#include <string>

#include "D:\Github\STL\HuffmanTree.h"

#define CHARSIZE 256
#define BUFFSIZE 1024

struct CharInfor
{
	char _ch;
	long long _count;
	string _huffmanCode;

	bool operator <(const CharInfor& c)const
	{
		return _count < c._count;
	}
	CharInfor operator+(const CharInfor& c)const
	{
		CharInfor temp;
		temp._count = _count + c._count;
		return temp;
	}

	bool operator != (const CharInfor& c)const
	{
		return _count != c._count;
	}
};

class DataCompress
{
	typedef HuffmanTreeNode<CharInfor> Node;
	//智能文件指针，自动关闭文件
	class FilePtr{
		FilePtr(const FilePtr&);
		FilePtr operator=(const FilePtr&)const;
	public:
		FilePtr(FILE* p){
			assert(p);
			_p = p;
		}
		~FilePtr(){
			fclose(_p);
		}
		FILE* _p;
	};
	//配置信息的结构体
	struct ConfigInfor{
		char _ch;
		long long _count;
	};
public:
	DataCompress()
	{
		for (int i = 0; i < CHARSIZE; ++i){
			_infor[i]._ch = i;
			_infor[i]._count = 0;
		}
	}
	//压缩
	void Compress(const char* fileName)
	{
		FilePtr _fpOut(fopen(fileName, "rb"));
		FILE* fpOut = _fpOut._p;
		//1. 统计字符个数
		char ch = fgetc(fpOut);
		while (!feof(fpOut)){
			++_infor[(unsigned char)ch]._count;
			ch = getc(fpOut);
		}

		//2. 构建赫夫曼树
		CharInfor invalue;
		invalue._count = 0;//出现次数为0的字符不参与树的构建
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);

		//3. 获取霍夫曼编码
		Node *root = ht.GetRoot();
		_GetHuffmanCode(root);

		//4.生成压缩文件名
		string compressFileName(fileName);
		compressFileName += ".hcp";

		FilePtr _fpIn(fopen(compressFileName.c_str(), "wb"));
		FILE* fpIn = _fpIn._p;
		fseek(fpOut, 0, SEEK_SET);

		//5.压缩配置信息
		string postfix(fileName);//原文件后缀
		char postfixBuff[16] = {0};
		size_t pos = postfix.rfind('.');
		if (pos != string::npos){
			postfix = postfix.substr(pos, postfix.size() - pos);
			strcpy(postfixBuff, postfix.c_str());
		}
		fwrite(postfixBuff, 1, 16, fpIn);

		ConfigInfor infor;//字符个数信息
		for (size_t i = 0; i < CHARSIZE; ++i){
			if (_infor[i]._count != 0){
				infor._ch = _infor[i]._ch;
				infor._count = _infor[i]._count;
				fwrite(&infor, sizeof(ConfigInfor), 1, fpIn);
			}
		}
		infor._count = 0;
		fwrite(&infor, sizeof(ConfigInfor), 1, fpIn);

		//6. 压缩编码信息
		char readBuff[BUFFSIZE];
		char writeBuff[BUFFSIZE];
		size_t wIdx = 0;
		pos = 0;
		char value;

		while (1){
			size_t size = fread(readBuff, 1, BUFFSIZE, fpOut);
			if (0 == size)
				break;

			for (size_t rIdx = 0; rIdx < size; ++rIdx){
				string& code = _infor[(unsigned char)readBuff[rIdx]]._huffmanCode;
				for (size_t i = 0; i < code.size(); ++i){
					if ('0' == code[i])
						value &= ~(1 << pos);	//将第pos位设置为0
					else if ('1' == code[i])
						value |= 1 << pos;		//将第pos位设置为1
					else						//编码出错
						assert(false);

					++pos;

					if (8 == pos){//如果够八个位就写入文件
						writeBuff[wIdx++] = value;
						value = 0;
						pos = 0;

						if (wIdx == BUFFSIZE){
							fwrite(writeBuff, 1, BUFFSIZE, fpIn);
							memset(writeBuff, '\0', BUFFSIZE);
							wIdx = 0;
						}
					}
				}
			}
		}

		if (0 != pos)
			writeBuff[wIdx++] = value;
		fwrite(writeBuff, 1, wIdx, fpIn);
	}
	//解压
	void Decompress(const char* fileName)
	{
		FilePtr _fpOut(fopen(fileName, "rb"));
		FILE* fpOut = _fpOut._p;

		// 1.生成解压后文件名
		char postfixBuff[16];
		//memset(postfixBuff, '\0', 16);
		fread(postfixBuff, 1, 16, fpOut);
		string decompressFileName(fileName);
		decompressFileName += postfixBuff;

		//2.读取字符个数信息
		ConfigInfor infor;
		while (1){
			fread(&infor, sizeof(ConfigInfor), 1, fpOut);
			if (0 == infor._count)
				break;
			_infor[(unsigned char)infor._ch]._ch = infor._ch;
			_infor[(unsigned char)infor._ch]._count = infor._count;
		}

		//2. 构建霍夫曼树
		CharInfor invalue;
		invalue._count = 0;
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);

		//3. 获取霍夫曼编码
		Node *root = ht.GetRoot();
		_GetHuffmanCode(root);

		//4. 解压
		FilePtr _fpIn(fopen(decompressFileName.c_str(), "wb"));
		FILE* fpIn = _fpIn._p;

		Node *cur = root;
		long long count = root->_w._count;
		char readBuff[BUFFSIZE];
		char writeBuff[BUFFSIZE];
		size_t wIdx = 0;
		while (1){
			size_t size = fread(readBuff, 1, BUFFSIZE, fpOut);
			if (size == 0)
				break;

			for (size_t rIdx = 0; rIdx < size; ++rIdx){
				char ch = readBuff[rIdx];
	
				for (size_t pos = 0; pos < 8; ++pos){
					if (0 == (ch & (1 << pos)))
						cur = cur->_left;
					else
						cur = cur->_right;

					if (NULL == cur->_left && NULL == cur->_right){
						writeBuff[wIdx++] = cur->_w._ch;
						cur = root;
						--count;
						
						if (0 == count)
							break;

						if (wIdx == BUFFSIZE){
							fwrite(writeBuff, 1, BUFFSIZE, fpIn);
							wIdx = 0;
							memset(writeBuff, '\0', BUFFSIZE);
						}
					}
				}
			}
		}

		fwrite(writeBuff, 1, wIdx, fpIn);
	}
protected:
	//获取霍夫曼编码
	void _GetHuffmanCode(Node *root)
	{
		if (NULL == root)
			return;

		_GetHuffmanCode(root->_left);
		_GetHuffmanCode(root->_right);

		if (NULL == root->_left && NULL == root->_right){
			string& code = _infor[(unsigned char)root->_w._ch]._huffmanCode;
			Node *cur = root;
			Node *parent = cur->_parent;
			while (parent){
				if (cur == parent->_left)
					code += '0';
				else
					code += '1';

				cur = parent;
				parent = cur->_parent;
			}

			reverse(code.begin(), code.end());
		}
	}
protected:
	CharInfor _infor[CHARSIZE];
};

#endif

void TestCompress(char* fileName)
{
	DataCompress fc;				// test.txt
	fc.Compress(fileName);		// --> test.txt.hcp
}
void TestDeCompress(char* fileName)
{
	DataCompress fc;							// test.txt.hcp
	fc.Decompress(fileName);			// --> test.txt.hcp.txt
}

