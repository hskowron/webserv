#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <map>
# include <unistd.h>
# include "Request.hpp"
# include "Client.hpp"

namespace ft
{
class Response
{
	public:
		typedef std::map<std::string, std::string>	CodeCont;

	private:
		std::string	_http;
		std::string	_resCode;
		std::string _resString;
		std::string	_header;
		std::string	_response;
		ft::Request	_request;
		CodeCont	_codeList;
		ft::Client	_client;

	public:
		Response(ft::Client &client);
		~Response();

		std::string getHttp() const;
		std::string getCode() const;
		std::string getString() const;
		std::string getResponse() const;

		void	setHttp(std::string http);
		void	setCode(std::string code);
		void	setResponse(std::string response);

		std::string	methodGet();
		std::string	methodPost();
		std::string	methodDelete();

		void	cgi();
		void	initCode();

		std::string	findString();
};
};

#endif