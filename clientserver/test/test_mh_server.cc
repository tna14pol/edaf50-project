/* test_mh_server.cc: Based on myserver.cc, but uses a MessageHandler object
 * the objective of this class is to test some of the functions in messagehandler.cc
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
		cerr << "Usage: test_mh_server port-number" << endl;
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

/* First receives three ints and writes back the sum om them
 * Then receives a string and writes it back mirrored
 */
int main(int argc, char* argv[])
{
	auto server = init(argc, argv);

	while (true) {
		auto conn = server.waitForActivity();
		MessageHandler mh(conn);
		if (conn != nullptr) {
			try {
				int sum{0};
				sum += mh.recvIntParameter();
				sum += mh.recvIntParameter();
				sum += mh.recvIntParameter();
				mh.sendIntParameter(sum);
				string s = mh.recvStringParameter();
				std::reverse( s.begin(), s.end());
				mh.sendStringParameter(s);
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
