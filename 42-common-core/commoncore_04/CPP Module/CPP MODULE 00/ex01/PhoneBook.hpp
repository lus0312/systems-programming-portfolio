#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

# include <iostream>
# include "Contact.hpp"

class PhoneBook{
private:
	Contact contacts[8];
	int currentSize;
	int	oldestIndex;
	std::string getInput(std::string str);
	std::string formatField(std::string str);
	
public:
	PhoneBook();
	~PhoneBook();
	void addContact();
	void searchContact();
};

#endif