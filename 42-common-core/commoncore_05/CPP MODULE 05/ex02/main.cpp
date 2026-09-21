#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(NULL));
	std::cout << "\033[1;38;2;109;212;136m===constructing test===\033[0m\n";
    try {
        AForm* form = new ShrubberyCreationForm("home");
        std::cout << *form << std::endl;
        delete form;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	std::cout << "\033[1;38;2;109;212;136m===sign test===\033[0m\n";
    try {
        Bureaucrat b1("HighB", 1);
        Bureaucrat b2("LowB", 150);
        ShrubberyCreationForm f1("home");

        std::cout << f1 << std::endl;
        b2.signForm(f1);
        b1.signForm(f1);
        std::cout << f1 << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
	std::cout << "\033[1;38;2;109;212;136m===execute && no sign===\033[0m\n";
    try {
        Bureaucrat b("Boss", 1);
        ShrubberyCreationForm f("home");
        b.executeForm(f);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
	std::cout << "\033[1;38;2;109;212;136m===ShrubberyCreationForm execute test===\033[0m\n";
    try {
        Bureaucrat b("Boss", 1);
        ShrubberyCreationForm f("tree");
        b.signForm(f);
        b.executeForm(f);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
	std::cout << "\033[1;38;2;109;212;136m===RobotomyRequestForm test===\033[0m\n";
    try {
        Bureaucrat b("Boss", 1);
        RobotomyRequestForm f("Bender");
        b.signForm(f);
        b.executeForm(f);
        b.executeForm(f);
        b.executeForm(f);
        b.executeForm(f);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "\033[1;38;2;109;212;136m===PresidentialPardonForm test===\033[0m\n";
    try {
        Bureaucrat b("Boss", 1);
        PresidentialPardonForm f("Marvin");
        b.signForm(f);
        b.executeForm(f);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
	std::cout << "\033[1;38;2;109;212;136m===execute grade test===\033[0m\n";
    try {
        Bureaucrat b("LowB", 72);
        RobotomyRequestForm f("Bender");
        b.signForm(f);
        b.executeForm(f);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}