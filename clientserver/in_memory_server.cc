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
	Protocol next = mh->recvCode();
	
	if (next != Protocol::COM_END)
	{
		protocolViolation("COM_LIST_NG not followed by COM_END");
	}
	
	mh->sendCode(Protocol::ANS_LIST_NG);
	
	auto list = db.list_news_groups();
	
	mh->sendIntParameter(list.size());
	
	for(auto ng : list) {
		mh->sendIntParameter(ng.first);
		mh->sendStringParameter(ng.second);
	}
	
	mh->sendCode(Protocol::ANS_END);
}

void create_news_group(MessageHandler* mh)
{
	std::cout << "creating news group\n";
	
	std::string name = mh->recvStringParameter();
	
	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_CREATE_NG);
	
	if (db.create_news_group(name))
	{
		mh->sendCode(Protocol::ANS_ACK);
	}
	else
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}	
	mh->sendCode(Protocol::ANS_END);
}

/* Waits for commands from clients
 */
int main(int argc, char* argv[])
{
	auto server = init(argc, argv);

	//std::cout << "created newsgroup: " << db.create_news_group("test group") << std::endl;

	while (true) {
		auto conn = server.waitForActivity();
		MessageHandler mh(conn);
		if (conn != nullptr) {
			try {
			
			
				Protocol p = mh.recvCode();
				
				switch (p) {
					case Protocol::COM_LIST_NG:    list_news_groups(&mh); break;
					case Protocol::COM_CREATE_NG:  create_news_group(&mh); break;
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
