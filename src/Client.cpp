#include "../includes/Client.hpp"

namespace ft
{
Client::Client()
{

}

Client::~Client()
{

}

struct sockaddr_storage	&Client::getAddr()
{
	return (_addr);
}

const int	&Client::getSockfd() const
{
	return (_sockfd);
}

void	Client::setSockfd(int fd)
{
	_sockfd = fd;
}

const int	&Client::getPort() const
{
	return (_port);
}

void	Client::setPort(int port)
{
	_port = port;
}

const int	&Client::getServSock() const
{
	return (_servSock);
}

void	Client::setServSock(int servSock)
{
	_servSock = servSock;
}

int	Client::sendTo(std::string msg)
{
	int ret;

	if (msg.size() > 0)
	{
		ret = send(_sockfd, msg.c_str(), msg.size(), 0);
		if (ret == -1)
			std::cout << "error send" << std::endl;
		return (ret);
	}
	return (0);
}

int	Client::recvFrom(std::string *ret)
{
	char	buf[1000];
	int		recb = recv(_sockfd, buf, 999, 0);

	if (recb == -1)
		return (-1);
	*ret = std::string(buf, recb);
	return (0);
}
};