#include "Controller.hpp"
#include <iostream>
#include <sstream>
#include <cerrno>
#include <cstdlib>
#include <cctype>
#include <set>

static const std::size_t MAX_WRITE_BUFFER = 32 * 1024 * 1024;

static char makeIrcCompareChar(char c)
{
	if ('A' <= c && c <= 'Z')
		return c + ('a' - 'A');
	if (c == '[')
		return '{';
	if (c == ']')
		return '}';
	if (c == '\\')
		return '|';
	if (c == '~')
		return '^';
	return c;
}

static std::string makeIrcCompareKey(const std::string& str)
{
	std::string result = str;

	for (std::size_t i = 0; i < result.size(); ++i)
		result[i] = makeIrcCompareChar(result[i]);
	return result;
}

Controller::Controller(const std::string& password) : _password(password)
{
}

Controller::~Controller()
{
}

void Controller::addClient(int fd)
{
    Client client(fd);
    _clients.insert(std::make_pair(fd, client));
}

Client* Controller::findClientByFd(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return NULL;
	return &(it->second);
}

Client* Controller::findClientByNickname(const std::string& nickname)
{
	std::string target = makeIrcCompareKey(nickname);
	for(std::map<int, Client>::iterator it = _clients.begin();
		it != _clients.end(); it++)
		{
			if(makeIrcCompareKey(it->second.getNickname()) == target)
				return &(it->second);
		}
	return NULL;
}

void Controller::removeClient(int fd)
{
	Client* client = findClientByFd(fd);
	if (client != NULL && client->getRegistered())
		broadcastToSharedClients(fd, makePrefix(*client)
			+ " QUIT :" + client->getQuitReason());
	removeClientFromAllChannels(fd);
	removeEmptyChannels();
	_clients.erase(fd);

}

bool Controller::isNicknameTaken(const std::string& nickname, int excludeFd) const
{
	std::string target = makeIrcCompareKey(nickname);
	for (std::map<int, Client>::const_iterator it = _clients.begin(); 
		it != _clients.end(); it++)
	{
		if (it->first != excludeFd 
			&& makeIrcCompareKey(it->second.getNickname()) == target)
			return true;
	}
	return false;
}


bool Controller::isValidNickname(const std::string& nickname) const
{
	if (nickname.empty() || nickname.size() > 30)
		return false;
	const std::string special = "[]\\`_^{|}";
	for (std::size_t i = 0; i < nickname.size(); ++i)
	{
		const unsigned char ch = static_cast<unsigned char>(nickname[i]);
		const bool letter = (ch >= 'A' && ch <= 'Z')
			|| (ch >= 'a' && ch <= 'z');
		const bool extra = special.find(ch) != std::string::npos;
		const bool digitOrHyphen = (ch >= '0' && ch <= '9') || ch == '-';
		if (!letter && !extra && !(i > 0 && digitOrHyphen))
			return false;
	}
	return true;
}

bool Controller::isValidChannelName(const std::string& name) const
{
	if (name.empty())
		return false;
	if (name[0] != '#' && name[0] != '&' && name[0] != '+' && name[0] != '!')
		return false;
	if (name.size()> 50)
		return false;
	for (std::size_t i = 0; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
			return false;
	}
	return true;
}

void Controller::getClientsToClose(std::vector<int>& fds) const
{
	for (std::map<int, Client>::const_iterator it = _clients.begin();
		it != _clients.end(); ++it)
	{
		if (it->second.shouldClose())
			fds.push_back(it->first);
	}
}

Channel* Controller::findChannel(const std::string& name)
{
	for (std::size_t i = 0; i < _channels.size(); i++)
	{
		if (makeIrcCompareKey(name) == makeIrcCompareKey(_channels[i].getName()))
			return &_channels[i];
	}
	return NULL;
}

Channel* Controller::createChannel(const std::string& name)
{
	_channels.push_back(Channel(name));
	return &_channels.back();
}

