#include "../includes/Config.hpp"

namespace ft
{
	Config::Config(std::string &confPath)
	{
		this->_confFile = readFile(confPath);
	}

	std::string Config::readFile(std::string &confPath)
	{
		std::ifstream	readFile(confPath);
		std::string		line;
		std::string		content;

		while (getline(readFile, line))
			content.append(line);
		return (content);
	}

	std::string Config::getconfFile()
	{
		return (this->_confFile);
	}

	Config::Config(ft::Config &config)
	{
		this->_confFile = config._confFile;
	}

	Config::~Config()
	{
		
	}
}