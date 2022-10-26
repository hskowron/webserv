#include "../includes/Request.hpp"

namespace ft
{
Request::Request()
{
	_envVar["AUTH_TYPE"];
	_envVar["CONTENT_LENGTH"];
	_envVar["CONTENT_TYPE"];
	_envVar["GATEWAY_INTERFACE"] = "CGI/1.1";
	_envVar["PATH_INFO"];
	_envVar["PATH_TRANSLATED"];
	_envVar["QUERY_STRING"];
	_envVar["REMOTE_ADDR"];
	_envVar["REMOTE_HOST"];
	_envVar["REMOTE_IDENT"];
	_envVar["REMOTE_USER"];
	_envVar["REQUEST_METHOD"];
	_envVar["SCRIPT_NAME"];
	_envVar["SERVER_NAME"];
	_envVar["SERVER_PORT"];
	_envVar["SERVER_PROTOCOL"] = "HTTP/1.1";
	_envVar["SERVER_SOFTWARE"] = "webserv/1.0";
	_envVar["CONNECTIONS"];
}

Request::~Request()
{
	
}

Request::container	&Request::getEnv()
{
	return (_envVar);
}

Request::container	&Request::getRequest()
{
	return (_reqHead);
}

std::string	&Request::getBody()
{
	return (_body);
}

void	Request::parseReq(std::string req)
{
	if (req.empty())
		throw (std::runtime_error("error: Client request empty"));
	for (std::string::iterator	it = req.begin(); it != req.end(); it = req.begin() + (1 + req.find_first_of('\n', std::distance(req.begin(), it))))
	{
		if (it == req.begin())
			_reqHead["status"] = req.substr(0, req.find_first_of('\r'));
		else if (*it == '\r')
		{
			if (it + 2 != req.end())
			{
				int bod = std::distance(req.begin(), it + 2);
				_body = req.substr(bod, req.size() - bod);
			}
			break ;
		}
		else
		{
			int	dist = std::distance(req.begin(), it);
			_reqHead[req.substr(dist, req.find_first_of(':', dist) - dist)] = req.substr(req.find_first_of(' ', dist) + 1, req.find_first_of('\r', dist) - (req.find_first_of(' ', dist) + 1));
		}
	}
}

void	fillEnv()
{

}
}
