#ifndef INTERN_HPP
# define INTERN_HPP

#include "AForm.hpp"

class Intern{
public:
	Intern();
	Intern(Intern const& obj);
	~Intern();
	Intern& operator=(Intern const& obj);

	AForm* makeForm(std::string const& name, std::string const& target);
};

#endif