void Controller::removeClientFromAllChannels(int fd)
{
	for (std::size_t i = 0; i < _channels.size(); ++i)
		_channels[i].removeClientState(fd);
}

void Controller::removeEmptyChannels()
{
	std::vector<Channel>::iterator it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->getMemberCount() == 0)
			it = _channels.erase(it);
		else
			it++;
	}
}

std::string Controller::makePrefix(const Client& client) const
{
    std::string prefix = ":" + client.getNickname();
    if (!client.getUsername().empty())
        prefix += "!" + client.getUsername();
    prefix += "@localhost"; 
    return prefix;
}

void Controller::sendWelcome(Client& client){
	std::string message;

	message = ":ircserv 001 " + client.getNickname()
		+ " :Welcome to the ft_irc server, "
		+ client.getNickname();

	enqueueMessage(client, message);
}

void Controller::checkAndRegister(Client& client)
{
	if (client.getRegistered())
		return ;
	if (!client.getPassOk())
		return ;
	if (client.getUsername().empty())
		return ;
	if (client.getNickname().empty())
		return ;
	client.setRegistered(true);
	sendWelcome(client);
}

bool Controller::processMessage(int clientFd, const Message& message)
{
	Client* client = findClientByFd(clientFd);
    if (!client)
		return true;
    const std::string& command = message.getCommand();
    // 등록 전(PASS/NICK/USER) 사용 불가 명령어 차단
    if (!client->getRegistered() && !canUseBeforeRegistration(command)) 
	{
        enqueueMessage(*client, ":ircserv 451 " + getDisplayNick(*client)
			+ " :You have not registered");
        return true;
    }
    if (command == "PASS") 
		handlePass(*client, message);
    else if (command == "NICK")
		handleNick(*client, message);
    else if (command == "USER")
		handleUser(*client, message);
    else if (command == "PING")
		handlePing(*client, message);
    else if (command == "CAP")
		return true;
    else if (command == "JOIN")
		handleJoin(*client, message);
    else if (command == "PRIVMSG")
		handlePrivmsg(*client, message);
    else if (command == "PART")
		handlePart(*client, message);
    else if (command == "QUIT")
	{
		handleQuit(*client, message);
		return false;
	}
	else if (command == "TOPIC")
		handleTopic(*client, message);
    else if (command == "KICK")
		handleKick(*client, message);
    else if (command == "MODE")
		handleMode(*client, message);
    else if (command == "INVITE")
		handleInvite(*client, message);
    else
	{
        enqueueMessage(*client, ":ircserv 421 " + getDisplayNick(*client) 
			+ " " + command + " :Unknown command");
	}
	return true;
}

//올바른 Password 입력했는지 확인
void Controller::handlePass(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	
	if (client.getRegistered()){
		enqueueMessage(client, ":ircserv 462 " + client.getNickname()
			+ " :You may not reregister");
		return;
	}
	if (params.empty()){
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client)
			+ " PASS :Not enough parameters");
		return ;
	}
	if (params[0] != _password){
		enqueueMessage(client, ":ircserv 464 * :Password incorrect");
		return ;
	}
	client.setPassOk(true);
	checkAndRegister(client);
}

void Controller::handleNick(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();

	if (params.empty())
	{
		enqueueMessage(client, ":ircserv 431 * :No nickname given");
		return ;
	}
	if (!isValidNickname(params[0]))
	{
		enqueueMessage(client, ":ircserv 432 " + getDisplayNick(client) 
			+ " " + params[0] + " :Erroneous nickname");
		return ;
	}
	if (isNicknameTaken(params[0], client.getFd()))
	{
		enqueueMessage(client, ":ircserv 433 * " + params[0]
			+ " :Nickname is already in use.");
		return;
	}
	std::string oldPrefix = makePrefix(client);
	client.setNickname(params[0]);
	//닉네임변경
	if (client.getRegistered())
	{
		std::string nickMessage = oldPrefix + " NICK :" + client.getNickname();
		enqueueMessage(client, nickMessage);
		broadcastToSharedClients(client.getFd(), nickMessage);
		return ;	
	}
	//최초닉네임등록
	checkAndRegister(client);
}

