#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>

class User
{
	private :
		std::string name;
		int client_fd;
	public :
		User(int fd);
		int get_fd() const;
};

int User::get_fd() const
{
	return (client_fd);
}

User::User(int fd):client_fd(fd)
{
	/*init the rest of data later */
}

class Irc
{
	private :
		int server_fd;
		std::vector <User> users;
		std::string password;
		int port;
	public :
		Irc(int port, std::string password);
		void start_server();
		User *look_for(int fd);
};

User *Irc::look_for(int fd)
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


void Irc::start_server()
{
	std::cout << "srever is lestenning to port : " << port << "\n" << std::endl;
	std::vector<pollfd> pfds;

	struct pollfd pfd; pfd.fd = server_fd; pfd.events = POLLIN;pfd.revents = 0;
	pfds.push_back(pfd);
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	while (true)
	{
		int check_poll = poll(pfds.data(), pfds.size(), -1);
		if (pfds[0].revents & POLLIN)
		{
			int new_client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);/*!*/
			if (new_client_fd < 0)
			{
				std::cerr << "error accepting new client." << std::endl;
				continue;
			}
			fcntl(new_client_fd, F_SETFL, O_NONBLOCK);
			std::cout << "new client connected!" << "\n" << std::endl;
			struct pollfd new_pfd; new_pfd.fd = new_client_fd; new_pfd.events = POLLIN; new_pfd.revents = 0;
			pfds.push_back(new_pfd);
			users.push_back(User(new_client_fd));
		}

		int i = 1;
		while(i < pfds.size())
		{
			if (pfds[i].revents & POLLIN)
			{
				char buff[1024];
				int n = read(pfds[i].fd, buff, sizeof(buff) - 1);
				if (n <= 0)
				{
					close (pfds[i].fd);
					std::cout << "Client disconnected!" << std::endl;
					pfds.erase(pfds.begin() + i);
					users.erase(users.begin() + (i - 1));
					i--;
				}
				else
				{
					buff[n] = '\0';
					std::cout << "received form " << i << ": " << buff << std::endl;
				}
			}
			i++;
		}
	}
	close(server_fd);
}

Irc::Irc(int port, std::string password) : port(port), password(password)
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	sockaddr_in serv_add;
	std::memset(&serv_add, 0, sizeof(serv_add));
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY;
	serv_add.sin_port = htons(port);

	bind(server_fd, (sockaddr *)&serv_add, sizeof(serv_add));

	listen(server_fd, 1);
}

int main (int ac , char **av)
{
	if (ac == 3)
	{
		int port = std::atoi(av[1]);
		std::string password = av[2];
		Irc server(port, password);
		server.start_server();
	}
}
