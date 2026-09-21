#include "HumanA.hpp"

HumanA::HumanA(std::string n, Weapon& w): name(n), weaponA(w){
}

HumanA::~HumanA(){
}

void HumanA::attack(){
	std::cout << name <<" attacks with their "<<weaponA.getType() <<std::endl;
}