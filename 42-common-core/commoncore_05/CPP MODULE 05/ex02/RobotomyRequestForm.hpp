#ifndef ROBOTOMYREQUESTFORM_HPP
# define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"

class RobotomyRequestForm : public AForm{
private:
	std::string _target;
public:
	RobotomyRequestForm();
	RobotomyRequestForm(std::string const & target);
	RobotomyRequestForm(RobotomyRequestForm const & obj);
	~RobotomyRequestForm();
	RobotomyRequestForm& operator=(RobotomyRequestForm const & obj);

	void execute(Bureaucrat const &executor) const;
};

#endif