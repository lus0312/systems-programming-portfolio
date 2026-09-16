#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <cstddef>

class Client
{
private:
	int _fd;
	bool		_passOk;
	bool		_registered;
	bool		_shouldClose;
	std::string _quitReason;
	std::string _buffer;
	std::string _writeBuffer;
	std::string _username;
	std::string _nickname;

	
public:
	Client();
	Client(int fd);
	Client(const Client& other);
	~Client();
	Client& operator=(const Client& other);

	int		getFd() const;
	const	std::string& getBuffer() const;
	void	appendBuffer(const std::string& data);
	void	eraseBuffer(std::size_t len);
	void	clearBuffer();
	bool	shouldClose() const;
	void	markShouldClose();
	const std::string& getQuitReason() const;
	void setQuitReason(const std::string& reason);

	const	std::string& getWriteBuffer() const;
	void	appendWriteBuffer(const std::string& data);
	void	eraseWriteBuffer(std::size_t len);
	bool	hasWriteData() const;

	bool	getPassOk() const;
	bool	getRegistered() const;
	void	setPassOk(bool value);
	void	setRegistered(bool value);

	const	std::string& getUsername() const;
	const	std::string& getNickname() const;
	void	setUsername(const std::string& username);
	void	setNickname(const std::string& nickname);
};

#endif
