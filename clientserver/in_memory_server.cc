/* in_memory_server.cc: Based on myserver.cc, but uses a MessageHandler object
 */
#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>

using namespace std;

Server init(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "Usage: in_memory_server port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong format for port number. " << e.what() << endl;
		exit(2);
	}

	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
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
				std::cout << "Received command: ";
				switch (p) {
					case Protocol::COM_LIST_NG:    std::cout << "COM_LIST_NG\n"; break; // list newsgroups
					case Protocol::COM_CREATE_NG:  std::cout << "COM_CREATE_NG\n"; break; // create newsgroup
					case Protocol::COM_DELETE_NG:  std::cout << "COM_DELETE_NG\n"; break; // delete newsgroup
					case Protocol::COM_LIST_ART:   std::cout << "COM_LIST_ART\n"; break; // list articles
					case Protocol::COM_CREATE_ART: std::cout << "COM_CREATE_ART\n"; break; // create article
					case Protocol::COM_DELETE_ART: std::cout << "COM_DELETE_ART\n"; break; // delete article
					case Protocol::COM_GET_ART:    std::cout << "COM_GET_ART\n"; break; // get article
					default: std::cout << "none\n";
				}
				mh.sendCode(Protocol::ANS_ACK);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
	return 0;
}
