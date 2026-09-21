#include "Cat.hpp"

Cat::Cat() : Animal(){
	type_ = "Cat";
	std::cout<<"Cat constructor called"<<std::endl;
}

Cat::Cat(const Cat& other): Animal(other){
	type_ = other.type_;
	std::cout<<"Cat copy constructor called"<<std::endl;
}

Cat::~Cat(){
	std::cout<<"Cat destuctor called"<<std::endl;
}

Cat& Cat::operator=(const Cat& other){
	if (this != &other){
		Animal::operator=(other);
		std::cout<<"Cat copy assignment operator called"<<std::endl;
	}
	return (*this);
}

void Cat::makeSound() const{
	std::cout<<"Meow!"<<std::endl;
}