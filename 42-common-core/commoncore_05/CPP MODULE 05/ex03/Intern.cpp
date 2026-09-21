#include "Intern.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"


Intern::Intern(){}
Intern::Intern(Intern const& obj){
	(void)obj;
}
Intern::~Intern(){}
Intern& Intern::operator=(Intern const& obj){
	(void)obj;
	return *this;
}

struct formTable{
	std::string name;
	AForm* (*create)(std::string const&);
};

static AForm* createShrubbery(std::string const& target){
	return new ShrubberyCreationForm(target);
}

static AForm* createRobotomy(std::string const& target){
	return new RobotomyRequestForm(target);
}

static AForm* createPresidential(std::string const& target){
	return new PresidentialPardonForm(target);
}

AForm* Intern::makeForm(std::string const& name, std::string const& target){
	static formTable table[] = {
		{"shrubbery creation", &createShrubbery},
		{"robotomy request", &createRobotomy},
		{"presidential pardon", &createPresidential}
	};

	for (int i = 0; i < 3;i++){
		if (table[i].name == name){
			std::cout<<"Intern creates "<<name<<" form"<<std::endl;
			return table[i].create(target);
		}		
	}
	std::cout<<"Error: form name does not exist."<<std::endl;
	return NULL;
}
