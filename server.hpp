#ifndef SERVER_HPP
#define SERVER_HPP

#include "pendingClient.hpp"
#include "user.hpp"
#include "channel.hpp"

class Server
{
	private :
		int server_fd;
		int port;
		std::vector <User> users;
		std::vector <PendingClient> pending_users;
		std::vector <Channel> channels;
		std::string password;
	public :
		Server(int port, std::string password);
		void start_server();
		User *look_for_user(int fd);
		PendingClient *look_for_pending(int fd);

		std::vector <User> getUsers();

		void remove_pending_client(int fd);
		void remove_user(int fd);
		void pending_to_user(PendingClient *client);/*later*/

		bool isReadyForRegistration(std::string& comand, PendingClient* pending) const;
		
		void dealWIthUser(std::string& buff, User* user);
		Channel* getChannel(std::string& name, std::string& key);
};

#endif