//USER <username> <hostname> <servername> :<realname>
void Controller::handleUser(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (client.getRegistered())
	{
		enqueueMessage(client, ":ircserv 462 " + client.getNickname()
			+ " :You may not reregister");
		return;
	}
	if (params.size() < 4)
	{
		enqueueMessage(client, ":ircserv 461 "+ getDisplayNick(client)
			+ " USER :Not enough parameters");
		return ;
	}
	client.setUsername(params[0]);
	checkAndRegister(client);
}

void Controller::handlePing(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
		
	if (params.empty()){
		enqueueMessage(client, ":ircserv 409 " + getDisplayNick(client)
			+ " PING :No origin specified");
		return ;
	}
	enqueueMessage(client, ":ircserv PONG :" + params[0]);
}

void Controller::handlePart(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();

	if (params.empty())
	{
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client)
		+ " PART :Not enough parameters");
		return ;
	}
	Channel* channel = findChannel(params[0]);
	if (channel == NULL){
		enqueueMessage(client, ":ircserv 403 " + client.getNickname()
			+ " " + params[0] + " :No such channel");
		return ;
	}
	if (!channel->hasMember(client.getFd())){
		enqueueMessage(client, ":ircserv 442 " + client.getNickname()
			+ " " + params[0] + " :You're not on that channel");
		return ;
	}
	std::string partMessage = makePrefix(client) + " " + "PART " + channel->getName();
	if (params.size() >= 2)
		partMessage += " :" + params[1];
	broadcastToChannel(*channel, partMessage);
	channel->removeClientState(client.getFd());
	removeEmptyChannels();
}

void Controller::handleQuit(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();

	if (!params.empty())
		client.setQuitReason(params[0]);
	else
		client.setQuitReason("Client Quit");
}

