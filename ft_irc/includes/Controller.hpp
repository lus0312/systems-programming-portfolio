#ifndef Controller_HPP
# define Controller_HPP
# include <map>
# include <vector>
# include <string>
# include "Client.hpp"
# include "Channel.hpp"
# include "Message.hpp"

class Controller
{
private:
	std::map<int, Client> _clients;
	std::vector<Channel> _channels;
	std::string _password; 

	// OCF 복사 금지
	Controller();
	Controller(const Controller& other);
	Controller& operator=(const Controller& other);
public:
	Controller(const std::string& password);
	~Controller();

	// Client 데이터 관리
	void addClient(int fd);
	void removeClient(int fd); // 기존 removeClientByFd 역할 (단, close(fd)는 안 함)
	Client* findClientByFd(int fd);
	Client* findClientByNickname(const std::string& nickname);
	bool isNicknameTaken(const std::string& nickname, int excludeFd) const;
	bool isValidNickname(const std::string& nickname) const;
	void getClientsToClose(std::vector<int>& fds) const;

	// Channel 데이터 관리
	Channel* findChannel(const std::string& name);
	Channel* createChannel(const std::string& name);
	bool isValidChannelName(const std::string& name) const;
	void removeClientFromAllChannels(int fd);
	void removeEmptyChannels();
	
	void sendWelcome(Client& client);
	void checkAndRegister(Client& client);
	std::string makePrefix(const Client& client) const;
	std::string getDisplayNick(const Client& client) const;
	void enqueueMessage(Client& client, const std::string& message);
	void broadcastToChannel(Channel& channel, const std::string& message, int excludeFd = -1);
	void broadcastToSharedClients(int clientFd, const std::string& message);

	bool processMessage(int clientFd, const Message& message);
	bool canUseBeforeRegistration(const std::string& command) const;
	bool handleCommand(Client& client, const Message& message);
	void handlePass(Client& client, const Message& message);
	void handleNick(Client& client, const Message& message);
	void handleUser(Client& client, const Message& message);
	void handlePing(Client& client, const Message& message);
	void handleJoin(Client& client, const Message& message);
	void handlePrivmsg(Client& client, const Message& message);
	void handlePart(Client& client, const Message& message);
	void handleQuit(Client& client, const Message& message);
	void handleTopic(Client& client, const Message& message);
	void handleKick(Client& client, const Message& message);
	void handleMode(Client& client, const Message& message);
	void handleInvite(Client& client, const Message& message);
};

#endif
