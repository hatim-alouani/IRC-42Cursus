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
			std::cerr << "poll " << std::endl;
			break;
		}
		if (pfds[0].revents & POLLIN)
		{
			int new_users_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);/*!*/
			if (new_users_fd < 0)
			{
				std::cerr << "error accepting new client." << std::endl;
				continue;
			}

			struct pollfd new_pfd; new_pfd.fd = new_users_fd; new_pfd.events = POLLIN; new_pfd.revents = 0;
			pfds.push_back(new_pfd);

			pending_users.push_back(new_users_fd);
			std::cout << "New client connected, waiting for authentication..." << std::endl;
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
					// pfds.erase(pfds.begin() + i);
					// users.erase(users.begin() + (i - 1));
					remove_pending_client(pfds[i].fd);
					remove_user(pfds[i].fd);
					pfds.erase(pfds.begin() + i);
					i--;
				}
				else
				{
					buff[n] = '\0';
					PendingClient *pending = look_for_pending(pfds[i].fd);
					User *user = look_for_user(pfds[i].fd);

					if (pending)
					{
						/*deal with pending*/
						if (isReadyForRegistration((std::string&)buff, pending))
							std::cout << "marhba biiik" << std::endl;
						else
							std::cout << "sir t9wd" << std::endl;
					}
					else if (user)
					{
						/*deal with user*/
					}
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
