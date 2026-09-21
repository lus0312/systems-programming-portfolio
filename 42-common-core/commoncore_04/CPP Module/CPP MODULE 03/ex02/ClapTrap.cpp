#include "ClapTrap.hpp"
#include <iostream>

ClapTrap::ClapTrap(): name_("default"), hitPoints_(10), energyPoints_(10), attackDamage_(0){
	std::cout << "ClapTrap "<<name_<<" constructor called"<<std::endl;
}
ClapTrap::ClapTrap(const ClapTrap& other){
	name_ = other.name_;
	hitPoints_ = other.hitPoints_;
	energyPoints_ = other.energyPoints_;
	attackDamage_ = other.attackDamage_;
	std::cout <<"ClapTrap "<< name_<<" copy constructor called"<<std::endl;
}
ClapTrap::~ClapTrap(){
	std::cout <<"ClapTrap "<<name_<<" destructor called"<<std::endl;
}
ClapTrap& ClapTrap::operator=(const ClapTrap& other){
	if (this != &other){
		name_ = other.name_;
		hitPoints_ = other.hitPoints_;
		energyPoints_ = other.energyPoints_;
		attackDamage_ = other.attackDamage_;
	}
	std::cout <<"ClapTrap "<<name_<<" copy assignment operator called"<<std::endl;
	return (*this);
}
ClapTrap::ClapTrap(const std::string& name) : name_(name), hitPoints_(10), energyPoints_(10), attackDamage_(0){
	std::cout << "ClapTrap "<< name <<" constructor called"<<std::endl; 
}

void ClapTrap::attack(const std::string& target){
	if (this->hitPoints_<= 0){
		std::cout << "ClapTrap "<< this->name_<<" cannot attack because it is already dead!"<<std::endl;
		return ;
	}
	else if (this->energyPoints_<= 0){
		std::cout << "ClapTrap "<< this->name_ <<" cannot attack because it has no energy left!"<<std::endl;
		return ;
	}
	std::cout <<"ClapTrap "<< this->name_<<" attacks "<<target<<", causing "<<this->attackDamage_<<" points of damage!"<<std::endl;
	this->energyPoints_ -= 1;
}
void ClapTrap::takeDamage(unsigned int amount){
	if (this->hitPoints_<= 0){
		std::cout << "ClapTrap "<< this->name_<< " is already dead!"<<std::endl;
		return ;
	}
	if (amount > this->hitPoints_)
		this->hitPoints_ = 0;
	else
		this->hitPoints_ -= amount;
	std::cout <<"ClapTrap "<<this->name_<<" takes "<<amount <<" damage!"<<std::endl;

}
void ClapTrap::beRepaired(unsigned int amount){
	if (this->hitPoints_<= 0){
		std::cout << "ClapTrap "<< this->name_<<" cannot be repaired because it is already dead!"<<std::endl;
		return ;
	}
	else if (this->energyPoints_<= 0){
		std::cout << "ClapTrap "<< this->name_ <<" cannot be repaired because it has no energy left!"<<std::endl;
		return ;
	}
	this->hitPoints_ += amount;
	this->energyPoints_ -= 1;
	std::cout << "ClapTrap "<<this->name_<<" repairs " <<amount<< " hitPoints!"<<std::endl;
}