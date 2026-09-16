#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>	//socket, setsockopt, bind, listen, send
#include <netinet/in.h>	//sockaddr_in, htons, INADDR_ANY
#include <unistd.h>		//close
#include <cstring>		//std::memset
#include <poll.h>		//poll
#include "Message.hpp"
#include "Controller.hpp"
#include <sstream>		//istringstream
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <cctype>

extern volatile sig_atomic_t g_running;

Server::Server(int port, const std::string& password)
	: _port(port),
	  _serverFd(-1),
	  _controller(password)
{
}

Server::~Server()
{
	for (std::size_t i = 1; i < _pollFds.size(); i++)
		close(_pollFds[i].fd);
	if (_serverFd != -1)
		close (_serverFd);
}

//buffer에서 \r\n 단위로 짤라서 line에 넣기
bool Server::processClientBuffer(Client& client)
{
	std::string::size_type pos;
	pos = client.getBuffer().find("\r\n");
	while(pos != std::string::npos)
	{
		// 1. 메시지 길이가 RFC 규격(512바이트)을 초과하면 연결 종료 (또는 무시)
		if (pos > 510)
			return false;
		std::string line = client.getBuffer().substr(0, pos);
		Message message;
		if ((message.parse(line)))
		{
			if(!_controller.processMessage(client.getFd(), message))
				return false;
		}
		client.eraseBuffer(pos+2);
		pos = client.getBuffer().find("\r\n");
	}

	if (client.getBuffer().size() > 512)
		return false;
	return true;
}

void Server::removeClientByFd(int fd)
{
	//논리적 데이터 지우기 (컨트롤러 담당)
	_controller.removeClient(fd);
	//물리적 데이터 지우기 (서버 담당)
	for (std::size_t i = 0; i< _pollFds.size(); i++)
	{
		if (fd == _pollFds[i].fd)
		{	_pollFds.erase(_pollFds.begin() + i); 
			break;
		}
	}
	close(fd);
    std::cout << "Client disconnected: fd " << fd << std::endl;
}

void Server::removeMarkedClients()
{
	std::vector<int> fds;

	_controller.getClientsToClose(fds);

	for (std::size_t i = 0; i < fds.size(); ++i)
	{
		if (_controller.findClientByFd(fds[i]) != NULL)
			removeClientByFd(fds[i]);
	}
}

bool Server::handleClientRead(std::size_t index)
{
	//운영체제(OS)의 가상 메모리 최소 단위인 페이지 크기(Page Size)와 일치하는
	//4096바이트(4KB)를 버퍼 크기로 가장 많이 사용
	char	buffer[4096];

	int fd = _pollFds[index].fd;
	Client* client = _controller.findClientByFd(fd);
	if (client == NULL)
		return false;
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
	// 정상종료 혹은 에러
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			return true;
		removeClientByFd(fd);
		return true;
	}
	if (bytes == 0)
	{
		removeClientByFd(fd);
		return true;
	}

	client->appendBuffer(std::string(buffer, static_cast<std::size_t>(bytes)));
	if (!processClientBuffer(*client))
	{
		removeClientByFd(fd);
		return true;
	}
	return true;
}

