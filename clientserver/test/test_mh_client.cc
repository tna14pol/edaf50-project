/* test_mh_client.cc: Based on myserver.cc, but uses a MessageHandler object
 * the objective of this class is to test some of the functions in messagehandler.cc
 */
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>

using namespace std;

/* Creates a client for the given args, if possible.
 * Otherwise exits with error code.
 */
std::shared_ptr<Connection> init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: test_mh_client host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[2]);
        } catch (exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        std::shared_ptr<Connection> conn(new Connection(argv[1], port));
        if (!conn->isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}

/* First sends three ints, and then a string
 * Checks that the server answers with the sum of the ints and the string reversed
 */
int main(int argc, char* argv[])
{
	std::shared_ptr<Connection> conn = init(argc, argv);
	MessageHandler mh(conn);
	try {
		mh.sendIntParameter(-1);
		mh.sendIntParameter(0);
		mh.sendIntParameter(4);
		cout << "-1 + 0 + 4 = " << mh.recvIntParameter() << endl;
		mh.sendStringParameter("test");
		cout << "test " << mh.recvStringParameter() << endl;
	} catch (ConnectionClosedException&) {
		cout << " no reply from server. Exiting." << endl;
		return 1;
	}
	cout << "\nexiting.\n";
	return 0;
}
