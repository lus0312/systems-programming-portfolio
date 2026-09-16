#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <vector>
# include <map>
# include <poll.h>
# include "Client.hpp"
# include "Message.hpp"
# include "Channel.hpp"
# include "Controller.hpp"

class Server
{
private:
	int							_port;
	int							_serverFd;
	std::vector<struct pollfd>	_pollFds; //poll로 감시할 fd목록
	Controller					_controller; //password + clients + channels

	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);

	bool setupSocket();
	bool acceptClient();
	bool setNonBlocking(int fd);
	bool runLoop();
	bool handlePollEvent(std::size_t index);
	bool handleClientRead(std::size_t index);
	bool handleClientWrite(std::size_t index);
	void removeClientByFd(int fd);
	void removeMarkedClients();
	bool processClientBuffer(Client& client);
	void syncPollOutFlags();
	
public:
	Server(int port, const std::string& password);
	~Server();
	bool run();
	int getPort() const;
	const std::string& getPassword() const;
};

#endif
