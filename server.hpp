#ifndef SERVER_HPP
#define SERVER_HPP

#include "pendingClient.hpp"
#include "client.hpp"

class Server
{
	private :
		int server_fd;
		std::vector <User> users;
		std::vector<PendingClient> pending_users;
		std::string password;
		int port;
	public :
		Server(int port, std::string password);
		void start_server();
		User *look_for_user(int fd);
		PendingClient *look_for_pending(int fd);

		void remove_pending_client(int fd);
		void remove_user(int fd);
		void pending_to_user(PendingClient *client);/*later*/

		bool isReadyForRegistration(std::string& comand, PendingClient* pending) const;
};

#endif













