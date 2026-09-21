#include "Zombie.hpp"

Zombie* newZombie(std::string str){
	Zombie* z = new Zombie(str);
	return z;
}