#include "../includes/Response.hpp"

namespace ft
{
Response::Response(ft::Client &client) : _client(client)
{
	_http = "HTTP/1.1";
	std::string	buf;
	initCode();
	if (_client.recvFrom(&buf) == -1)
		std::cout << "error: recv from client fd: " << _client.getSockfd() << "  port: " << _client.getPort() << std::endl;
	_request.parseReq(buf);
	std::cout << "\033[33m		Receiving data from client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
}

Response::~Response()
{

}

void	Response::setCode(std::string code)
{
	_resCode = code;
	CodeCont::iterator	it = _codeList.find(code);
	if (it == _codeList.end())
		throw (std::runtime_error("error: Bad status code"));
	_resString = it->second;
}

void	Response::initCode()
{
	_codeList["200"] = "OK";
	_codeList["404"] = "Not found";
}

std::string	Response::methodGet()
{
	return ("200");
}

std::string	Response::methodPost()
{
	retun ()
}

std::string	Response::methodDelete()
{

}

void	Response::cgi()
{
	// static int x = 1;
	

	
	// std::cout << "buf size: " << buf.size() << std::endl << "buf content:" << std::endl << buf << std::endl << "content end" << std::endl;

	// std::cout << "request: " << std::endl;
	// for (ft::Request::iterator it = _request.getRequest().begin(); it != _request.getRequest().end(); it++)
	// 	std::cout << it->first << " = " << it->second << std::endl;
	// std::cout << _request.getBody() << std::endl;
	if (!buf.empty() && buf.find("GET / HTTP/1.1") != std::string::npos)
	{
		std::cout << "\033[33m		Sending response to client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
		_client.sendTo("HTTP/1.1 200 OK\r\n");
		char arg[] = {"index.php"};
		char arg2[] = {"php-cgi"};
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
		int ret = read(p[0], buf2, 999);
		close(p[0]);
		std::string text(buf2, ret);
		std::string subtext(text.substr(text.find_first_of("<")));

		// std::cout << "html:" << std::endl << text << std::endl << "html end" << std::endl;

		_client.sendTo("Content-Length: " + std::to_string(subtext.size()) + "\r\n");
		_client.sendTo(text);
		_client.sendTo("\r\n");
	}
	else if (!buf.empty() && buf.find("GET /favicon.ico HTTP/1.1") != std::string::npos)
	{
		std::cout << "\033[33m		Sending response favicon to client on port: " << _client.getPort() << " (client fd: " << _client.getSockfd() << ")\033[0m" << std::endl;
		_client.sendTo("HTTP/1.1 404 Not Found\r\n");
		_client.sendTo("Content-Length: 0\r\n");
		_client.sendTo("\r\n");
	}
}
};
