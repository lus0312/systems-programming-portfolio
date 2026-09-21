#ifndef PRESIDENTIALPARDONFORM_HPP
# define PRESIDENTIALPARDONFORM_HPP

# include "AForm.hpp"

class PresidentialPardonForm : public AForm{
private:
	std::string _target;
public:
	PresidentialPardonForm();
	PresidentialPardonForm(std::string const & target);
	PresidentialPardonForm(const PresidentialPardonForm& obj);
	~PresidentialPardonForm();
	PresidentialPardonForm& operator=(const PresidentialPardonForm& obj);

	void execute(Bureaucrat const & executor) const;
};



#endif