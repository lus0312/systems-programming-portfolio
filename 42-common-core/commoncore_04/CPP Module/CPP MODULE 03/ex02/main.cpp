#include "FragTrap.hpp"

int main() {
    ClapTrap    A("player_A");
    FragTrap    B("player_B");
    B.highFivesGuys();
    B.attack("player_A");
    A.takeDamage(30);
    B.attack("player_A");
    A.takeDamage(30);
    B.attack("player_A");
    A.takeDamage(30);
    B.attack("player_A");
    A.takeDamage(30);
    A.beRepaired(30000);
    for (int i = 0; i < 97; i++){
        B.attack("player_A");
        A.takeDamage(30);
    }
    return 0;
}