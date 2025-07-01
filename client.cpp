#include "server.hpp"

int Client::get_fd() const
{
	return (client_fd);
}

Client::Client(int fd):client_fd(fd)
{
	/*init the rest of data later */
}

Client *Server::look_for(int fd)
{
	int i = 0;
	while (i < clients.size())
	{
		if (clients[i].get_fd() == fd)
			return (&clients[i]);
		i++;
	}
	return (NULL);
}