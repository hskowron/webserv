#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <map>
# include <iostream>
# include <fstream>
# include <sstream>

namespace ft
{
class	Config
{
	private:std::string							_confFile;

		
	public:
		Config();
		Config(ft::Config &config);
		Config(std::string &confPath);
		std::string readFile(std::string &confPath);
		std::string getconfFile();
		~Config();
};
};

#endif