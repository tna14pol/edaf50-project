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

void delete_news_group(MessageHandler* mh)
{
	unsigned int id_nbr = mh->recvIntParameter();

	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_DELETE_NG);
	
	if (db.delete_news_group(id_nbr))
	{
		mh->sendCode(Protocol::ANS_ACK);
	}
	else
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh->sendCode(Protocol::ANS_END);
}

void list_articles(MessageHandler* mh)
{
	unsigned int id_nbr = mh->recvIntParameter();

	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_LIST_ART);
	
	try
	{
		auto list = db.list_articles(id_nbr);
	
		mh->sendCode(Protocol::ANS_ACK);
		mh->sendIntParameter(list.size());
	
		for(auto a : list) {
			mh->sendIntParameter(a.first);
			mh->sendStringParameter(a.second);
		}
	}
	catch (const std::out_of_range&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	mh->sendCode(Protocol::ANS_END);
}

void create_article(MessageHandler* mh)
{
	unsigned int id_nbr = mh->recvIntParameter();
	std::string title = mh->recvStringParameter();
	std::string author = mh->recvStringParameter();
	std::string text = mh->recvStringParameter();
	
	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_CREATE_ART);
	try {
		db.create_article(id_nbr, title, author, text);
		mh->sendCode(Protocol::ANS_ACK);
	}
	catch (const std::out_of_range&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh->sendCode(Protocol::ANS_END);
}

void delete_article(MessageHandler* mh)
{
	unsigned int ng_id = mh->recvIntParameter();
	unsigned int a_id = mh->recvIntParameter();
	
	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_DELETE_ART);
	try {
		db.delete_article(ng_id, a_id);
		mh->sendCode(Protocol::ANS_ACK);
	}
	catch (const std::out_of_range& oor)
	{
		mh->sendCode(Protocol::ANS_NAK);
		std::string news_group = "news group";
		std::string article = "article";
		if (news_group.compare(oor.what()))
		{
			mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		else if (article.compare(oor.what()))
		{
			mh->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else
		{
			throw oor;
		}
	}
	mh->sendCode(Protocol::ANS_END);
}

void get_article(MessageHandler* mh)
{
	unsigned int ng_id = mh->recvIntParameter();
	unsigned int a_id = mh->recvIntParameter();
	
	if (mh->recvCode() != Protocol::COM_END)
	{
		protocolViolation("expected COM_END");
	}
	
	mh->sendCode(Protocol::ANS_GET_ART);
	try {
		auto t = db.get_article(ng_id, a_id);
		mh->sendCode(Protocol::ANS_ACK);
		mh->sendStringParameter(std::get<0>(t));
		mh->sendStringParameter(std::get<1>(t));
		mh->sendStringParameter(std::get<2>(t));
		
	}
	catch (const std::out_of_range& oor)
	{
		mh->sendCode(Protocol::ANS_NAK);
		std::string news_group = "news group";
		std::string article = "article";
		if (news_group.compare(oor.what()))
		{
			mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		else if (article.compare(oor.what()))
		{
			mh->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else
		{
			throw oor;
		}
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
