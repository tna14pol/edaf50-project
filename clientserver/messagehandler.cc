#include "connection.h"
#include <memory>
#include <string>
#include "messagehandler.h"

MessageHandler::MessageHandler(std::shared_ptr<Connection> connection)
{
	conn = connection;
}

void MessageHandler::sendCode(int) const {}

void MessageHandler::sendInt(int) const {}

void MessageHandler::sendIntParameter(int) const {}

void MessageHandler::sendStringParameter(const std::string&) const {}

unsigned int MessageHandler::recvCode() const
{
	return 0;
}

int MessageHandler::recvInt() const
{
	unsigned char byte1 = recvByte();
	unsigned char byte2 = recvByte();
	unsigned char byte3 = recvByte();
	unsigned char byte4 = recvByte();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int MessageHandler::recvIntParameter() const
{
	return 0;
}

std::string MessageHandler::recvStringParameter() const
{
	return "unimplemented";
}

void MessageHandler::sendByte(unsigned char) const {}

unsigned char MessageHandler::recvByte() const
{
	return conn->read();
}
