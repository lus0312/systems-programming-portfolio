#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include <cstdlib>
#include <ctime>
#include "Intern.hpp"

int main()
{
    std::srand(std::time(NULL));
    std::cout << "\033[1;38;2;109;212;136m===Intern makeForm test===\033[0m\n";
    {
        Intern intern;
        AForm* f1 = intern.makeForm("shrubbery creation", "home");
        AForm* f2 = intern.makeForm("robotomy request", "Bender");
        AForm* f3 = intern.makeForm("presidential pardon", "Marvin");

        std::cout << *f1 << std::endl;
        std::cout << *f2 << std::endl;
        std::cout << *f3 << std::endl;

        delete f1;
        delete f2;
        delete f3;
    }

    std::cout << "\033[1;38;2;109;212;136m===Unknown form test===\033[0m\n";
    {
        Intern intern;
        AForm* f = intern.makeForm("rrrobotomy form", "target");
        if (f)
            delete f;
        else
            std::cout << "Correctly handled NULL returned" << std::endl;
    }

	std::cout << "\033[1;38;2;109;212;136m===Execute without sign test===\033[0m\n";
	{
		Intern intern;
		Bureaucrat boss("Boss", 1);
		AForm* f = intern.makeForm("presidential pardon", "Marvin");

		if (f) {
			try {
				boss.executeForm(*f);
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			delete f;
		}
	}

	std::cout << "\033[1;38;2;109;212;136m===Full flow test===\033[0m\n";
	{
		Intern intern;
		Bureaucrat boss("Boss", 1);
		AForm* f = intern.makeForm("robotomy request", "Bender");

		if (f) {
			try {
				boss.signForm(*f);
				boss.executeForm(*f);
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			delete f;
		}
	}

	std::cout << "\033[1;38;2;109;212;136m===Grade too low execute test===\033[0m\n";
	{
		Intern intern;
		Bureaucrat lowB("lowB", 150);
		AForm* f = intern.makeForm("presidential pardon", "Marvin");
		if (f) {
			try {
				lowB.signForm(*f);
				lowB.executeForm(*f);
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			delete f;
		}
	}
    return 0;
}