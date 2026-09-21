#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(): _name("default"),_isSigned(false),_signGrade(10),_executeGrade(10){}
AForm::AForm(const std::string& name, int signGrade, int executeGrade)
: _name(name), _isSigned(false), _signGrade(signGrade), _executeGrade(executeGrade){
	if (_signGrade < 1)
		throw GradeTooHighException();
	if (_signGrade >150)
		throw GradeTooLowException();
	if (_executeGrade < 1)
		throw GradeTooHighException();
	if (_executeGrade >150)
		throw GradeTooLowException();
}
AForm::AForm(const AForm& obj)
: _name(obj._name),_isSigned(obj._isSigned),_signGrade(obj._signGrade),_executeGrade(obj._executeGrade){}
AForm::~AForm(){}
AForm& AForm::operator=(const AForm& obj){
	if (this != &obj)
		_isSigned = obj._isSigned;
	return *this;
}
std::string AForm::getName() const{
	return _name;
}
bool AForm::getIsSigned() const{
	return _isSigned;
}
int AForm::getSignGrade() const{
	return _signGrade;
}

int AForm::getExecuteGrade() const{
	return _executeGrade;
}

void AForm::beSigned(const Bureaucrat& obj){
	if (_isSigned)
		throw AlreadySignedException();
	int grade = obj.getGrade();
	if (grade > _signGrade){
		throw GradeTooLowException();
	}
	_isSigned = true;
}

const char* AForm::GradeTooHighException::what() const throw(){
	return "grade is too high.";
}
const char* AForm::GradeTooLowException::what() const throw(){
	return "grade is too low.";
}
const char* AForm::AlreadySignedException::what() const throw(){
	return "form is already signed.";
}
const char* AForm::NotSignedException::what() const throw(){
	return "form is not signed yet.";
}

void AForm::checkExecute(Bureaucrat const &executor) const{
	if (!_isSigned)
		throw NotSignedException();
	if (getExecuteGrade() < executor.getGrade())
		throw GradeTooLowException();
}

std::ostream& operator<<(std::ostream& os, const AForm& obj){
	os <<obj.getName()<<", signGrade: "<<obj.getSignGrade()
	<<", execGrade: "<<obj.getExecuteGrade()<<", signed: "
	<<std::boolalpha<<obj.getIsSigned()<<".";
	return os;
}

