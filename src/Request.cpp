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
	_envVar["SERVER_PROTOCOL"];
	_envVar["SERVER_SOFTWARE"];
	_envVar["CONNECTIONS"];
}
}
