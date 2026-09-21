#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <iostream>

class Zombie
{
private:
	std::string name;
public:
	Zombie(std::string str);
	~Zombie();
	void announce(void);
};

Zombie* newZombie(std::string str);
void	randomChump(std::string str);

#endif