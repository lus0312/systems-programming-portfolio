#include "PhoneBook.hpp"
#include <iomanip>
#include <sstream>

PhoneBook::PhoneBook(){
	currentSize = 0;
	oldestIndex = 0;
}

PhoneBook::~PhoneBook(){
}

std::string PhoneBook::formatField(std::string str){
	if (str.length() > 10)
		str = str.substr(0,9) + ".";
	return str;
}

std::string PhoneBook::getInput(std::string prompt){
	std::string line;
	std::cout << prompt;
	std::getline(std::cin, line);
	while (line.empty()){
	        if (std::cin.eof())
			return "";
		std::cout << "Error: Field cannot be empty!"<<std::endl;
		std::cout << prompt;
		std::getline(std::cin, line);
	}
	return line;
}

void PhoneBook::addContact(){


	if (currentSize < 8)
		currentSize++;
	
	contacts[oldestIndex].setFirstName(getInput("First name: "));
	if (std::cin.eof()) return ;
	contacts[oldestIndex].setLastName(getInput("Last name: "));
	if (std::cin.eof()) return ;
	contacts[oldestIndex].setNickname(getInput("Nickname: "));
	if (std::cin.eof()) return ;
	contacts[oldestIndex].setPhoneNumber(getInput("Phone number: "));
	if (std::cin.eof()) return ;
	contacts[oldestIndex].setDarkestSecret(getInput("Darkest Secret: "));
	if (std::cin.eof()) return ;

	if (oldestIndex == 7)
		oldestIndex = 0;
	else
		oldestIndex++;
}

void PhoneBook::searchContact(){
	if (currentSize == 0){
		std::cout << "PhoneBook is empty!" <<std::endl;
		return ;
	}
	std::cout << "|" <<std::setw(10) <<"index"
			  << "|" <<std::setw(10) <<"first name"
			  << "|" <<std::setw(10) <<"last name"
			  << "|" <<std::setw(10) <<"nickname"
			  << "|" <<std::endl;
	for (int i = 0; i < currentSize; i++){
		std::cout << "|" <<std::setw(10) <<std::right << i+1
				  << "|" <<std::setw(10) <<std::right <<formatField(contacts[i].getFirstName())
				  << "|" <<std::setw(10) <<std::right <<formatField(contacts[i].getLastName())
				  << "|" <<std::setw(10) <<std::right <<formatField(contacts[i].getNickname())
				  << "|"<<std::endl;
	}
	int index = -1;
	std::string input;
	while (index < 1 || index >= currentSize){
		std::cout << "Enter the index: ";
		std::getline(std::cin, input);
		if (std::cin.eof())
			return ;  
		if (input.empty()){
			std::cout << "Error: Field cannot be empty!"<<std::endl;
			continue;
		}
		std::istringstream iss(input);
		if (!(iss >> index)){
			std::cout << "Error: Invalid index!"<<std::endl;
			continue;
		}
		if (index < 1 || index > currentSize){
			std::cout << "Error: Invalid index!"<<std::endl;
			continue;
		}
		else
			break;
	}
	std::cout <<"first name: " <<contacts[index-1].getFirstName()<<std::endl;
	std::cout <<"last name: " <<contacts[index-1].getLastName()<<std::endl;
	std::cout <<"nickname: " <<contacts[index-1].getNickname()<<std::endl;
	std::cout <<"phone number: " <<contacts[index-1].getPhoneNumber()<<std::endl;
	std::cout <<"darkest secret: " <<contacts[index-1].getDarkestSecret()<<std::endl;
}
