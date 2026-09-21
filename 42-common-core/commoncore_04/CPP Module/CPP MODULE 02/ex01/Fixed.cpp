#include "Fixed.hpp"
#include <iostream>
#include <cmath>

Fixed::Fixed(){
	_value = 0;
	std::cout << "Default constructor called"<<std::endl;
}

Fixed::Fixed(const int n){
	_value = n << _fractionalBits;
	std::cout << "Int constructor called"<<std::endl;
}

Fixed::Fixed(const float f){
	_value = roundf(f *(1 << _fractionalBits));
	std::cout << "Float constructor called"<<std::endl;
}

Fixed::Fixed(const Fixed& other){
	std::cout <<"Copy constructor called" <<std::endl;
	_value = other.getRawBits();
}

Fixed& Fixed::operator=(const Fixed& other){
	std::cout <<"Copy assignment operator called" <<std::endl;
	if (this != &other)
		_value = other.getRawBits();
	return (*this);
}

Fixed::~Fixed(){
	std::cout <<"Destructor called"<<std::endl;
}

int Fixed::toInt(void) const{
	return (_value >> _fractionalBits);
}

float Fixed::toFloat(void) const{
	return ((float)_value / (1 << _fractionalBits));
}

int Fixed::getRawBits(void) const{
	return (_value);
}

void Fixed::setRawBits(int const raw){
	_value = raw;
}

std::ostream& operator<<(std::ostream& out, const Fixed& fixed){
	out << fixed.toFloat();
	return (out);
}