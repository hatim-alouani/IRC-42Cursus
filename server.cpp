#include "server.hpp"

void Server::start_server()
{
	std::cout << "srever is lestenning to port : " << port << "\n" << std::endl;
	std::vector<pollfd> pfds;

	struct pollfd pfd; pfd.fd = server_fd; pfd.events = POLLIN;pfd.revents = 0;
	pfds.push_back(pfd);
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	while (true)
	{
		int check_poll = poll(pfds.data(), pfds.size(), BLOCK_WAIT);
		if (check_poll < 0)
		{
			std::cerr << "pll " << std::endl;
			break;
		}
		if (pfds[0].revents & POLLIN)
		{
			int new_client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);/*!*/
			if (new_client_fd < 0)
			{
				std::cerr << "error accepting new client." << std::endl;
				continue;
			}
			/*fcntl(new_client_fd, F_SETFL, O_NONBLOCK);*/
			struct pollfd new_pfd; new_pfd.fd = new_client_fd; new_pfd.events = POLLIN; new_pfd.revents = 0;
			pfds.push_back(new_pfd);
			/*now I gotta check the password and the rest of data before accepting him or not */
			clients.push_back(Client(new_client_fd));
			std::cout << "new client connected!" << "\n" << std::endl;
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
					clients.erase(clients.begin() + (i - 1));
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

Server::Server(int port, std::string password) : port(port), password(password)
{
	server_fd = socket(AF_INET, SOCK_STREAM, DEF_PROTOCOL);

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	sockaddr_in serv_add;
	std::memset(&serv_add, 0, sizeof(serv_add));
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY;
	serv_add.sin_port = htons(port);

	bind(server_fd, (sockaddr *)&serv_add, sizeof(serv_add));

	listen(server_fd, MAX_PENDING);
}