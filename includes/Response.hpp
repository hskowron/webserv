#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include "Request.hpp"

namespace ft
{
class Response
{
	private:
		std::string	_http;
		std::string	_resCode;
		std::string _resString;
		std::string	_response;
		ft::Request	_request;
		// std::string _codeList[];

	public:
		Response();
		~Response();

		std::string getHttp() const;
		std::string getCode() const;
		std::string getString() const;
		std::string getResponse() const;

		void	setHttp(std::string http);
		void	setCode(std::string code);
		void	setString(std::string str);
		void	setResponse(std::string response);

		void	cgi();

		std::string	findString();
};
};

#endif