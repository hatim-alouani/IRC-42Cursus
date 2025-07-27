#include "server.hpp"

bool valid(std::string str)
{
	size_t i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '	')
			return (false);
		i++;
	}
	return (true);
}


int main (int ac , char **av)
{
	if (ac == 3)
	{
		try
		{
			signal(SIGQUIT, Server::sig_handler);
			signal(SIGINT, Server::sig_handler);
			signal(SIGPIPE, SIG_IGN);
			int port = std::atoi(av[1]);
			if (port < 1024 || port > 49151)
			{
				std::cerr << "invalid port number" << std::endl;
				return 1;
			}
			std::string password = av[2];
			if (password.size() < 3 || !valid(password))
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
	}
	else
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
	}
}
