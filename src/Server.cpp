#include "../includes/Server.hpp"

namespace ft
{
//	ServerBlock start

ServerBlock::ServerBlock(std::vector<int> &ports, std::map<int, int> *portSocks)
{
	struct sockaddr_in	saddr;
	int	yes = 1;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		if ((*portSocks).find(*it) == (*portSocks).end())
		{
			saddr.sin_port = htons(*it);
			if (((*portSocks)[*it] = socket(PF_INET ,SOCK_STREAM, 0)) == -1)
				throw std::runtime_error("Failed to create socket.");
			if (setsockopt((*portSocks)[*it], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0)
				throw std::runtime_error("Failed to change socket option.");
			if (bind((*portSocks)[*it], (struct sockaddr *)&saddr, sizeof saddr) == -1)
				throw std::runtime_error("Failed to bind socket.");
			startListening((*portSocks)[*it]);
			std::cout << "\033[1;32mServer is now listening on Port: " << *it << "\033[0m" << std::endl;
		}
	}
}

ServerBlock::~ServerBlock()
{
}

void	ServerBlock::startListening(int sockfd)
{
	if (listen(sockfd, 10) == -1)
		throw std::runtime_error("Failed to start listening.");
}

//	Server start

Server::Server(ft::Config &conf) : _config(conf)
{
	_nmbpollfds = 0;
	std::vector<int>	test;
	test.push_back(8000);
	test.push_back(8001);

	// while (parsing)
	// {

		ServerBlock	testblock(test, &_portSocks);
		//	weitere config parsen und in block speichern
		testblock._name.push_back("localhost");
		testblock._name.push_back("www.localhost");
		for (std::vector<int>::iterator vit = test.begin(); vit != test.end(); vit++)
		{
			_blocks.insert(std::make_pair<int, ServerBlock*>(*vit, &testblock)); 
		}
	// }

	for (PSiterator it = getPortSockIt(); it != getPortSockEnd(); it++)
	{
		expandPollfds(it->second);
	}
	
	/* parsing loop:
	parsed bis server block
	{
		liest alle ports speichert in vector  (z.B. "ports")
		macht ein ServerBlock element, übergibt ports vector und &portsSock (std::map<int, int>, enthält port nummer/sockfd pair)
		liest weitere einstellungen und übergibt sie an den neuen ServerBlock (name, cgi, server_name)

		while (ports[i])
			_blocks[portsSock[ports[i]]] = neuer ServerBlock
			i++
	}
	expandPollfds() für jeden port, sockfds nimmt man aus der portsSock map
	*/
}

Server::~Server()
{
	/*
	**	free clientList
	*/
}

const int	&Server::getNmbPollfds() const
{
	return (this->_nmbpollfds);
}

struct pollfd *Server::getPollfds()
{
	return (&_pollfds.front());
}

std::map<int, Client>	&Server::getClLst()
{
	return (this->_clients);
}

void	Server::expandPollfds(int fd)
{
	struct pollfd nPollfd;
	nPollfd.fd = fd;
	nPollfd.events = POLLIN;
	_pollfds.push_back(nPollfd);
	_nmbpollfds++;
}

void	Server::deletePollfd(struct pollfd &pfd)
{
	if (_nmbpollfds == 0)
		throw(std::runtime_error("Deleting fds out of empty fd array"));

	for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd == pfd.fd)
		{
			_pollfds.erase(it);
			break ;
		}
	}
	_nmbpollfds--;
}

void	Server::setClLst(std::map<int, Client> ClLst)
{
	_clients = ClLst;
}

std::map<int, ft::ServerBlock>	Server::parsing(std::string confFile)
{
	std::map<int, ft::ServerBlock> blocks;
	if (!check_confFile(confFile))
		throw std::runtime_error("invalid config_file");
	return (blocks);
}

bool	Server::check_confFile(std::string confFile)
{
	(void)confFile;
	return (true);
}

bool	Server::acceptClient(int sockfd)
{
	Client nClient;
	socklen_t sin_len = sizeof(nClient.getAddr());
	nClient.setSockfd(accept(sockfd, (struct sockaddr *)(&nClient.getAddr()), &sin_len));
	if (nClient.getSockfd() == -1)
		throw std::runtime_error("Failed to accept connection.");
	nClient.setPort(findPort(sockfd));
	nClient.setServSock(sockfd);
	_clients[nClient.getSockfd()] = nClient;
	expandPollfds(nClient.getSockfd());
	fcntl(nClient.getSockfd(), F_SETFL, O_NONBLOCK);
	std::cout << "\033[1;32m	Client connected onto port: " << findPort(sockfd) << " (client fd: " << nClient.getSockfd() << ")\033[0m" << std::endl;
	return (true);
}

int	Server::findPort(int sockfd)
{
	
	for (PSiterator it = getPortSockIt(); it != getPortSockEnd(); it++)
	{
		if (it->second == sockfd)
			return (it->first);
	}
	return (-1);
}

bool	Server::checkForNewCon(int index)
{
	for (ft::Server::PSiterator it = getPortSockIt(); it != getPortSockEnd(); it++)
	{
		if (getPollfds()[index].fd == it->second)
			return (acceptClient(it->second));
	}
	return (false);
}

Server::PSiterator	Server::getPortSockIt()
{
	return (_portSocks.begin());
}

Server::PSiterator	Server::getPortSockEnd()
{
	return (_portSocks.end());
}
};