#ifndef HUMANB_HPP
# define HUMANB_HPP

# include "Weapon.hpp"
# include <iostream>

class HumanB{
private:
	Weapon* weaponType;
	std::string name;
public:
	HumanB(std::string name);
	~HumanB();
	void setWeapon(Weapon& w);
	void attack();

};

#endif