void Controller::handleMode(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.empty())
	{
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client)
			+ " MODE :Not enough parameters");
		return ;
	}
	Channel* channel = findChannel(params[0]);
	if (channel == NULL)
	{
		//user_mode 무시
		if (findClientByNickname(params[0]) != NULL)
			return;
		enqueueMessage(client, ":ircserv 403 " + client.getNickname()
			+ " " + params[0] + " :No such channel");
		return ;
	}
	//단순 조회
	if (params.size() == 1)
	{
		std::string modes = "+";
		std::string modeParams;
		if (channel->getMode().inviteOnly)
			modes += "i";
		if (channel->getMode().topicRestricted)
			modes += "t";
		if (channel->getMode().hasKey)
		{
			modes += "k";
			if (channel->hasMember(client.getFd()))
				modeParams += " " + channel->getMode().key;
			else
				modeParams += " *";
		}
		if (channel->getMode().hasUserLimit)
		{
			modes += "l";
			std::ostringstream oss;
			oss << channel->getMode().userLimit;
			modeParams += " " + oss.str();
		}
		enqueueMessage(client, ":ircserv 324 " + client.getNickname()
			+ " " + channel->getName()
			+ " " + modes + modeParams);
		return;
	}
	// "mode #42 :" 같은 경우
	if (params[1].empty())
		return ;
	
	if (!channel->isOperator(client.getFd()))
	{
		enqueueMessage(client, ":ircserv 482 " + client.getNickname() 
			+ " " + channel->getName() + " :You're not channel operator");
		return ;
	}

	const std::string& modeString = params[1];
	char currentSign = '\0';
	char lastBroadcastSign = '\0';
	std::size_t nextValueIdx = 2;
	std::string broadcastModes;
	std::string broadcastValues;

	for (std::size_t i = 0; i < modeString.size(); i++)
	{
		char modeChar = modeString[i];
		// 1. 부호 설정
		if (modeChar == '+' || modeChar == '-')
		{
			currentSign = modeChar;
			continue;
		}
		// 2. 부호 누락 에러처리
		if (currentSign == '\0')
		{
			enqueueMessage(client, ":ircserv 472 " + client.getNickname() + " " + std::string(1, modeChar) + " :is unknown mode char to me");
				continue;	
		}
		bool modeApplied = false;
		bool missingValue = false;
		std::string modeValue;

		// 3. 지원하는 모드 판별 및 파라미터 직접 추출
		switch (modeChar)
		{
			case 'i':
				channel->getMode().inviteOnly = (currentSign == '+');
				modeApplied = true;
				break;
			case 't':
				channel->getMode().topicRestricted = (currentSign == '+');
				modeApplied = true;
				break;
			case 'k':
				if (currentSign == '+')
				{
					if (nextValueIdx >= params.size() || params[nextValueIdx].empty()) 
					{
						missingValue = true;
						break;
					}
					modeValue = params[nextValueIdx++];
					channel->getMode().hasKey = true;
					channel->getMode().key = modeValue; 
				}
				else
				{
					channel->getMode().hasKey = false;
					channel->getMode().key.clear();
				}
				modeApplied = true;
				break;
			case 'l':
			{
				if (currentSign == '+')
				{
					if (nextValueIdx >= params.size() || params[nextValueIdx].empty())
					{
						missingValue = true;
						break;
					}
					modeValue = params[nextValueIdx++];
					char* endptr;
					errno = 0;
					long limit = std::strtol(modeValue.c_str(), &endptr,10);
					if (errno == ERANGE || *endptr != '\0'|| limit <=0)
						break;
					channel->getMode().hasUserLimit = true;
					channel->getMode().userLimit = static_cast<std::size_t>(limit);
				}
				else
				{
					channel->getMode().hasUserLimit = false;
					channel->getMode().userLimit = 0;
				}
				modeApplied = true;
				break;
			}
			case 'o':
			{
				// 💡 o 로직 전체 구현
				if (nextValueIdx >= params.size() || params[nextValueIdx].empty())
				{
					missingValue = true;
					break;
				}
				modeValue = params[nextValueIdx++];

				Client* target = findClientByNickname(modeValue);
				if (target == NULL)
				{
					enqueueMessage(client, ":ircserv 401 " + client.getNickname() 
						+ " " + modeValue + " :No such nick/channel");
					break;
				}
				if (!channel->hasMember(target->getFd()))
				{
					enqueueMessage(client, ":ircserv 441 " + client.getNickname() + " " 
						+ modeValue + " " + channel->getName() + " :They aren't on that channel");
					break;
				}
				if (currentSign == '+')
					channel->addOperator(target->getFd());
				else
				{
					if (!channel->isOperator(target->getFd())) 
						break;
					channel->removeOperator(target->getFd());
				}
				modeApplied = true;
				break;
			}
			default:
				enqueueMessage(client, ":ircserv 472 " + client.getNickname()
					+ " " + std::string(1, modeChar) + " :is unknown mode char to me");
				break;
		}
		// 파라미터 부족 에러 발생 시 루프 즉시 종료
		if (missingValue) 
		{
			enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client) 
				+ " MODE :Not enough parameters");
			break; 
		}
		// 적용실패시 문자열 추가 생략
		if (!modeApplied)
			continue;
		if (lastBroadcastSign != currentSign)
		{
            broadcastModes += currentSign;
            lastBroadcastSign = currentSign;
        }
        broadcastModes += modeChar;
        if (!modeValue.empty()) 
			broadcastValues += " " + modeValue;
	}
	if (broadcastModes.empty())
		return ;
	std::string modeMessage = makePrefix(client) + " MODE " + channel->getName() 
		+ " " + broadcastModes + broadcastValues;
	broadcastToChannel(*channel, modeMessage);
}

