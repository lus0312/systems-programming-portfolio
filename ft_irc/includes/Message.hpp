
#ifndef Message_HPP
# define Message_HPP
# include <vector>
# include <string>

class Message
{
private:
	std::string					_command;
	std::vector<std::string>	_params;

public:
	Message();
	Message(const Message& other);
	Message& operator=(const Message& other);
	~Message();

	const std::string& getCommand() const;
	const std::vector<std::string>& getParams() const;

	bool parse(const std::string& line);
};

#endif
