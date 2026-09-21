#ifndef CLAPTRAP_HPP
# define CLAPTRAP_HPP

# include <iostream>

class ClapTrap{
private:
	std::string		name_;
	unsigned int	hitPoints_;
	unsigned int	energyPoints_;
	unsigned int	attackDamage_;
public:
	ClapTrap();
	ClapTrap(const ClapTrap& other);
	~ClapTrap();
	ClapTrap& operator=(const ClapTrap& other);
	ClapTrap(const std::string& name);
	void attack(const std::string& target);
	void takeDamage(unsigned int amount);
	void beRepaired(unsigned int amount);
};

#endif
