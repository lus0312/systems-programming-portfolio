#include "FragTrap.hpp"


FragTrap::FragTrap() : ClapTrap(){
	hitPoints_ = 100;
	energyPoints_ = 100;
	attackDamage_ = 30;
	std::cout <<"FragTrap default constructor called"<<std::endl;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other){
	std::cout <<"FragTrap "<<name_<<" copy constructor called"<<std::endl;
}

FragTrap::~FragTrap(){
	std::cout <<"FragTrap "<<name_<<" destructor called"<<std::endl;
}

FragTrap& FragTrap::operator=(const FragTrap& other){
	if (this != &other)
		ClapTrap::operator=(other);
	std::cout <<"FragTrap "<<name_<<" copy assignment operator called"<<std::endl;
	return (*this);
}

FragTrap::FragTrap(const std::string& name) : ClapTrap(name){
	hitPoints_ = 100;
	energyPoints_ = 100;
	attackDamage_ = 30;	
	std::cout <<"FragTrap "<<name_<<" constructor called"<<std::endl;
}
void FragTrap::highFivesGuys(void){
	std::cout <<"FragTrap "<<name_<<" wants to do high five!"<<std::endl;
}