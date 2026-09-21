#include "ClapTrap.hpp"
#include <iostream>

int main() {
    ClapTrap A("Player");
    ClapTrap B("Enemy");

    A.attack("Enemy");  
    B.takeDamage(5);
    B.attack("Player");
    A.takeDamage(0);
    A.attack("Enemy");
    B.takeDamage(5);
    B.beRepaired(10);
    B.attack("Player");
    for (int i = 0; i < 8; i++) {
        A.beRepaired(1);
    }
    A.attack("Player");

    return 0;
}
