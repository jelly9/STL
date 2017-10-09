//利用霍夫曼编码实现简单的文件压缩

#define _CRT_SECURE_NO_WARNINGS

#ifndef __MY_FILE_COMPRESS_H__
#define __MY_FILE_COMPRESS_H__

#include <iostream>
#include <algorithm>
#include <string>

#include "D:\Github\STL\HuffmanTree.h"

const int CHARSIZE = 256;
const int BUFFSIZE = 1024;

//文件的智能指针,自动关闭文件指针
class FilePtr
{
public:
	FILE* _p;

	FilePtr(FILE* p)
	{
		assert(p);
		_p = p;
	}
	~FilePtr()
	{
		fclose(_p);
	}
private:
	FilePtr(const FilePtr&);
	FilePtr operator=(const FilePtr&);
};

struct CharInfor
{
	char _ch;
	int _count;
	string _huffmanCode;

	bool operator >(const CharInfor& c)const
	{
		return _count > c._count;
	}
	bool operator <(const CharInfor& c)const
	{
		return _count < c._count;
	}
	CharInfor operator+(const CharInfor& c)
	{
		CharInfor temp;
		temp._count = _count + c._count;
		return temp;
	}
	bool operator == (const CharInfor& c)const
	{
		return _count == c._count;
	}

	bool operator != (const CharInfor& c)const
	{
		return _count != c._count;
	}
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfor> Node;
public:
	FileCompress()
	{
		for (size_t i = 0; i < CHARSIZE; ++i){
			_infor[i]._ch = i;
			_infor[i]._count = 0;
		}
	}

	void Compress(const char* filePath)
	{
		string fileName(filePath);//获取文件名
		_GetFileName(filePath, fileName);

		//1. 统计字符个数
		FilePtr _fpOut(fopen(fileName.c_str(), "r"));//使用智能指针
		FILE* fpOut = _fpOut._p;
		//FILE* fpOut = fopen(fileName.c_str(), "r");


		char ch = getc(fpOut);
		while (!feof(fpOut)){//ch != EOF
			++_infor[(unsigned char)ch]._count;
			ch = getc(fpOut);
		}

		//2. 构建赫夫曼树
		CharInfor invalue;
		invalue._count = 0;//出现次数为0的字符不参与树的构建
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);

		//3. 获取霍夫曼编码
		Node *root = ht.GetRoot();
		//string code;
		_GetHuffmanCode(root);

		/////////////////////////////////////////
		//cout << "################################" << endl;
		//_PreOrder(root);


		//4. 压缩文件

		//生成压缩文件名
		string compressFileName(fileName);
		size_t idx = compressFileName.rfind('_');
		compressFileName.erase(idx, compressFileName.size() - idx);
		compressFileName += ".hzp";

		FilePtr _fpIn(fopen(compressFileName.c_str(), "w"));
		FILE* fpIn = _fpIn._p;
		//FILE* fpIn = fopen(compressFileName.c_str(), "w");

		////压缩文件的头部信息——编码格式(文件后缀+字符种类数+每个字符的个数)
		string headInforBuff;

		//（1）获取文件后缀
		string filePostfix;
		_GetFilePostfix(fileName, filePostfix);
		filePostfix += '\n';
		//写入文件后缀
		fwrite(filePostfix.c_str(), 1, filePostfix.size(), fpIn);

		//（2）获取字符出现个数
		string charInforBuff;//字符信息
		int count = 0;//字符种类数
		fseek(fpOut, 0, SEEK_SET);//设置文件指针至文件开头
		for (size_t i = 0; i < CHARSIZE; ++i){
			if (0 != _infor[i]._count){
				++count;
				charInforBuff += _infor[i]._ch;
				charInforBuff += ':';

				char countBuff[32]; //每个字符出现个数
				_itoa(_infor[i]._count, countBuff, 10);

				charInforBuff += countBuff;
				charInforBuff += '\n';
			}
		}
		char speciesBuff[32];//字符种类数
		_itoa(count, speciesBuff, 10);
		string countBuff(speciesBuff);
		countBuff += '\n';
		//写入count
		fwrite(countBuff.c_str(), 1, countBuff.size(), fpIn);

		//写入charInfor
		fwrite(charInforBuff.c_str(), 1, charInforBuff.size(), fpIn);

		//（3）压缩字符
		fseek(fpOut, 0, SEEK_SET);//设置文件指针至文件开头
		ch = getc(fpOut);
		char value = 0;
		int pos = 0;
		int c = 0;
		while (ch != EOF){
			string code = _infor[(unsigned char)ch]._huffmanCode;
			for (size_t i = 0; i < code.size(); ++i){
				if ('0' == code[i])
					value &= ~(1 << pos);//将第pos位设置为0
				else if ('1' == code[i])
					value |= 1 << pos;//将第pos位设置为1
				else//error
					assert(false);

				++pos;

				if (8 == pos){//如果够八个位就写入文件
					fputc(value, fpIn);
					value = 0;
					pos = 0;
				}
			}

			ch = fgetc(fpOut);
			cout << ch << ' ';
			++c;
		}

		if (0 != pos)
			fputc(value, fpIn);

