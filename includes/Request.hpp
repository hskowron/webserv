#ifndef	REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <map>
# include "Server.hpp"
# include "Client.hpp"

namespace ft
{
class Request
{
	typedef	std::map<std::string, std::string>	container;
	typedef	container::iterator					iterator;

	private:
		container	_envVar;

	public:
		Request();
		~Request();
};
};

#endif