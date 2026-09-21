#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type_("WrongAnimal"){
	std::cout<<"WrongAnimal constructor called"<<std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& other){
	type_ = other.type_;
	std::cout<<"WrongAnimal copy constructor called"<<std::endl;
}

WrongAnimal::~WrongAnimal(){
	std::cout<<"WrongAnimal destructor called"<<std::endl;
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& other){
	if (this != &other){
		type_ = other.type_;
		std::cout<<"WrongAnimal copy operator called"<<std::endl;
	}
	return (*this);
}

void WrongAnimal::makeSound() const{
	std::cout<<"..."<<std::endl;
}

std::string WrongAnimal::getType() const{
	return (type_);
}