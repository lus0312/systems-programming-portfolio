#include "Message.hpp"
#include <sstream>
#include <cctype>

Message::Message()
{
}

Message::Message(const Message& other)
	: _command(other._command),
	  _params(other._params)
{
}

Message& Message::operator=(const Message& other)
{
	if (this != &other) {
		_command = other._command;
		_params = other._params;
	}
	return *this;
}	

Message::~Message()
{
}

const std::string& Message::getCommand() const
{
	return _command;
}

const std::vector<std::string>& Message::getParams() const
{
	return _params;
}

bool Message::parse(const std::string& line)
{
	//framing거치고도 \r, \n 남아있는 경우 RFC 규격 위반으로 보고 그냥 거부
	if (line.find('\r') != std::string::npos 
		|| line.find('\n') != std::string::npos)
        return false;
	std::istringstream iss(line);
	std::string token;
	
	_command.clear();
	_params.clear();

	if (!(iss >> token))
		return false;
	if (token[0] == ':')
		return false;
	_command = token;
	for (std::size_t i = 0; i < _command.size(); ++i)
		_command[i] = std::toupper(static_cast<unsigned char>(_command[i]));
	while (iss >> token){
		if (token[0] == ':'){
			std::string trailing;
			std::getline(iss, trailing);
			_params.push_back(token.substr(1) + trailing);
			return true;
		}
		if (_params.size() >= 14)
		{
            std::string trailing;
            std::getline(iss, trailing);
            _params.push_back(token + trailing);
            return true;
        }
		_params.push_back(token);
	}
	return true;
}

