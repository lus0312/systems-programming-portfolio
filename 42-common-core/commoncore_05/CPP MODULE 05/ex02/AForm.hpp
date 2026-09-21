#ifndef AForm_HPP
# define AForm_HPP

#include <iostream>
#include <exception>

class Bureaucrat;

class AForm{
private:
	const std::string _name;
	bool _isSigned;
	const int _signGrade;
	const int _executeGrade;
public:
	AForm();
	AForm(const std::string& name, int signGrade, int executeGrade);
	AForm(const AForm& obj);
	virtual ~AForm();
	AForm& operator=(const AForm& obj);
	
	std::string getName() const;
	bool getIsSigned() const;
	int getSignGrade() const;
	int getExecuteGrade() const;

	void beSigned(const Bureaucrat& obj);
	
	class GradeTooHighException: public std::exception{
	public: 
		virtual const char* what() const throw();
	};
	class GradeTooLowException: public std::exception{
	public: 
		virtual const char* what() const throw();
	};
	class AlreadySignedException : public std::exception{
		virtual const char* what() const throw();
	};
	class NotSignedException: public std::exception{
	public:
		virtual const char* what() const throw();
	};

	void checkExecute(Bureaucrat const &executor) const;
	virtual void execute(Bureaucrat const &executor) const = 0;
};

std::ostream& operator<<(std::ostream& os, const AForm& obj);

#endif