void Controller::handleInvite(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.empty() || params.size() < 2)
	{
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client)
			+ " INVITE :Not enough parameters");
		return ;
	}
	Channel * channel = findChannel(params[1]);
	if (channel == NULL)
	{
		enqueueMessage(client, ":ircserv 401 " + client.getNickname()
			+ " " + params[1] + " :No such nick/channel");
		return;
	}
	if (!channel->hasMember(client.getFd()))
	{
		enqueueMessage(client, ":ircserv 442 " + client.getNickname() + " " 
			+ channel->getName() + " :You're not on that channel");
		return ;
	}
	if (channel->getMode().inviteOnly)
	{
		if (!channel->isOperator(client.getFd()))
		{
			enqueueMessage(client, ":ircserv 482 " + client.getNickname() 
				+ " " + channel->getName() + " :You're not channel operator");
			return ;		
		}
	}
	Client* target = findClientByNickname(params[0]);
	if (target == NULL)
	{
		enqueueMessage(client, ":ircserv 401 " + client.getNickname()
			+ " " + params[0] + " :No such nick/channel");
		return;
	}
	if (channel->hasMember(target->getFd()))
	{
		enqueueMessage(client, ":ircserv 443 " + client.getNickname()
			+ " " + params[0] + " " +  params[1] + " :is already on channel");
		return ;
	}
	channel->addInvited(target->getFd());
	std::string inviteMessage = makePrefix(client) + " INVITE " 
		+ params[0] + " " + channel->getName();
	enqueueMessage(*target, inviteMessage);
	enqueueMessage(client, ":ircserv 341 " + client.getNickname()
		+ " " + params[0] + " " + channel->getName());
	return ;
}

void Controller::enqueueMessage(Client& client, const std::string& message)
{
	// Preserve command/parameters; only the final text may be shortened.
	std::string wire = message;
	if (wire.size() > 510)
	{
		const std::size_t trailing = wire.find(" :");
		if (trailing == std::string::npos || trailing + 2 > 510)
			return;
		std::size_t end = 510;
		while (end > trailing + 2
			&& (static_cast<unsigned char>(wire[end]) & 0xc0) == 0x80)
			--end;
		wire.resize(end);
	}
	const std::string framed = wire + "\r\n";

	if (framed.size() > MAX_WRITE_BUFFER
		|| client.getWriteBuffer().size() > MAX_WRITE_BUFFER - framed.size())
	{
		client.markShouldClose();
		return ;
	}
	client.appendWriteBuffer(framed);
}

void Controller::handleKick(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.size() < 2){
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client) + " KICK :Not enough parameters");
		return ;		
	}
	Channel* channel = findChannel(params[0]);
	if (channel == NULL)
	{
		enqueueMessage(client, ":ircserv 403 " + client.getNickname() 
			+ " "+ params[0] + " :No such channel");
		return ;
	}
	if (!channel->isOperator(client.getFd()))
	{
		enqueueMessage(client, ":ircserv 482 " + client.getNickname() 
			+ " " + channel->getName() + " :You're not channel operator");
		return ;
	}
	Client* target = findClientByNickname(params[1]);
	if (target == NULL){
		enqueueMessage(client, ":ircserv 401 " + client.getNickname()
			+ " " + params[1] + " :No such nick/channel");
		return;
	}
	if (!channel->hasMember(target->getFd()))
	{
		enqueueMessage(client, ":ircserv 441 " + client.getNickname() + " " + target->getNickname() 
			+ " " + channel->getName() + " :They aren't on that channel");
		return ;
	}
	std::string kickMessage = makePrefix(client) + " KICK " + channel->getName()
		+ " " + target->getNickname();
	if (params.size() > 2)
		kickMessage += " :" + params[2];		
	broadcastToChannel(*channel, kickMessage);
	channel->removeClientState(target->getFd());
	removeEmptyChannels();
}

