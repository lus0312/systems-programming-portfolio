#include "RPN.hpp"
#include <sstream>
#include <iostream>
#include <climits>

RPN::RPN(){}
RPN::RPN(const RPN& other): _stack(other._stack)
{
}
RPN::~RPN(){}

RPN& RPN::operator=(const RPN& other){
	if (this != &other)
		_stack = other._stack;
	return *this;
}

static bool isNumber(const std::string& token){
	if (token.size() != 1)
		return false;
	return (token[0] >= '0' && token[0] <= '9');		
}

static bool isOperator(const std::string& token){
	if (token.size() != 1)
		return false;
	return (token[0] == '+' || token[0] == '-'
		||token[0] == '*' || token[0] == '/');
}

bool RPN::operate(int left, int right, char token){
	long result;

	if (token == '+')
		result = static_cast<long>(left) + static_cast<long>(right);
	else if (token == '-')
		result = static_cast<long>(left) - static_cast<long>(right);
	else if (token == '*')
		result = static_cast<long>(left) * static_cast<long>(right);
	else if (token == '/'){
		if (right == 0)
			return false;
		result = static_cast<long>(left) / static_cast<long>(right);
	}
	else
		return false;
	if (result > INT_MAX || result < INT_MIN)
		return false;
	_stack.push(static_cast<int>(result));
	return true;
}

bool RPN::process(const std::string& input){
	std::stringstream ss(input);
	std::string token;

	while (ss >> token){
		if (isNumber(token))
			_stack.push(token[0] - '0');
		else if (isOperator(token)){
			if (_stack.size() < 2)
				return false;
			int right = _stack.top();
			_stack.pop();
			int left = _stack.top();
			_stack.pop();
			if (!operate(left, right, token[0]))
				return false;
		}
		else
			return false;
	}
	if (_stack.size() != 1)
		return false;
	std::cout<<_stack.top()<<std::endl;
	return true;
}