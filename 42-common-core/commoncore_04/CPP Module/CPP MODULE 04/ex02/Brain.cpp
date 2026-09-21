#include "Brain.hpp"

Brain::Brain(){
	std::cout<<"Brain constructor called"<<std::endl;
}
Brain::Brain(const Brain& other){
	for (int i = 0; i < 100; i++)
		ideas[i] = other.ideas[i];
	std::cout<<"Brain copy constructor called"<<std::endl;
}
Brain::~Brain(){
	std::cout<<"Brain destructor called"<<std::endl;
}
Brain& Brain::operator=(const Brain& other){
	if (this != &other){
		for (int i = 0; i < 100; i++)
			ideas[i] = other.ideas[i];
		std::cout<<"Brain copy assignment operator called"<<std::endl;
	}
	return (*this);
}

std::string Brain::getIdea(const int idx) const{
	if (idx < 0 || idx > 99){
	    std::cerr << "Brain: index out of range" << std::endl;
		return "";
	}
	return (ideas[idx]);
}
void Brain::setIdea(const int idx, const std::string idea){
	if (idx < 0 || idx > 99){
	    std::cerr << "Brain: index out of range" << std::endl;
		return ;
	}
	ideas[idx] = idea;
}