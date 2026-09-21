#include "Dog.hpp"

Dog::Dog(): Animal(){
	type_ = "Dog";
	dBrain = new Brain();
	std::cout <<type_<<" constructor called"<<std::endl;
}

Dog::Dog(const Dog& other) : Animal(other){
	type_ = other.type_;
	dBrain = new Brain(*other.dBrain);
	std::cout<<type_<<" copy constructor called"<<std::endl;
}

Dog::~Dog(){
	delete dBrain;
	std::cout<<"Dog destructor called"<<std::endl;
}

Dog& Dog::operator=(const Dog& other){
	if (this != &other){
		Animal::operator=(other);
		delete dBrain;
		dBrain = new Brain(*other.dBrain);
		std::cout<<type_ <<" copy assignment operator called"<<std::endl;
	}
	return (*this);
}

void Dog::makeSound() const{
	std::cout<<"BowWow!"<<std::endl;
}

Brain& Dog::getBrain(){
	return (*dBrain);
}