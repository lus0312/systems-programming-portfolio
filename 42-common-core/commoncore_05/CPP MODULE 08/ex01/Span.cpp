#include "Span.hpp"
#include <iostream>
#include <algorithm>

Span::Span() : _numbers(), _maxSize(0){}

Span::Span(unsigned int size) : _numbers(), _maxSize(size){}

Span::Span(const Span& other) : _numbers(other._numbers), _maxSize(other._maxSize) {}

Span& Span::operator=(const Span& other) {

	if (this != &other) {
		_numbers = other._numbers;
		_maxSize = other._maxSize;
	}
	return *this;
}

Span::~Span() {}

const char* Span::spanFullException::what() const throw() {
	return "Error: Span is full";
}

const char* Span::notEnoughNumbersException::what() const throw(){
	return "Error: Not enough numbers to find span";
}

void Span::addNumber(int num){
	if (_numbers.size() >= _maxSize)
		throw spanFullException();
	_numbers.push_back(num);
}

unsigned int Span::shortestSpan() const{
	std::vector<int> copy;
	long shortest;
	long diff;
	if (_numbers.size() < 2)
		throw notEnoughNumbersException();
	copy = _numbers;
	std::sort(copy.begin(), copy.end());
	shortest = static_cast<long>(copy[1]) - static_cast<long>(copy[0]);
	for (unsigned int i = 2; i < copy.size(); i++){
		diff = static_cast<long>(copy[i]) - static_cast<long>(copy[i-1]);
		if (shortest > diff)
			shortest = diff;
	}
	return static_cast<unsigned int>(shortest);
}

unsigned int Span::longestSpan() const{
	if (_numbers.size() < 2)
		throw notEnoughNumbersException();
	std::vector<int>::const_iterator minIt;
	std::vector<int>::const_iterator maxIt;
	long minValue;
	long maxValue;
	unsigned int result;
	minIt = std::min_element(_numbers.begin(),_numbers.end());
	maxIt = std::max_element(_numbers.begin(), _numbers.end());
	minValue = static_cast<long>(*minIt);
	maxValue = static_cast<long>(*maxIt);	
	result = static_cast<unsigned int>(maxValue - minValue);
	return result;
}

