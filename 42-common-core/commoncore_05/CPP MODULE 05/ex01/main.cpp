#include "Bureaucrat.hpp"
#include "Form.hpp"

int main(){

	std::cout << "\033[1;38;2;109;212;136m---constructing test---\033[0m\n";
	const int gradeSigns[] = {0,1,75,150,151};
	std::string names[] = {"A","B","C","D","E"};
	for (int i = 0; i < 5; i++){
		try{
			Form f(names[i],gradeSigns[i], 10);
			std::cout <<f<<std::endl;
		}
		catch(std::exception &e){
			std::cout <<e.what()<<std::endl;
		}
	}
	std::cout << "\033[1;38;2;109;212;136m---sign test---\033[0m\n";
	Bureaucrat b1("Amy",5);
	Bureaucrat b2("Bob",75);
	Form f1("first paper", 1, 1);
	Form f2("second paper",5, 1);
	std::cout<<f1<<std::endl;
	std::cout<<f2<<std::endl;
	b1.signForm(f1);
	b1.signForm(f2);
	b1.signForm(f2);
	b2.signForm(f1);
	b2.signForm(f2);
	std::cout<<f1<<std::endl;
	std::cout<<f2<<std::endl;
	return 0;
}