#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
// # include <netdb.h>
# include <map>
# include <exception>
# include <arpa/inet.h>
# include <poll.h>
# include <fcntl.h>
# include <vector>
# include "Client.hpp"
# include "Config.hpp"

namespace ft
{
	class ServerBlock
	{
		friend class	Server;

		private:
			std::map<std::string, std::string>	_conf; // maybe die config anders speichern, schon in einzelnen variablen wie mit _name
			std::vector<std::string>			_name;

		public:
			ServerBlock(std::vector<int> &ports, std::map<int, int> *portSocks);
			ServerBlock();
			~ServerBlock();

			void						startListening(int sockfd);
			std::vector<std::string>	&getName();
	};

	class Server
	{
		public:
			typedef	std::multimap<int, ft::ServerBlock*>	blockmap;
			typedef	std::map<int, int>						intmap;
			typedef	intmap::iterator						PSiterator;
			typedef	std::map<int, Client>					clientmap;

		private:
			ft::Config					_config;
			blockmap					_blocks;
			intmap						_portSocks;
			int							_nmbpollfds;
			clientmap					_clients;
			std::vector<struct pollfd>	_pollfds;

		public:
			Server(ft::Config &conf);
			~Server();

			clientmap						&getClLst();
			const int						&getNmbUser() const;
			const int						&getNmbPollfds() const;
			struct pollfd					*getPollfds();
			PSiterator						getPortSockIt();
			PSiterator						getPortSockEnd();
			blockmap						&getBlocks();

			blockmap						parsing(std::string confFile);
			bool							check_confFile(std::string confFile);
			void							expandPollfds(int fd);
			void							deletePollfd(struct pollfd &pfd);
			void							setClLst(std::map<int, Client> ClLst);
			bool							acceptClient(int sockfd);
			int								findPort(int sockfd);
			bool							checkForNewCon(int index);
	};
};

#endif