#include "Bureaucrat.hpp"

int main(){
	std::cout << "\033[1;38;2;109;212;136m---constructing test---\033[0m\n";
	std::string names[] = {"A", "B", "C", "D", "E"};
	int grades[] = {0,1,75,150, 151};
	for (int i = 0; i < 5; i++){
		try{
			Bureaucrat b(names[i], grades[i]);
			std::cout<<b<<std::endl;
		}
		catch(std::exception& e){
			std::cout <<e.what()<<std::endl;
		}
	}
	std::cout << "\033[1;38;2;109;212;136m---incremention/decremention test---\033[0m\n";
	Bureaucrat a("Amy", 1);
	try{
		a.decrementGrade();
		std::cout<<a<<std::endl;
		a.incrementGrade();
		std::cout<<a<<std::endl;
		a.incrementGrade();
		std::cout<<a<<std::endl;
	}
	catch(std::exception& e){
		std::cout<<e.what()<<std::endl;
	}
	Bureaucrat b("Bob", 150);
	try{
		b.incrementGrade();
		std::cout<<b<<std::endl;
		b.decrementGrade();
		std::cout<<b<<std::endl;
		b.decrementGrade();
	}
	catch(std::exception& e){
		std::cout<<e.what()<<std::endl;
	}
	return 0;
}