void Controller::handleTopic(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.empty()){
		enqueueMessage(client, ":ircserv 461 " + getDisplayNick(client)
			+ " TOPIC :Not enough parameters");
		return ;		
	}
	Channel* channel = findChannel(params[0]);
	if (channel == NULL){
		enqueueMessage(client, ":ircserv 403 " + client.getNickname() + " "+ params[0] + " :No such channel");
		return ;
	}
	if (!channel->hasMember(client.getFd()))
	{
		enqueueMessage(client, ":ircserv 442 " + client.getNickname() + " " 
			+ channel->getName() + " :You're not on that channel");
		return ;
	}
	//TOPIC변경 or clear
	if (params.size() >= 2)
	{
		if ((channel->getMode().topicRestricted) 
			&& (!channel->isOperator(client.getFd())))
		{
			enqueueMessage(client, ":ircserv 482 " + client.getNickname()
				+ " " + channel->getName() + " :You're not channel operator");
			return;
		}
		channel->setTopic(params[1]);
		broadcastToChannel(*channel, makePrefix(client) + " TOPIC " + channel->getName()
			+ " :" + channel->getTopic());
		return ;
	}
	//TOPIC 확인
	if (channel->getTopic().empty())
	{
			enqueueMessage(client, ":ircserv 331 " + client.getNickname()
				+ " " + channel->getName() + " :No topic is set");
	}
	else{
		enqueueMessage(client, ":ircserv 332 " + client.getNickname() 
			+ " " + channel->getName() + " :" + channel->getTopic());
	}
	return ;
}

//채널 찾아서 (없으면 만들어서)JOIN + 한명이면 operator설정
void Controller::handleJoin(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.empty())
	{
		enqueueMessage(client, ":ircserv 461 "+ client.getNickname()
			+ " JOIN :Not enough parameters");
		return ;
	}
	const std::string& channelName = params[0];
	if (!isValidChannelName(channelName))
	{
		enqueueMessage(client, ":ircserv 403 " + client.getNickname()
			+ " " + channelName + " :No such channel");
		return ;		
	}
	Channel* channel = findChannel(channelName);
	if (channel == NULL)
		channel = createChannel(channelName);
	else
	{
		if (channel->hasMember(client.getFd()))
			return ;
		if (channel->getMode().inviteOnly)
		{
			if (!channel->isInvited(client.getFd()))
			{
				enqueueMessage(client, ":ircserv 473 " + client.getNickname() 
					+ " " + channel->getName() + " :Cannot join channel");
				return ;
			}
		}
		if (channel->getMode().hasKey)
		{
			if (params.size() < 2|| channel->getMode().key != params[1])
			{
				enqueueMessage(client, ":ircserv 475 " + client.getNickname() 
					+ " " + channel->getName() + " :Cannot join channel");
				return ;		
			}
		}
		if (channel->getMode().hasUserLimit)
		{
			if (channel->getMode().userLimit <= channel->getMemberCount())
			{
				enqueueMessage(client, ":ircserv 471 " + client.getNickname() 
					+ " " + channel->getName() + " :Cannot join channel");
				return ;
			}
		}
	}
	bool isFirstMember = channel->getMemberCount() == 0;
	channel->addMember(client.getFd());
	channel->removeInvited(client.getFd());
	if (isFirstMember)
		channel->addOperator(client.getFd());
	broadcastToChannel(*channel, makePrefix(client) + " JOIN " + channel->getName());
	const std::string namesPrefix = ":ircserv 353 " + client.getNickname()
		+ " = " + channel->getName() + " :";
	std::string names;
	const std::vector<int>& members = channel->getMembersFd();
	for (std::size_t i = 0; i < members.size(); i++)
	{
		Client* member = findClientByFd(members[i]);
		if (member == NULL)
			continue;
		std::string entry;
		if (channel->isOperator(members[i]))
			entry = "@";
		entry += member->getNickname();
		if (!names.empty()
			&& namesPrefix.size() + names.size() + 1 + entry.size() > 510)
		{
			enqueueMessage(client, namesPrefix + names);
			names.clear();
		}
		if (!names.empty())
			names += " ";
		names += entry;
	}
	if (!names.empty())
		enqueueMessage(client, namesPrefix + names);
	enqueueMessage(client, ":ircserv 366 " + client.getNickname()
		+ " " + channel->getName() + " :End of /NAMES list");
	if (!channel->getTopic().empty())
	{
		enqueueMessage(client, ":ircserv 332 " + client.getNickname()
		+ " " + channel->getName() + " :" + channel->getTopic());
	}
}

