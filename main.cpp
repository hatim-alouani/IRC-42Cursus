#include "server.hpp"

int main (int ac , char **av)
{
	if (ac == 3)
	{
		try
		{
			int port = std::atoi(av[1]);
			if (port <= 0 || port >= 65535)
			{
				std::cerr << "invalid port number" << std::endl;
				return 1;
			}
			std::string password = av[2];
			if (password.size() < 3)
			{
				std::cerr << "invalid password " << std::endl;
				return 1;
			}
			Server server(port, password);
			server.start_server();
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
		return (0);
	}
}
