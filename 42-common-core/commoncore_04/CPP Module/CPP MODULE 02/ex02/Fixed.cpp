#include "Fixed.hpp"
#include <cmath>

Fixed::Fixed(){
	_value = 0;
	std::cout <<"Default constructor called"<<std::endl;
}

Fixed::Fixed(const Fixed& other){
	_value = other.getRawBits();
	std::cout <<"Copy constructor called"<<std::endl;
}

Fixed::Fixed(const int n){
	_value = n << _fractionalBits;
	std::cout <<"Int constructor called"<<std::endl;
}

Fixed::Fixed(const float f){
	_value = roundf(f * (1 << _fractionalBits));
	std::cout <<"Float constructor called"<<std::endl;
}

Fixed::~Fixed(){
	std::cout <<"Destructor called"<<std::endl;
}

Fixed& Fixed::operator=(const Fixed& other){
	if (this != &other)
		_value = other.getRawBits();
	std::cout <<"Copy assignment operator called"<<std::endl;
	return (*this);
}

int Fixed::toInt(void) const{
	return (_value >> _fractionalBits);
}

float Fixed::toFloat(void) const{
	return ((float)_value) /(1 << _fractionalBits);
}

int Fixed::getRawBits(void) const{
	return (_value);
}

void Fixed::setRawBits(int const raw){
	_value = raw;
}

bool Fixed::operator>(const Fixed& other) const{
	return (this->_value > other._value);
}
bool Fixed::operator<(const Fixed& other) const{
	return (this->_value < other._value);
}
bool Fixed::operator>=(const Fixed& other) const{
	return (this->_value >= other._value);
}
bool Fixed::operator<=(const Fixed& other) const{
	return (this->_value <= other._value);
}
bool Fixed::operator==(const Fixed& other) const{
	return (this->_value == other._value);
}
bool Fixed::operator!=(const Fixed& other) const{
	return (this->_value != other._value);
}
Fixed Fixed::operator+(const Fixed& other) const{
	return (Fixed(this->toFloat() + other.toFloat()));
}
Fixed Fixed::operator-(const Fixed& other) const{
	return (Fixed(this->toFloat() - other.toFloat()));
}
Fixed Fixed::operator*(const Fixed& other) const{
	return (Fixed(this->toFloat() * other.toFloat()));
}
Fixed Fixed::operator/(const Fixed& other) const{
	return (Fixed(this->toFloat() / other.toFloat()));
}
Fixed& Fixed::operator++(){
	_value += 1;
	return (*this);
}
Fixed Fixed::operator++(int){
	Fixed temp;
	temp.setRawBits(_value);
	_value += 1;
	return temp;
}
Fixed& Fixed::operator--(){
	this->_value -= 1;
	return (*this);
}
Fixed Fixed::operator--(int){
	Fixed temp;
	temp.setRawBits(_value);
	_value -= 1;
	return temp;
}
Fixed& Fixed::min(Fixed& a, Fixed& b){
	if (a < b)
		return a;
	return b;
}
const Fixed& Fixed::min(const Fixed& a, const Fixed& b){
	if (a < b)
		return a;
	return b;
}
Fixed& Fixed::max(Fixed& a, Fixed& b){
	if (a > b)
		return a;
	return b;
}
const Fixed& Fixed::max(const Fixed& a, const Fixed& b){
	if (a > b)
		return a;
	return b;
}

std::ostream& operator<<(std::ostream& out, const Fixed& fixed){
	out << fixed.toFloat();
	return out;
}