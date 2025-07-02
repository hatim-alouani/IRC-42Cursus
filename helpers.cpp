#include "server.hpp"

User *Server::look_for_user(int fd)
{
	int i = 0;
	while (i < users.size())
	{
		if (users[i].get_fd() == fd)
			return (&users[i]);
		i++;
	}
	return (NULL);
}

PendingClient *Server::look_for_pending(int fd)
{
	int i = 0;
	while (i < pending_users.size())
	{
		if (pending_users[i].get_fd() == fd)
			return (&pending_users[i]);
		i++;
	}
	return (NULL);
}

void Server::remove_pending_client(int fd)
{
	for (size_t i = 0; i < pending_users.size(); i++)
	{
		if (pending_users[i].get_fd() == fd)
		{
			pending_users.erase(pending_users.begin() + i);
			break;
		}
	}
}

void Server::remove_user(int fd)
{
	int i = 0;
	while (i < users.size())
	{
		if (users[i].get_fd() == fd)
		{
			users.erase(users.begin() + i);
			break;
		}
		i++;
	}
}
