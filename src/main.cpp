#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Config.hpp"
#include "../includes/Response.hpp"
#include <fstream>
#include <unistd.h>

int main(int argc, char **argv)
{
	std::string	confPath;
	if (argc == 1)
		confPath = "config/default.config";
	else if (argc == 2)
		confPath = argv[1];
	else
	{
		std::cout << "Please enter the path to only ONE config file." << std::endl;
		return (1);
	}
	try
	{
		ft::Config	conf(confPath);
		ft::Server	server(conf);

		for (;;)
		{
			// for (int o = 0; o < server.getNmbPollfds(); o++)
			// 	std::cout << "before fd: " << server.getPollfds()[o].fd << "	revent: " << server.getPollfds()[o].revents << std::endl;
			int	npoll = poll(server.getPollfds(), server.getNmbPollfds(), -1);
			if (npoll == -1)
				std::cout << "error: poll" << std::endl;
			// std::cout << std::endl << "npoll: " << npoll << std::endl;
			// for (int o = 0; o < server.getNmbPollfds(); o++)
			// 	std::cout << "after fd: " << server.getPollfds()[o].fd << "	revent: " << server.getPollfds()[o].revents << std::endl;
			for (int x = 0; x < server.getNmbPollfds(); x++)
			{
				if (server.getPollfds()[x].revents & POLLIN)
				{
					// std::cout << server.getPollfds()[x].fd << "	" << server.getPollfds()[x].revents << std::endl;
					if (server.getPollfds()[x].revents & POLLHUP)
					{
						ft::Client	&tmp = server.getClLst().find(server.getPollfds()[x].fd)->second;
						std::cout << "\033[1;31mClient on port " << server.findPort(tmp.getServSock()) << " has hung up (client fd: " << tmp.getSockfd() << ")\033[0m" << std::endl;
						close(server.getPollfds()[x].fd);
						server.getClLst().erase(server.getPollfds()[x].fd);
						server.deletePollfd(server.getPollfds()[x]);
					}
					else
					{
						bool	isServer = server.checkForNewCon(x);
						if (isServer == false)
						{
							ft::Client		&tmp = server.getClLst().find(server.getPollfds()[x].fd)->second;
							ft::Response	resp(tmp, server.getBlocks());
							// std::cout << "fd: " << tmp.getSockfd() << "	tmp revent: " << server.getPollfds()[x].revents << std::endl;
							// std::cout << "revent: " << server.getPollfds()[x].revents << std::endl;
							
						}
					}
				}
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
