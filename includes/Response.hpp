#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <map>
# include <unistd.h>
# include "Request.hpp"
# include "Client.hpp"
# include "Server.hpp"

namespace ft
{
class Response
{
	public:
		typedef std::map<std::string, std::string>	CodeCont;

	private:
		std::string		_http;
		std::string		_resCode;
		std::string 	_resString;
		std::string		_header;
		std::string		_cgiResponse;
		ft::Request		_request;
		CodeCont		_codeList;
		ft::Client		_client;
		ft::ServerBlock	_block;

	public:
		Response(ft::Client const &client, ft::Server::blockmap const &blocks);
		~Response();

		void			setHttp(std::string const &http);
		void			setCode(std::string const &code);
		void			setResponse(std::string const &response);

		std::string		cgi();
		void			respond();
		void			initCode();
		ft::ServerBlock	&findBlock(ft::Server::blockmap	const &blocks);
		bool			findName(std::vector<std::string> const &names);
		void			createHeader();
		std::string		conLenHeader();

		std::string		findString();
		std::string		&formResponse();
};
};

#endif