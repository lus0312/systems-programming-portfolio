#include "PhoneBook.hpp"
#include "Contact.hpp"

int main(){
	PhoneBook pb;
	std::cout << "===== Welcome to my awesome phonebook =====" <<std::endl<< std::endl;

	while (true){
		std::cout << "Enter the command(ADD/SEARCH/EXIT): ";
		std::string input;
		std::getline(std::cin, input);
    		if (std::cin.eof())
        		break;
		if (input == "ADD"){
			pb.addContact();
		    	if (std::cin.eof())
        			break;
        	}
		else if (input == "SEARCH"){
			pb.searchContact();
		    	if (std::cin.eof())
        			break;
        	}
		else if (input== "EXIT")
			break;
		else
			continue;
	}
	return 0;
}
