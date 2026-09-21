#include "Contact.hpp"

void Contact::setFirstName(const std::string &name){
	firstName = name;
}
void Contact::setLastName(const std::string &name){
	lastName = name;
}
void Contact::setNickname(const std::string &name){
	nickname = name;
}
void Contact::setPhoneNumber(const std::string &number){
	phoneNumber = number;
}                                                                                                                                         
void Contact::setDarkestSecret(const std::string &secret){
	darkestSecret = secret;
}

const std::string Contact::getFirstName() const{
	return firstName;
}
const std::string Contact::getLastName() const{
	return lastName;
}
const std::string Contact::getNickname() const{
	return nickname;
}
const std::string Contact::getPhoneNumber() const{
	return phoneNumber;
}
const std::string Contact::getDarkestSecret() const{
	return darkestSecret;
}