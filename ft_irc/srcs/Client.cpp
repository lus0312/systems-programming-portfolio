#include "Client.hpp"

Client::Client()
	: _fd(-1),
	  _passOk(false),
	  _registered(false),
	  _shouldClose(false),
	  _quitReason("Connection closed")
{	
}

Client::Client(int fd)
	: _fd(fd),
	  _passOk(false),
	  _registered(false),
	  _shouldClose(false),
	  _quitReason("Connection closed")
{
}

Client::Client(const Client& other)
	: _fd(other._fd),
	  _passOk(other._passOk),
	  _registered(other._registered),
	  _shouldClose(other._shouldClose),
	  _quitReason(other._quitReason),
	  _buffer(other._buffer),
	  _writeBuffer(other._writeBuffer),
	  _username(other._username),
	  _nickname(other._nickname)
{
}

Client& Client::operator=(const Client& other)
{
	if (this != &other){
		_fd = other._fd;
		_passOk = other._passOk;
		_registered = other._registered;
		_shouldClose = other._shouldClose;
		_quitReason = other._quitReason;
		_buffer = other._buffer;
		_writeBuffer = other._writeBuffer;
		_username = other._username;
		_nickname = other._nickname;
	}
	return *this;
}

Client::~Client()
{
}

int Client::getFd() const
{
	return _fd;
}

const std::string& Client::getBuffer() const
{
	return _buffer;
}

void Client::appendBuffer(const std::string& data)
{
	_buffer += data;
}

void Client::eraseBuffer(std::size_t len)
{
	_buffer.erase(0, len);
}

void Client::clearBuffer(){
	_buffer.clear();
}

bool Client::shouldClose() const
{
	return _shouldClose;
}

void Client::markShouldClose()
{
	_shouldClose = true;
}

const std::string& Client::getWriteBuffer() const
{
	return _writeBuffer;
}
void Client::appendWriteBuffer(const std::string& data)
{
	_writeBuffer += data;
}
void Client::eraseWriteBuffer(std::size_t len)
{
	_writeBuffer.erase(0, len);
}
bool Client::hasWriteData() const
{
	return !_writeBuffer.empty();
}

bool Client::getPassOk() const
{
	return _passOk;
}
bool Client::getRegistered() const
{
	return _registered;
}
void Client::setPassOk(bool value)
{
	_passOk = value;
}
void Client::setRegistered(bool value)
{
	_registered = value;
}
const std::string& Client::getUsername() const
{
	return _username;
}
const std::string& Client::getNickname() const
{
	return _nickname;
}
void Client::setUsername(const std::string& username)
{
	_username = username;
}
void Client::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

const std::string& Client::getQuitReason() const
{
	return _quitReason;
}

void Client::setQuitReason(const std::string& reason)
{
	_quitReason = reason;
}
