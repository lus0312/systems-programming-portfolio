#include "ScavTrap.hpp"

int main() {
    ClapTrap A("Player");
//    ScavTrap B("Enemy");
    ScavTrap *B = new ScavTrap("Enemy");
    ClapTrap *C = new ScavTrap("Error");
    C->attack("ssssssssssss");
    std::cout << "___________________________________________"<<std::endl;
    A.attack("Enemy");  
    B->takeDamage(5); 
    B->attack("Player");
    C->attack("Player");
    A.takeDamage(0);
    A.attack("Enemy");
    B->takeDamage(5);
    B->beRepaired(10);
    B->attack("Player");
    for (int i = 0; i < 8; i++) {
        A.beRepaired(1);
    }
    A.attack("Player");
    delete B;
    delete C;
    return 0;
}