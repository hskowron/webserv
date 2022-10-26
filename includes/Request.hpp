#ifndef	REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <map>
# include <exception>
# include "Server.hpp"
# include "Client.hpp"

namespace ft
{
class Request
{
	friend class Response;

	public:
		typedef	std::map<std::string, std::string>	container;
		typedef	container::iterator					iterator;

	private:
		container	_envVar;
		container	_reqHead;
		std::string	_body;

	public:
		Request();
		~Request();

		container	&getEnv();
		container	&getRequest();
		std::string	&getBody();

		void		parseReq(std::string req);
		void		fillEnv();
		char		**convertEnv();
};
};

#endif