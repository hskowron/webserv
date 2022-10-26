#include "../includes/Response.hpp"

namespace ft
{
Response::Response(ft::Client const &client, ft::Server::blockmap const &blocks) : _client(client)
{
	_http = "HTTP/1.1";
	std::string	buf;
	initCode();
	if (_client.recvFrom(&buf) == -1)
		std::cout << "error: recv from client fd: " << _client.getSockfd() << "  port: " << _client.getPort() << std::endl;
	_request.parseReq(buf);
	std::cout << "\033[33m		Receiving data from client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
	_block = findBlock(blocks);
	setCode(cgi());
	createHeader();
	respond();
}

Response::~Response()
{

}

void	Response::setCode(std::string const &code)
{
	_resCode = code;
	CodeCont::iterator	it = _codeList.find(code);
	if (it == _codeList.end())
		throw (std::runtime_error("error: Bad status code"));
	_resString = it->second;
}

void	Response::initCode()
{
	_codeList["200"] = "OK\r\n";
	_codeList["404"] = "Not found\r\n";
	_codeList["500"] = "Internal Server Error\r\n";
}

std::string	Response::cgi()
{
	// static int x = 1;
	

	
	// std::cout << "buf size: " << buf.size() << std::endl << "buf content:" << std::endl << buf << std::endl << "content end" << std::endl;

	// std::cout << "request: " << std::endl;
	// for (ft::Request::iterator it = _request.getRequest().begin(); it != _request.getRequest().end(); it++)
	// 	std::cout << it->first << " = " << it->second << std::endl;
	// std::cout << _request.getBody() << std::endl;
	
		char arg[] = {"index.php"};
		char arg2[] = {"php-cgi-fail"};
		char earg6[] = "CONNECTIONS=";
		// std::string teststr = "CONNECTIONS=" + std::to_string(5);
		// x++;
		// std::strcpy(earg6, teststr.c_str());
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

		int pip[2];
		pipe(pip);
		
		// std::vector<char *>	_env;
		// std::string			tmp;
		// char				*tmp2 = NULL;

		// for (Request::container::iterator it = _request._envVar.begin(); it != _request._envVar.end(); it++)
		// {
		// 	tmp = it->first + "=" + it->second;
		// 	std::strcpy(tmp2, tmp.c_str());
		// 	_env.push_back(tmp2);
		// }

		int	*status;
		int pid = fork();
		if (pid == 0)
		{
			close(pip[0]);
			dup2(pip[1], STDOUT_FILENO);
			close(pip[1]);
			if (execve("cgi-bin/php-cgi-fail", exearg, _env) != 0)
				exit(-1);
		}
		else
		{
			close(pip[1]);
			waitpid(pid, status, 0);
			if (WEXITSTATUS(status) == -1)
			{
				std::cout << "\033[1;31mFailed to execute CGI script\033[0m" << std::endl;
				return ("500");
			}
		}
		char buf2[1000];
		int ret = read(pip[0], buf2, 999);
		close(pip[0]);
		std::string response(buf2, ret);
		_cgiResponse = response;
		return ("200");
}

void	Response::respond()
{
	if (_request._reqHead["status"].find("favicon") != std::string::npos)	// temporär, eigentlich sollten die nach GET, POST und DELETE checken
	{
		std::cout << "\033[33m		Sending response favicon to client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
		_client.sendTo("HTTP/1.1 404 Not Found\r\n");
		_client.sendTo("Content-Length: 0\r\n");
		_client.sendTo("\r\n");
	}
	else
	{
		std::cout << "\033[33m		Sending response to client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
		std::string fullresponse;
		fullresponse =	_http + " " + _resCode + " " + _resString + _header + _cgiResponse;
		_client.sendTo(fullresponse);
	}
}

bool	Response::findName(std::vector<std::string> const &names)
{
	std::string	tmp = _request._reqHead.find("Host")->second;
	std::string tar = tmp.substr(0, tmp.find_first_of(':'));
	for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); it++)
	{
		if (*it == tar)
		{
			std::cout << "		\033[33mUsing Server Block: \033[1;34m" << tmp << "\033[0m" << std::endl;
			return (1);
		}
	}
	return (0);
}

ft::ServerBlock	&Response::findBlock(ft::Server::blockmap const &blocks)
{
	for (ft::Server::blockmap::const_iterator it = blocks.begin(); it != blocks.end(); it++)
	{
		if (findName(it->second->getName()))
			return (*(it->second));
	}
	std::cout << "		\033[33mUsing default Server Block\033[0m" << std::endl;
	return (*(blocks.begin()->second));	// sollte den default server returnen, also den obersten im config file, müssen wir am besten mit ner flag machen glaub ich
}

void	Response::createHeader()
{
	_header += conLenHeader();
}

std::string	Response::conLenHeader()
{
	std::string sub;
	for (std::string::iterator it = _cgiResponse.begin(); it != _cgiResponse.end(); \
		it = _cgiResponse.begin() + (1 + _cgiResponse.find_first_of('\n', std::distance(_cgiResponse.begin(), it))))
	{
		if (*it == '\r')
		{
			sub = _cgiResponse.substr(2 + std::distance(_cgiResponse.begin(), it));
			break;
		}
	}
	return ("Content-Length: " + std::to_string(sub.size()) + "\r\n");
}
};
