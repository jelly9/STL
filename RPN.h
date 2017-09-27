#ifndef _RPN_H_
#define _RPN_H_

#include <iostream>
using namespace std;

#include <vector>
#include <stack>
#include <assert.h>

enum Cal_Type
{
	OP_NUM,
	OP_SYMBOL,

	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
};


struct Cell
{
	int _value;
	Cal_Type _type;
};


class Calculator
{
public:
	//构造函数
	//参数一为逆波兰表达式
	Calculator(const Cell *exp, size_t n)
	{
		for (size_t i = 0; i < n; i++){
			_exp.push_back(exp[i]);
		}
	}
	//参数一为算术表达式（是一个字符串）
	Calculator(string exp)
	{
		
	}

	//计算
	int Count()
	{
		stack<int> count;
		for (size_t i = 0; i < _exp.size(); i++){
			if (OP_NUM == _exp[i]._type)
				count.push(_exp[i]._value);
			else{
				int right = count.top();
				count.pop();
				int left = count.top();
				count.pop();

				if (OP_ADD == _exp[i]._value)
					count.push(left+right);
				else if (OP_SUB == _exp[i]._value)
					count.push(left - right);
				else if (OP_MUL == _exp[i]._value)
					count.push(left * right);
				else if (OP_DIV == _exp[i]._value){
					assert(0 != right);
					count.push(left / right);
				}
				else{
					cout << "error" << endl;
					exit(0);
				}
			}
		}

		return count.top();
	}

private:
	vector<Cell> _exp;
};

#endif


void TestRPN()
{
	Cell exp[] =
	{
		{2, OP_NUM},
		{3, OP_NUM},
		{4, OP_NUM},
		{OP_MUL, OP_SYMBOL},
		{OP_SUB, OP_SYMBOL},
		{5, OP_NUM},
		{OP_ADD, OP_SYMBOL},
	};

	//2-3*4+5
	cout << Calculator(exp, sizeof(exp) / sizeof(Cell)).Count() << endl;

}