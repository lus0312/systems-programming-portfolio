#include "Channel.hpp"
#include <iostream>

Channel::Channel()
{
}

Channel::Channel(const std::string& name) :_name(name)
{
}

Channel::Channel(const Channel& other) 
	: _topic(other._topic),
	  _name(other._name),
	  _membersFd(other._membersFd),
	  _operatorsFd(other._operatorsFd),
	  _invitedFds(other._invitedFds),
	  _mode(other._mode)
{
}

Channel& Channel::operator=(const Channel& other)
{

	if (this != &other) {
		_topic = other._topic;
		_name = other._name;
		_membersFd = other._membersFd;
		_operatorsFd = other._operatorsFd;
		_invitedFds = other._invitedFds;
		_mode = other._mode;
	}
	return *this;
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
	return _name;
}
const std::string& Channel::getTopic() const
{
	return _topic;
}

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

ChannelMode& Channel::getMode()
{
	return _mode;
}

const ChannelMode& Channel::getMode() const
{
	return _mode;
}

bool Channel::hasMember(int fd) const
{
	for (std::size_t i = 0; i < _membersFd.size(); i++)
	{
		if (_membersFd[i] == fd)
			return true;
	}
	return false;
}
void Channel::addMember(int fd)
{
	if (!hasMember(fd))
		_membersFd.push_back(fd);
}
void Channel::removeMember(int fd)
{
	for (std::vector<int>::iterator it = _membersFd.begin(); it != _membersFd.end(); it++)
	{
		if (*it == fd){
			_membersFd.erase(it);
			return ;
		}
	}
}

bool Channel::isOperator(int fd) const
{
	for (std::size_t i = 0; i < _operatorsFd.size(); i++)
	{
		if (_operatorsFd[i] == fd)
			return true;
	}
	return false;
}
void Channel::addOperator(int fd)
{
	if (!isOperator(fd))
		_operatorsFd.push_back(fd);
}

void Channel::removeOperator(int fd)
{
	for(std::vector<int>::iterator it = _operatorsFd.begin(); it != _operatorsFd.end(); it++){
		if (*it == fd){
			_operatorsFd.erase(it);
			return ;
		}
	}
}

bool Channel::isInvited(int fd) const
{
	for (std::vector<int>::const_iterator it =_invitedFds.begin();
		it != _invitedFds.end(); it++){
		if (*it == fd)
			return true;
	}
	return false;
}
void Channel::addInvited(int fd)
{
	if (!isInvited(fd))
		_invitedFds.push_back(fd);
}

void Channel::removeInvited(int fd)
{
	for (std::vector<int>::iterator it = _invitedFds.begin();
		it != _invitedFds.end(); it++)
	{
		if (*it == fd)
		{
			_invitedFds.erase(it);
			return ;
		}	
	}
}

void Channel::removeClientState(int fd)
{
	removeMember(fd);
	removeOperator(fd);
	removeInvited(fd);
}


std::size_t Channel::getMemberCount() const
{
	return (_membersFd.size());
}

const std::vector<int>& Channel::getMembersFd() const
{
	return _membersFd;
}