void Controller::handlePrivmsg(Client& client, const Message& message)
{
	const std::vector<std::string>& params = message.getParams();
	if (params.empty())
	{
		enqueueMessage(client, ":ircserv 411 " + client.getNickname()
        	+ " :No recipient given (PRIVMSG)");
    	return;
	}
	else if (params.size() < 2 || params[1].empty())
	{
		enqueueMessage(client, ":ircserv 412 " + client.getNickname()
        	+ " :No text to send");
		return ;
	}

	//1. params[0]이 채널이면 기존 채널 PRIVMSG
	//2. params[0]이 nick이면 Private DM
	Channel* channel = findChannel(params[0]);
	if (channel != NULL)
	{
		if (!channel->hasMember(client.getFd()))
		{
			enqueueMessage(client, ":ircserv 442 " + client.getNickname()
				+ " " + params[0] + " :You're not on that channel");
			return ;
		}
		const std::vector<int>& memberFds = channel->getMembersFd();
		for (size_t i = 0; i < memberFds.size(); i++)
		{
			if (memberFds[i] == client.getFd())
				continue;
			Client* member = findClientByFd(memberFds[i]);
			if (member != NULL)
			{
				enqueueMessage(*member, makePrefix(client) + " PRIVMSG " 
					+ channel->getName() + " :" + params[1]);
			}
		}
		return ;
	}
	Client* target = findClientByNickname(params[0]);
	if (target == NULL)
	{
		enqueueMessage(client, ":ircserv 401 " + client.getNickname()
			+ " " + params[0] + " :No such nick/channel");
		return ;
	}
	enqueueMessage(*target, makePrefix(client) + " PRIVMSG " 
		+ target->getNickname() + " :" + params[1]);
}

void Controller::broadcastToChannel(Channel& channel, const std::string& message, int excludeFd)
{
	const std::vector<int>& membersFd = channel.getMembersFd();
	for (std::size_t i = 0; i < membersFd.size(); i++)
	{
		if (membersFd[i] == excludeFd)
			continue;
		Client* member = findClientByFd(membersFd[i]);
		if (member != NULL)
			enqueueMessage(*member, message);
	}
}

//중복 broadcast막기 위해 set 사용
void Controller::broadcastToSharedClients(int clientFd, const std::string& message)
{
	std::set<int> recipients;

	for(std::size_t i = 0; i < _channels.size(); i++)
	{
		if (!_channels[i].hasMember(clientFd))
			continue;
		const std::vector<int>& members = _channels[i].getMembersFd();
		recipients.insert(members.begin(), members.end());
	}
	recipients.erase(clientFd);

	for(std::set<int>::const_iterator it = recipients.begin();
		it != recipients.end(); it++)
	{
		Client* target = findClientByFd(*it);
		if (target != NULL)
			enqueueMessage(*target, message);
	}
}

std::string Controller::getDisplayNick(const Client& client) const
{
	if (client.getNickname().empty())
		return "*";
	return client.getNickname();
}

bool Controller::canUseBeforeRegistration(const std::string& command) const
{
	return command == "PASS"
		|| command == "NICK"
		|| command == "USER"
		|| command == "CAP"
		|| command == "PING"
		|| command == "QUIT";
}
