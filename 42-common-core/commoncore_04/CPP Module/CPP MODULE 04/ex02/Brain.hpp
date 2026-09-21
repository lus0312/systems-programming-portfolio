#ifndef BRAIN_HPP
# define BRAIN_HPP

#include <iostream>

class Brain{
private:
	std::string ideas[100];
public:
	Brain();
	Brain(const Brain& other);
	~Brain();
	Brain& operator=(const Brain& other);
	std::string getIdea(const int idx) const;
	void setIdea(const int idx, const std::string idea);
};

#endif
