#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "connection.h"
#include "protocol.h"
#include <string>
#include <memory>

class MessageHandler {
public:
	MessageHandler(std::shared_ptr<Connection> conn);
	void sendCode(Protocol) const;
	void sendInt(int) const;
	void sendIntParameter(int) const;
	void sendStringParameter(const std::string&) const;
	Protocol recvCode() const;
	int recvInt() const;
	int recvIntParameter() const;
	std::string recvStringParameter() const;
private:
	std::shared_ptr<Connection> conn;
	void sendByte(unsigned char) const;
	unsigned char recvByte() const;
};

#endif
