#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm() : AForm("PresidentialPardonForm",25,5){}
PresidentialPardonForm::PresidentialPardonForm(std::string const & target)
: AForm("PresidentialPardonForm",25,5),_target(target){}
PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& obj)
: AForm("PresidentialPardonForm",25,5),_target(obj._target){}
PresidentialPardonForm::~PresidentialPardonForm(){}

void PresidentialPardonForm::execute(Bureaucrat const & executor) const{
	checkExecute(executor);
	std::cout<<_target<<" has been pardoned by Zaphod Beeblebrox."<<std::endl;
}