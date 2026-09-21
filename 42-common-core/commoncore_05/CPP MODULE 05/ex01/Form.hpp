#ifndef FORM_HPP
# define FORM_HPP

#include <iostream>
#include <exception>

class Bureaucrat;

class Form{
private:
	const std::string _name;
	bool _isSigned;
	const int _signGrade;
	const int _executeGrade;
public:
	Form();
	Form(const std::string& name, int signGrade, int executeGrade);
	Form(const Form& obj);
	~Form();
	Form& operator=(const Form& obj);
	
	std::string getName() const;
	bool getIsSigned() const;
	int getSignGrade() const;
	int getExecuteGrade() const;

	void beSigned(const Bureaucrat& obj);
	
	class GradeTooHighException : public std::exception{
	public: 
		virtual const char* what() const throw();
	};
	class GradeTooLowException : public std::exception{
	public: 
		virtual const char* what() const throw();
	};
	class AlreadySignedException : public std::exception{
		virtual const char* what() const throw();
	};
};

std::ostream& operator<<(std::ostream& os, const Form& obj);

#endif