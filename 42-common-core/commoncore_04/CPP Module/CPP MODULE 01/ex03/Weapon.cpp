#include "Weapon.hpp"

Weapon::Weapon(std::string weaponType){
	type = weaponType;
}

Weapon::~Weapon(){
};

const std::string&	Weapon::getType(){
	std::string& typeREF = type;
	return typeREF;
}

void	Weapon::setType(std::string nType){
	type = nType;
}

