#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <string>
# include <vector>
# include "Client.hpp"

struct ChannelMode
{
	ChannelMode()
		: inviteOnly(false),
		  topicRestricted(false),
		  hasKey(false),
		  key(),
		  hasUserLimit(false),
		  userLimit(0)
	{
	}

	bool		inviteOnly;
	bool		topicRestricted;
	bool		hasKey;
	std::string	key;
	bool		hasUserLimit;
	std::size_t	userLimit;
};

class Channel
{
private:
	std::string			_topic;
	std::string			_name;
	std::vector<int>	_membersFd;
	std::vector<int>	_operatorsFd;
	std::vector<int>	_invitedFds;
	ChannelMode			_mode;
public:
	Channel();
	Channel(const std::string& name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	const std::string& getName() const;
	const std::string& getTopic() const;
	void setTopic(const std::string& topic);
	ChannelMode& getMode();
	const ChannelMode& getMode() const;

	bool hasMember(int fd) const;
	void addMember(int fd);
	void removeMember(int fd);

	bool isOperator(int fd) const;
	void addOperator(int fd);
	void removeOperator(int fd);

	bool isInvited(int fd) const;
	void addInvited(int fd);
	void removeInvited(int fd);

	void removeClientState(int fd);

	std::size_t getMemberCount() const;
	const std::vector<int>& getMembersFd() const;
};

#endif
