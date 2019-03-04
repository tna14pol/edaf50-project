/* in_memory_server.cc: Based on myserver.cc, but uses a MessageHandler object
 */
#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "messagehandler.h"
#include "databaseinterface.h"
#include "inmemorydb.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>


InMemoryDB db = InMemoryDB();

/*
 * Throws a runtime exception
 */
void protocolViolation(std::string msg)
{
	std::cerr << "Protocol violation: " + msg << std::endl;
	throw std::runtime_error("Protocol violation: " + msg);
}

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

void list_news_groups(MessageHandler* mh)
{
	std::cout << "listing news groups\n";
	Protocol next = mh->recvCode();
	if (next != Protocol::COM_END)
	{
		mh->sendCode(Protocol::ANS_NAK);
		protocolViolation("COM_LIST_NG not followed by COM_END");
	}
	mh->sendCode(Protocol::ANS_LIST_NG);
	auto list = db.list_news_groups();
	std::cout << "sending list of " << list.size() << " news groups\n";
	mh->sendIntParameter(list.size());
	for(auto ng : list) {
		std::cout << "sending int param: " << ng.first << "\n";
		mh->sendIntParameter(ng.first);
		std::cout << "sending string param: " << ng.second << "\n";
		mh->sendStringParameter(ng.second);
	}
	mh->sendCode(Protocol::ANS_END);
}

/* Waits for commands from clients
 */
int main(int argc, char* argv[])
{
	auto server = init(argc, argv);

	std::cout << "created newsgroup: " << db.create_news_group("test group") << std::endl;

	while (true) {
		auto conn = server.waitForActivity();
		MessageHandler mh(conn);
		if (conn != nullptr) {
			try {
			
			
				Protocol p = mh.recvCode();
				
				switch (p) {
					case Protocol::COM_LIST_NG: list_news_groups(&mh);break; // list newsgroups
					case Protocol::COM_CREATE_NG:  std::cout << "COM_CREATE_NG\n"; break; // create newsgroup
					case Protocol::COM_DELETE_NG:  std::cout << "COM_DELETE_NG\n"; break; // delete newsgroup
					case Protocol::COM_LIST_ART:   std::cout << "COM_LIST_ART\n"; break; // list articles
					case Protocol::COM_CREATE_ART: std::cout << "COM_CREATE_ART\n"; break; // create article
					case Protocol::COM_DELETE_ART: std::cout << "COM_DELETE_ART\n"; break; // delete article
					case Protocol::COM_GET_ART:    std::cout << "COM_GET_ART\n"; break; // get article
					default: std::cout << "other:" << std::endl;
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
