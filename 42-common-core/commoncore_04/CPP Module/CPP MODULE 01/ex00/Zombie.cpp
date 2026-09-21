#include "Zombie.hpp"

Zombie::Zombie(std::string str){
	name = str;
}

Zombie::~Zombie(){
	std::cout << name << " is destroyed!"<<std::endl;
}

void	Zombie::announce(void){
	std::cout << name <<": BraiiiiiiinnnzzzZ..."<<std::endl;
}