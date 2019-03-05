/* in_memory_server.cc: Based on myserver.cc, but uses a MessageHandler object
 */
#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "messagehandler.h"
#include "serverhelpfunctions.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>


Server init(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: in_memory_server port-number" << std::endl;
		exit(1);
	}

	int port = -1;
	try {
		port = std::stoi(argv[1]);
	} catch (std::exception& e) {
		std::cerr << "Wrong format for port number. " << e.what() << std::endl;
		exit(2);
	}

	Server server(port);
	if (!server.isReady()) {
		std::cerr << "Server initialization error." << std::endl;
		exit(3);
	}
	return server;
}



/* Waits for commands from clients
 */
int main(int argc, char* argv[])
{
	auto server = init(argc, argv);

	while (true) {
		auto conn = server.waitForActivity();
		MessageHandler mh(conn);
		if (conn != nullptr) {
			try {
			
			
				Protocol p = mh.recvCode();
				
				switch (p) {
					case Protocol::COM_LIST_NG:    list_news_groups(&mh); break;
					case Protocol::COM_CREATE_NG:  create_news_group(&mh); break;
					case Protocol::COM_DELETE_NG:  delete_news_group(&mh); break; // delete newsgroup
					case Protocol::COM_LIST_ART:   list_articles(&mh); break; // list articles
					case Protocol::COM_CREATE_ART: create_article(&mh); break; // create article
					case Protocol::COM_DELETE_ART: delete_article(&mh); break; // delete article
					case Protocol::COM_GET_ART:    get_article(&mh); break; // get article
					default: protocolViolation("unexpected code");
				}
				
				
				
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				std::cout << "Client closed connection" << std::endl;
			}
		} else {
			conn = std::make_shared<Connection>();
			server.registerConnection(conn);
			std::cout << "New client connects" << std::endl;
		}
	}
	return 0;
}
