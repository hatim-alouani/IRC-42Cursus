#include "server.hpp"
#include "replies.hpp"

bool Server::sig_serv = true;

void Server::sig_handler(int sig)
{
	(void)sig;
	Server::sig_serv = false;
}

void Server::start_server()
{
	std::cout << "srever is lestenning to port : " << port << "\n" << std::endl;
	std::vector<pollfd> pfds;

	/*pfd.events = POLLIN
		means “notify me when this socket is ready to read”—in this case, when a new client is trying to connect
	*/
	struct pollfd pfd; pfd.fd = server_fd; pfd.events = POLLIN;pfd.revents = 0;
	pfds.push_back(pfd);
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	while (Server::sig_serv)
	{
		/* Waits for one or more file descriptors to become ready (e.g., ready for reading or writing).*/
		/* -1 blocks indefinitely until at least one socket has some activity */
		int check_poll = poll(pfds.data(), pfds.size(), -1);
		if (check_poll < 0)
		{
			break;
		}
		if (pfds[0].revents & POLLIN)/*if the server socket (pfds[0]) has incoming connection(s) ready to accept*/
		{
			int new_users_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
			if (new_users_fd < 0)
			{
				std::cerr << "error accepting new client." << std::endl;
				continue;
			}

			struct pollfd new_pfd; new_pfd.fd = new_users_fd; new_pfd.events = POLLIN; new_pfd.revents = 0;
			pfds.push_back(new_pfd);

			char ip[INET_ADDRSTRLEN];
			/*inet_ntop : converts the binary IPv4 address to a readable string*/
			inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
			PendingClient new_pending(new_users_fd);
			std::string ip_str(ip);
			new_pending.set_client_ip(ip_str);
			pending_users.push_back(new_pending);
			std::cout << "New client connected, waiting for authentication..." << std::endl;
		}

		size_t i = 1;
		while(i < pfds.size())
		{
			if (pfds[i].revents & POLLIN)
			{
				char buff[1024];
				int n = read(pfds[i].fd, buff, sizeof(buff) - 1);
				if (n >= 1023)
				{
					n -= 1;
					buff[n] = '\0';
				}
				if (n <= 0)
				{
					close (pfds[i].fd);
					std::cout << "Client disconnected!" << std::endl;
					kickFromChannels(pfds[i].fd);
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
						std::string save1 = pending->getBuffer();
						save1 += buff;
						if (save1.size() >= 1024)
							pending->setBufferEmpty();
						if (save1.find('\n') != std::string::npos){
							pending->setBufferEmpty();
							std::vector<std::string> tokens = splitByLine(save1);
							for (size_t i = 0; i < tokens.size(); ++i){
								if (isReadyForRegistration(splitBySpace(tokens[i]), pending))
								{
									std::string msg = "Welcome to the Internet Relay Network " + pending->getNickname() + "!" + pending->getUsername() + "@" + pending->getHostname();
									sendReply(pending->get_fd(), RPL_WELCOME(pending->getNickname(), msg));
									std::cout << "\n" << "Welcome to the Internet Relay Network " << pending->getNickname() << "\n" << std::endl;
									User new_user(pending->get_fd(), pending->getUsername(), pending->get_ip_client(), pending->getServername(), pending->getRealname(), pending->getNickname());
									users.push_back(new_user);
									remove_pending_client(pending->get_fd());
									break;
								}
							}
						}
						else
							pending->setBuffer(buff);
					}
					else if (user)
					{
						std::string save2 = user->getBuffer();
						if (save2.size() >= 1024)
							user->setBufferEmpty();
						save2 += buff;
						if (save2.find('\n') != std::string::npos){
							user->setBufferEmpty();
							dealWIthUser(save2, user);
						}
						else
							user->setBuffer(buff);
					}
				}
			}
			i++;
		}
	}
	size_t k = 0;
	while (k < pfds.size())
	{
		if (pfds[k].fd >= 0)
			close(pfds[k].fd);
		k++;
	}
	pfds.clear();
	close(server_fd);
}

Server::Server(int port, std::string password) : port(port), password(password)
{
	/*
	socket(AF_INET, SOCK_STREAM, 0);
		This creates a TCP socket using the IPv4 protocol (AF_INET).
		SOCK_STREAM means we want a reliable, connection-oriented stream (TCP).
		0 allows the system to choose the appropriate protocol automatically (which is TCP for SOCK_STREAM)
	*/
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw std::runtime_error("Failed to create socket");

	/*
	SOL_SOCKET: socket-level options.

	SO_REUSEADDR :
		allows the server to reuse the port immediately after a previous instance closed,
		even if it's still in TIME_WAIT
	*/
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Failed to set socket options");

	/*Changes the socket mode to non-blocking.
	makes calls like read an accept return immediately if there’s nothing to do
		F_SETFL: set file status flags.
		O_NONBLOCK: flag to enable non-blocking mode*/
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	sockaddr_in serv_add;
	std::memset(&serv_add, 0, sizeof(serv_add));
	serv_add.sin_family = AF_INET;/*set to AF_INET for IPv4*/
	serv_add.sin_addr.s_addr = INADDR_ANY;/*means listen on all interfaces (e.g., LAN IP and localhost)*/
	serv_add.sin_port = htons(port);/*network byte order*/

	/*Binds the server socket to a specific IP address and port so it can listen for incoming connections there*/
	if (bind(server_fd, (sockaddr *)&serv_add, sizeof(serv_add)))
		throw std::runtime_error("Failed to bind socket");

	/*
	Marks the socket as a passive socket to accept incoming connection requests
	SOMAXCONN(max pending connections waiting)
	*/
	if(listen(server_fd, SOMAXCONN))
		throw std::runtime_error("Failed to listen on socket");
}

