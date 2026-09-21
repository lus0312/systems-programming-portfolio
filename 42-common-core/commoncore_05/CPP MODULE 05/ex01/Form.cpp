#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form(): _name("default"),_isSigned(false),_signGrade(10),_executeGrade(10){}
Form::Form(const std::string& name, int signGrade,int executeGrade)
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
Form::Form(const Form& obj)
: _name(obj._name),_isSigned(obj._isSigned),_signGrade(obj._signGrade),_executeGrade(obj._executeGrade){}
Form::~Form(){}
Form& Form::operator=(const Form& obj){
	if (this != &obj)
		_isSigned = obj._isSigned;
	return *this;
}
std::string Form::getName() const{
	return _name;
}
bool Form::getIsSigned() const{
	return _isSigned;
}
int Form::getSignGrade() const{
	return _signGrade;
}
int Form::getExecuteGrade() const{
	return _executeGrade;
}

void Form::beSigned(const Bureaucrat& obj){
	if (_isSigned)
		throw AlreadySignedException();
	int grade = obj.getGrade();
	if (grade > _signGrade)
		throw GradeTooLowException();
	_isSigned = true;
}

const char* Form::GradeTooHighException::what() const throw(){
	return "grade is too high.";
}
const char* Form::GradeTooLowException::what() const throw(){
	return "grade is too low.";
}
const char* Form::AlreadySignedException::what() const throw(){
	return "form is already signed.";
}

std::ostream& operator<<(std::ostream& os, const Form& obj){
	os <<obj.getName()<<", signGrade: "<<obj.getSignGrade()
	<<", execGrade: "<<obj.getExecuteGrade()<<", signed: "
	<<std::boolalpha<<obj.getIsSigned()<<".";
	return os;
}
