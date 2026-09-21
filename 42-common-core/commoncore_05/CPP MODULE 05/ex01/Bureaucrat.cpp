#include "Bureaucrat.hpp"
#include "Form.hpp"

Bureaucrat::Bureaucrat() : _name("default"), _grade(10){}
Bureaucrat::Bureaucrat(const std::string& name, int grade): _name(name), _grade(grade){
	if (_grade < 1)
		throw GradeTooHighException();
	if (_grade > 150)
		throw GradeTooLowException();
}
Bureaucrat::Bureaucrat(const Bureaucrat& obj): _name(obj._name), _grade(obj._grade){}
Bureaucrat::~Bureaucrat(){}
Bureaucrat& Bureaucrat::operator=(const Bureaucrat& obj){
	if (this != &obj)
		_grade = obj._grade;
	return (*this);
}
std::string Bureaucrat::getName()const{
	return _name;
}
int Bureaucrat::getGrade() const{
	return _grade;
}

const char* Bureaucrat::GradeTooHighException::what() const throw(){
	return "Grade is too high.";
}
const char* Bureaucrat::GradeTooLowException::what() const throw(){
	return "Grade is too low.";
}

void Bureaucrat::incrementGrade(){
	if (_grade <=1)
		throw GradeTooHighException();
	_grade--;
}
void Bureaucrat::decrementGrade(){
	if (_grade >= 150)
		throw GradeTooLowException();
	_grade++;
}

void Bureaucrat::signForm(Form& obj){
	try{
		obj.beSigned(*this);
		std::cout<<_name<<" signed "<<obj.getName()<<std::endl;
	}
	catch(std::exception& e){
		std::cout<<_name<<" couldn't sign "<<obj.getName()
		<<" because "<<e.what()<<std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& obj){
	os <<obj.getName()<<", bureaucrat grade "<<obj.getGrade()<<".";
	return os;
}