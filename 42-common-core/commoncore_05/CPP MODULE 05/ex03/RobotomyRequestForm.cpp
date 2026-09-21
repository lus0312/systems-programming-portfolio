#include "RobotomyRequestForm.hpp"
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm() : AForm("RobotomyRequestForm", 72, 45){}
RobotomyRequestForm::RobotomyRequestForm(std::string const & target) 
: AForm("RobotomyRequestForm", 72, 45),_target(target){}
RobotomyRequestForm::RobotomyRequestForm(RobotomyRequestForm const & obj) 
: AForm("RobotomyRequestForm", 72, 45),_target(obj._target){}
RobotomyRequestForm::~RobotomyRequestForm(){}
RobotomyRequestForm& RobotomyRequestForm::operator=(RobotomyRequestForm const & obj){
	if (this != & obj)
		_target = obj._target;
	return *this;
}

void RobotomyRequestForm::execute(Bureaucrat const & executor) const{
	checkExecute(executor);
	std::cout << "Drrrrr...drrrrr..."<<std::endl;
    if (std::rand() % 2)
        std::cout << _target << " has been robotomized successfully." << std::endl;
    else
        std::cout << _target << " robotomy failed." << std::endl;
}