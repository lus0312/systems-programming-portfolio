#include "Zombie.hpp"

int main(){
	Zombie* z = newZombie("Foo");
	z->announce();
	delete z;
	randomChump("Fooooo");
	return 0;
}