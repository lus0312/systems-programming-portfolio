#include "functions.hpp"

Base* generate(void){
	int r = std::rand() % 3;
    //std::cout << "generated: " << r << std::endl; 
	if (r == 0)
		return new A();
	else if (r == 1)
		return new B();
	return new C();
}

void identify(Base* p){
	if (dynamic_cast<A*>(p))
		std::cout<<"original type: A"<<std::endl;
	else if (dynamic_cast<B*>(p))
		std::cout<<"original type: B"<<std::endl;
	else if (dynamic_cast<C*>(p))
		std::cout<<"original type: C"<<std::endl;
}

void identify(Base& p){
	try{
		dynamic_cast<A&>(p);
		std::cout<<"original type: A"<<std::endl;
	}
	catch(std::exception &e){}
	try{
		dynamic_cast<B&>(p);
		std::cout<<"original type: B"<<std::endl;
	}
	catch(std::exception &e){}
	try{
		dynamic_cast<C&>(p);
		std::cout<<"original type: C"<<std::endl;
	}
	catch(std::exception &e){}
}