bool Server::handleClientWrite(std::size_t index)
{
	int fd = _pollFds[index].fd;
	Client* client = _controller.findClientByFd(fd);
	if (client == NULL)
		return false;
	if (!client->hasWriteData())
	{
		_pollFds[index].events &= ~POLLOUT;
		return true;
	}
	const std::string& buffer = client->getWriteBuffer();
	ssize_t bytes = send(fd, buffer.c_str(), buffer.size(), 0);
	if (bytes > 0)
	{
		client->eraseWriteBuffer(static_cast<std::size_t>(bytes));
		if (!client->hasWriteData())
			_pollFds[index].events &= ~POLLOUT;
		return true;
	}
// Do not retry send() here.
// The fd was already reported writable by poll(), but a nonblocking send()
// may still fail with EAGAIN/EWOULDBLOCK if the socket buffer is full.
// Keep the write buffer unchanged and wait for the next POLLOUT.
	if (bytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		return true;
	removeClientByFd(fd);
	return true;
}

bool Server::handlePollEvent(std::size_t index)
{
	int fd = _pollFds[index].fd;
	short revents = _pollFds[index].revents;

	if (revents & (POLLERR | POLLHUP | POLLNVAL))
	{
		if (fd == _serverFd)
		{
			if (revents & (POLLERR | POLLNVAL))
				std::cerr << "Error: server socket error" << std::endl;
			return false;
		}
		removeClientByFd(fd);
		return true;
	}
	if (revents & POLLOUT)
	{
		if (!handleClientWrite(index))
			return false;
		
		if (_controller.findClientByFd(fd) == NULL)
			return true;
	}
	if (revents & POLLIN)
	{
		if (fd == _serverFd)
			return acceptClient();
		return handleClientRead(index);
	}
	return true;
}

void Server::syncPollOutFlags()
{
    for (std::size_t i = 1; i < _pollFds.size(); ++i)
    {
        Client* client = _controller.findClientByFd(_pollFds[i].fd);
        if (client != NULL && !client->getWriteBuffer().empty()) 
            _pollFds[i].events |= POLLOUT;
        else
            _pollFds[i].events &= ~POLLOUT;
    }
}


bool Server::runLoop(){
	int 		ready;
	int			index;

	syncPollOutFlags();
	// 등록해둔 fd목록을 커널이 검사(이벤트 발생할 때까지 계속 대기) -> 이벤트 발생시 각 fd에 실제로 발생한 이벤트를 revents에 표시함
	ready = poll(&_pollFds[0], _pollFds.size(), -1);
	if (ready == -1){
		if (errno == EINTR)
			return true;
		std::cerr<< "Error: poll failed" <<std::endl;
		return false;
	}
	//삭제가 일어나면 i가 밀리니까 뒤에서부터 순회
	index = static_cast<int>(_pollFds.size() -1);
	while(index >= 0)
	{
		if (_pollFds[index].revents != 0)
		{
			if (!handlePollEvent(index))
				return false;
		}
		index--;
	}
	removeMarkedClients();
	return true;
}

bool Server::run(){
	if (!setupSocket())
		return false;
	std::cout<<"Server is listening on port "<<_port<<std::endl;
	while (g_running){
		if (!runLoop())
			return false;
	}
	return true;
}

int Server::getPort() const
{
	return _port;
}

//파일 상태 플래그를 논블로킹으로 설정
bool Server::setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error: fcntl failed" << std::endl;
		return false;
	}
	return true;
}

bool Server::setupSocket()
{
	//어떤 종류의 소켓을 만들지 결정(IPv4 TCP 소켓 fd 생성)-> 소켓 만들어서 서버 fd 저장
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1){
		std::cerr<< "Error: socket failed" <<std::endl;
		return false;
	}
	//서버fd를 논블로킹으로 설정 
	if (!setNonBlocking(_serverFd))
	{
		close(_serverFd);
		_serverFd = -1;
		return false;
	}
	int opt = 1;
	// 소켓 옵션 설정: 주소/포트 재사용 옵션 키기
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cerr<< "Error: setsockopt failed" <<std::endl;
		close(_serverFd);
		_serverFd = -1;
		return false;
	}
	// IP/port 정보 준비: bind()에게 서버 소켓을 어떤 IP주소에서 몇번 port로 열지 정보 제공
	//address : IPv4 주소 체계를 저장할 수 있는 전용 구조체 변수
	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	//주소 체계(Address Family)를 IPv4인터넷 체계로 지정
	address.sin_family = AF_INET;
	//서버의 IP 주소를 INADDR_ANY로 설정
	address.sin_addr.s_addr = INADDR_ANY;
	//호스트 바이트 순서로 된 포트 번호를 네트워크 바이트 순서로 변환
	address.sin_port = htons(_port);
	// _serverFd를 해당 IP/port에 묶음: 주소정보를 서버 소켓에 할당
	if (bind(_serverFd, reinterpret_cast<const struct sockaddr *>(&address), sizeof(address)) == -1)
	{
		std::cerr<< "Error: bind failed" <<std::endl;
		close(_serverFd);
		_serverFd = -1;
		return false;
	}
	// port에 묶인 소켓을 클라이언트 연결 요청을 기다리는 상태로 만들기
	if (listen(_serverFd, SOMAXCONN) == -1)
	{
		std::cerr<< "Error: listen failed" <<std::endl;
		close(_serverFd);
		_serverFd = -1;
		return false;		
	}
	//server fd를 poll 목록에 추가
	struct pollfd serverPollFd;

	serverPollFd.fd = _serverFd;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	_pollFds.push_back(serverPollFd);
	return true;
}

bool Server::acceptClient()
{
	int clientFd;
	// listen() 상태의 서버 소켓에서 대기중인 클라이언트 요청 꺼내기
	clientFd = accept(_serverFd, NULL, NULL);
	if (clientFd == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			return true;
		std::cerr<< "Error: accept failed"<<std::endl;
		return false;
	}
	if (!setNonBlocking(clientFd))
	{
		close(clientFd);
		return false;
	}
	//물리적 네트워크 설정 (poll 감시 목록에 추가)
	struct pollfd clientPollFd;
	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	_pollFds.push_back(clientPollFd);
	//논리적 데이터 생성 (Controller에게 위임)
    _controller.addClient(clientFd);
    std::cout << "New client connected: fd " << clientFd << std::endl;
	return true;
}



