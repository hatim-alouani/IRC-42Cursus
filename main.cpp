#include "server.hpp"

int main (int ac , char **av)
{
	if (ac == 3)
	{
		int port = std::atoi(av[1]);
		std::string password = av[2];
		Server server(port, password);
		server.start_server();
	}
}