		//fclose(fpOut);
		//fclose(fpIn);
	}
	void Decompress(const char *filePath)
	{
		//// 提取解压文件名
		string fileName;
		_GetFileName(filePath, fileName);

		FilePtr _fpOut(fopen(fileName.c_str(), "r"));
		FILE* fpOut = _fpOut._p;

		string decompressFileName(fileName);
		int idx = decompressFileName.rfind('.');
		decompressFileName.erase(idx, decompressFileName.size() - idx);
		string filePostfix;
		_GetLine(fpOut, filePostfix);
		decompressFileName += "_out";
		decompressFileName += filePostfix;

		//1. 读取字符个数信息
		string sizeBuff;
		_GetLine(fpOut, sizeBuff);
		size_t size = atoi(sizeBuff.c_str());

		string charInfor;
		string charCount;
		for (size_t i = 0; i < size; ++i)
		{
			charInfor = "";
			_GetLine(fpOut, charInfor);
			if (0 != charInfor.size()){
				_infor[(unsigned char)charInfor[0]]._ch = charInfor[0];
				charCount = charInfor.substr(2, charInfor.size() - 2);
				_infor[(unsigned char)charInfor[0]]._count = atoi(charCount.c_str());
			}
			else{
				_GetLine(fpOut, charInfor);
				_infor[(unsigned char)charInfor[0]]._ch = '\n';
				charCount = charInfor.substr(1, charInfor.size() - 1);
				_infor[(unsigned char)('\n')]._count = atoi(charCount.c_str());
			}
		}

		//2. 构建霍夫曼树
		CharInfor invalue;
		invalue._count = 0;
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);


		//3. 获取霍夫曼编码
		Node *root = ht.GetRoot();
		_GetHuffmanCode(root);
		
		//cout << "################################" << endl;
		//_PreOrder(root);

		//4. 解压缩
		FilePtr _fpIn(fopen(decompressFileName.c_str(), "w"));
		FILE* fpIn = _fpIn._p;
		//FILE *fpIn = fopen(decompressFileName.c_str(), "w");//打开解压后的文件

		Node *cur = root;
		char ch = fgetc(fpOut);
		long long count = root->_w._count;

		while (count){
			for (size_t pos = 0; pos < 8; ++pos){
				if (0 == (ch & (1 << pos)))//如果第pos位是0
					cur = cur->_left;
				else
					cur = cur->_right;

				if (NULL == cur->_left && NULL == cur->_right){
					fputc(cur->_w._ch, fpIn);
					cout << cur->_w._ch << ' ';

					--count;
					cur = root;
				}
			}
			if (0 == count)
				break;
			ch = fgetc(fpOut);
		}
		//fclose(fpOut);
		//fclose(fpIn);
	}
	~FileCompress()
	{}
protected:
	void _PreOrder(Node* root)
	{
		if (NULL == root)
			return;

		cout << root->_w._ch << ' ' << root->_w._count << ' ' << root->_w._huffmanCode << endl;

		_PreOrder(root->_left);
		_PreOrder(root->_right);
	}
	//读取一行
	void _GetLine(FILE* fp, string& buff)
	{
		assert(fp);

		char ch;
		while (!feof(fp)){
			ch = getc(fp);
			if ('\n' == ch)
				break;
			else
				buff.push_back(ch);
		}
	}
	//获取文件名后缀
	void _GetFilePostfix(const string& fileName, string& filePostfix)
	{
		size_t pos = fileName.find_last_of('.');
		filePostfix = fileName.substr(pos, fileName.size()-pos);
	}
	//获取文件名
	void _GetFileName(const string& filePath, string &fileName)
	{
		//D:\\VS2013\\STL\FileCompress\\FileCompress\\test.txt
		size_t pos = filePath.find_last_of('\\');
		if (pos == string::npos)
			fileName = filePath;
		else
			fileName = filePath.substr(pos+1, filePath.size()-pos-1);
	}

	void _GetHuffmanCode(Node *root)//获取霍夫曼编码
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
	void _GetHuffmanCode(Node *root, string code)
	{
		if (NULL == root)
			return;
		
		if (NULL == root->_left && NULL == root->_right){
			_infor[(unsigned char)root->_w._ch]._huffmanCode = code;
			return;
		}
		_GetHuffmanCode(root->_left, code+'0');
		_GetHuffmanCode(root->_right, code+'1');
	}
protected:
	CharInfor _infor[CHARSIZE];
};

#endif

void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("test_in.txt");
	cout << endl << "########################################" << endl << endl << endl;



	//char buff[32] = "我爱学习";

	//fc.Compress("timg_in.jpg");
	//fc.Decompress("timg.huffman");

	//char ch[] = "Linux（i英语发音：/?l?n?ks/ LIN-?ks）是一种自由和开放源代码的类UNIX操作系统。\
	//	该操作系统的内核由林纳斯·托瓦兹在1991年10月5日首次发布。，在加上用户空间的应 \
	//	用程序之后，成为Linux操作系统。Linux也是自由软件和开放源代码软件发展中最著名 \
	//	的例子。只要遵循GNU通用公共许可证，任何个人和机构都可以自由地使用Linux的所有 \
	//	底层源代码，也可以自由地修改和再发布。大多数Linux系统还包括像提供GUI的X Wind \
	//	ow之类的程序。除了一部分专家之外，大多数人都是直接使用Linux发行版，而不是自己 \
	//	选择每一样组件或自行设置。";

	//cout << '\\' << endl;
	//FilePtr _fpOut(fopen("test_in.txt", "r"));
	//FILE* fpOut = _fpOut.GetPtr();
} 
void TestFileDecompress()
{
	FileCompress fc;
	fc.Decompress("test.hzp");
	getchar();
}