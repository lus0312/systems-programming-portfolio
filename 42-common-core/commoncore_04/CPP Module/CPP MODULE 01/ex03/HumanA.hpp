#ifndef HUMANA_HPP
# define HUMANA_HPP

# include "Weapon.hpp"
# include <iostream>

class HumanA{
private:
	std::string name;
	Weapon& weaponA;
public:
	HumanA(std::string nameA, Weapon& w);
	~HumanA();
	void attack();
};

#endif