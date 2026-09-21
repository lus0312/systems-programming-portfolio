#include "HumanB.hpp"

HumanB::HumanB(std::string n){
	name = n;
}

HumanB::~HumanB(){
}

void	HumanB::setWeapon(Weapon& w){
	weaponType = &w;
}

void	HumanB::attack(){
	std::cout << name <<" attacks with their "<<weaponType->getType() <<std::endl;
}