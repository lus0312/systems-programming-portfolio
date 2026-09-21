//#include "functions.hpp"
//#include <ctime>

//int main(){
//	std::srand(time(NULL));
//	Base* a = generate();
//	Base* b = generate();
//	Base* c = generate();

//	identify(a);
//	identify(*a);
//	identify(b);
//	identify(*b);
//	identify(c);
//	identify(*c);
//	delete a;
//	delete b;
//	delete c;
//	return 0;
//}

#include "functions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	Base*	ptr;
	A*		a_ptr;

	std::srand(std::time(NULL));

	ptr = generate();

	std::cout << "\033[1;38;2;109;212;136m=== identify by pointer ===\033[0m\n";
	identify(ptr);

	std::cout << "\033[1;38;2;109;212;136m=== identify by reference ===\033[0m\n";
	identify(*ptr);

	std::cout << "\033[1;38;2;109;212;136m=== dynamic_cast fail test ===\033[0m\n";
	a_ptr = dynamic_cast<A*>(ptr);
	if (a_ptr == NULL)
		std::cout << "This object is not A" << std::endl;
	else
		std::cout << "This object is A" << std::endl;

	delete ptr;
	return (0);
}