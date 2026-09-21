#ifndef CONTACT_HPP
# define CONTACT_HPP

# include <iostream>

class Contact{
private:
	std::string firstName;
	std::string lastName;
	std::string nickname;
	std::string phoneNumber;
	std::string darkestSecret;
public:
	void setFirstName(const std::string &name);
	void setLastName(const std::string &name);
	void setNickname(const std::string &name);
	void setPhoneNumber(const std::string &number);
	void setDarkestSecret(const std::string &secret);
	
	const std::string getFirstName() const;
	const std::string getLastName() const;
	const std::string getNickname() const;
	const std::string getPhoneNumber() const;
	const std::string getDarkestSecret() const;
};

#endif