std::vector <User> Server::getUsers(){
	return this->users;
}

Channel* Server::getChannel(std::string& name, std::string& key){
	std::vector <Channel>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
		if (it->getName() == name)
			return &(*it);
	Channel new_channel(name, key);
	channels.push_back(new_channel);
	return &channels[channels.size() - 1];
}

Channel* Server::getChannelName(std::string& delChannel){
	if (channels.empty())
		return NULL;
	std::vector <Channel>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
		if (it->getName() == delChannel)
			return &(*it);
	return NULL;
}

User* Server::getDelUser(std::string& name){
	std::vector <User>::iterator it = users.begin();
	for (; it != users.end(); ++it)
		if (it->getNickname() == name)
			return &(*it);
	return NULL;
}

User* Server::getUserByFd(int fd){
	std::vector <User>::iterator it = users.begin();
	for (; it != users.end(); ++it)
		if (it->get_fd() == fd)
			return &(*it);
	return NULL;
}

void Server::dealWIthUser(std::string& buff, User* user){
	std::vector<std::string> tokens = splitBySpace(buff);
	if (tokens.empty())
		return;
	if (!user)
		return ;
	std::string command = tokens[0];
	if (command == "JOIN" && tokens.size() >= 2 && tokens.size() <= 3){
		joinCmd(tokens, user);
	}
	else if (command == "KICK" && tokens.size() <= 4){
		kickCmd(tokens, user);
	}
	else if (command == "INVITE"  && tokens.size() <= 3){
		inviteCmd(tokens, user);
	}
	else if (command == "PRIVMSG"){
		handlePrivateMessage(tokens, users, channels, user);
	}
	else if (command == "TOPIC"){
		topicCmd(tokens, user);
	}
	else if (command == "MODE"){
		modeCmd(tokens, user);
	}
	else if (command == "NICK"){
		nickChangeCmd(tokens, user);
	}
	else if ((command == "USER" || command == "PASS") && tokens.size() == 2){
		sendReply(user->get_fd(), ERR_ALREADYREGISTRED(user->getNickname()));
	}
	else if (command == "USER" && tokens.size() == 2){
		sendReply(user->get_fd(), ERR_ALREADYREGISTRED(user->getNickname()));
	}
	else if (command == "PONG"){}
	else
	{
		sendReply(user->get_fd(), ERROR_UNKNOWNCOMMAND(user->getNickname(), user->getHostname(), command));
	}
}

void Server::kickFromChannels(int fd){
	for (size_t i=0; i < channels.size(); ++i){
		bool yes = false;
		Channel& channel = channels[i];
		if (channel.isUserInChannel(fd)){
			if (channel.isOperator(fd)){
				if (!channel.checkOperators()){
					if (channel.getUsers_fd().size() > 1){
						yes = true;
					}
				}
			}
			User* user = channel.getUserByfd(fd, users);
			if (!user)
				return;
			channel.removeUser(fd);
			user->leaveChannel(channel.getName());
			std::string msg = ":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname() + " QUIT :Client Quit\r\n";
			const std::vector<int>& user_fds = channel.getUsers_fd();
			for (size_t i = 0; i < user_fds.size(); ++i) {
				sendReply(user_fds[i], msg);
			}
			if (yes){
				User* opUser = channel.getUserByfd(channel.getUsers_fd()[0], users);
				channel.addOperator(opUser->get_fd());
				std::string msg = ":" + opUser->getNickname() + "!" + opUser->getUsername() + "@" + opUser->getHostname() +
	                  " MODE " + channel.getName() + " +o " + opUser->getNickname() + "\r\n";
				for (std::vector<int>::const_iterator it = channel.getUsers_fd().begin(); it != channel.getUsers_fd().end(); ++it){
					sendReply(*it, msg);
				}
			}
		}
	}
}

Channel* Server::getChannel(const std::string& name) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].getName() == name)
            return &channels[i];
    }
    return NULL;
}

std::vector<Channel>& Server::getChannels() {
    return this->channels;
}

bool Server::checkNickname(std::string& nickname, int user_fd){
	std::vector<User>::const_iterator it = users.begin();
	if (nickname.empty() || !isValidNickname(nickname)) {
        sendReply(user_fd, ERR_ERRONEUSNICKNAME(nickname));
        return false;
    }
    for (; it != users.end(); ++it) {
        if (it->getNickname() == nickname) {
			sendReply(user_fd, ERR_NICKNAMEINUSE(nickname));
            return false;
        }
    }
	std::vector<PendingClient>::const_iterator itt = pending_users.begin();
	for (; itt != pending_users.end(); ++itt) {
		if (itt->getNickname() == nickname) {
			sendReply(user_fd, ERR_NICKNAMEINUSE(nickname));
			return false;
		}
	}
    return true;
}
