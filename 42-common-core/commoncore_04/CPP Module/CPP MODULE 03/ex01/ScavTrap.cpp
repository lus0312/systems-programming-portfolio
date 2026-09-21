#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap(){
	hitPoints_ = 100;
	energyPoints_ = 50;
	attackDamage_= 20;
	std::cout << "ScavTrap default constructor called"<<std::endl;
}
ScavTrap::~ScavTrap(){
	std::cout <<"ScavTrap "<<name_<<" destructor called"<<std::endl;
}
ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name){
	hitPoints_ = 100;
	energyPoints_ = 50;
	attackDamage_= 20;
	std::cout << "ScavTrap "<<name_ <<" constructor called"<<std::endl;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other){
	std::cout <<"ScavTrap "<< name_<<" copy constructor called"<<std::endl;
}

ScavTrap& ScavTrap::operator=(const ScavTrap& other){
	if (this != &other){
		ClapTrap::operator=(other);
	}
	std::cout <<"ScavTrap "<<name_<<" copy assignment operator called"<<std::endl;
	return (*this);
}
void ScavTrap::attack(const std::string& target){
if (hitPoints_<= 0){
		std::cout << "ScavTrap "<<name_<<" cannot attack because it is already dead!"<<std::endl;
		return ;
	}
	else if (energyPoints_<= 0){
		std::cout << "ScavTrap "<<name_ <<" cannot attack because it has no energy left!"<<std::endl;
		return ;
	}
	std::cout <<"ScavTrap "<<name_<<" attacks "<<target<<", causing "<<attackDamage_<<" points of damage!"<<std::endl;
	energyPoints_ -= 1;
}
void ScavTrap::guardGate(){
	std::cout <<"ScavTrap "<<name_<<" is now in Gatekeeper mode."<<std::endl;
}