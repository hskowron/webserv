#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Config.hpp"
#include <unistd.h>
#include <fstream>

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
		ft::Config						config(confPath);
		ft::Server						server(config);
		std::string						buf;

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
							ft::Client	&tmp = server.getClLst().find(server.getPollfds()[x].fd)->second;
							// std::cout << "fd: " << tmp.getSockfd() << "	tmp revent: " << server.getPollfds()[x].revents << std::endl;
							// std::cout << "revent: " << server.getPollfds()[x].revents << std::endl;
							if (tmp.recvFrom(&buf) == -1)
								std::cout << "error: recv from client fd: " << tmp.getSockfd() << "  port: " << tmp.getPort() << std::endl;
							
							std::cout << "\033[33m		Receiving data from client on port: " << tmp.getPort() << " (client fd: " << tmp.getSockfd() << ")\033[0m" << std::endl;
							// std::cout << "buf size: " << buf.size() << std::endl << "buf content:" << std::endl << buf << std::endl << "content end" << std::endl;
							if (!buf.empty() && buf.find("GET / HTTP/1.1") != std::string::npos)
							{
								std::cout << "\033[33m		Sending response to client on port: " << tmp.getPort() << " (client fd: " << tmp.getSockfd() << ")\033[0m" << std::endl;
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("HTTP/1.1 200 OK\r\n");
								char arg[] = {"index.php"};
								char arg2[] = {"php-cgi"};
								char earg6[] = "";
								std::string teststr = "CONNECTIONS=" + std::to_string(server.getClLst().size());
								std::strcpy(earg6, teststr.c_str());
								char *exearg[3];
								exearg[0] = arg2;
								exearg[1] = arg;
								exearg[2] = NULL;
								char *_env[7];
								char earg1[] = "REQUEST_METHOD=GET";
								_env[0] = earg1;
								char earg2[] = "SERVER_PROTOCOL=HTTP/1.1";
								_env[1] = earg2;
								char earg3[] = "PATH_INFO=www/index.php";
								_env[2] = earg3;
								char earg4[] = "REDIRECT_STATUS=200";
								_env[3] = earg4;
								char earg5[] = "SCRIPT_FILENAME=www/index.php";
								_env[4] = earg5;
								_env[5] = earg6;
								_env[6] = NULL;

								int p[2];
								pipe(p);

								int pid = fork();
								if (pid == 0)
								{
									close(p[0]);
									dup2(p[1], STDOUT_FILENO);
									close(p[1]);
									if (execve("cgi-bin/php-cgi", exearg, _env) != 0)
									{
										perror("failed: ");
										exit(1);
									}
									exit(1);
								}
								else
								{
									close(p[1]);
									waitpid(pid, NULL, 0);
								}
								char buf2[1000];
								read(p[0], buf2, 999);
								close(p[0]);
								std::string text(buf2);
								std::string subtext(text.substr(text.find_first_of("<")));

								// std::cout << "html:" << std::endl << text << std::endl << "html end" << std::endl;

								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("Content-Length: " + std::to_string(subtext.size()) + "\r\n");
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo(text);
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("\r\n");
							}
							else if (!buf.empty() && buf.find("GET /favicon.ico HTTP/1.1") != std::string::npos)
							{
								std::cout << "\033[33m		Sending response favicon to client on port: " << tmp.getPort() << " (client fd: " << tmp.getSockfd() << ")\033[0m" << std::endl;
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("HTTP/1.1 404 Not Found\r\n");
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("Content-Length: 0\r\n");
								server.getClLst().find(server.getPollfds()[x].fd)->second.sendTo("\r\n");
							}
						}
					}
				}
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
