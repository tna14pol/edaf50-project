#include "connection.h"
#include <memory>
#include <string>
#include <iostream>
#include "messagehandler.h"
#include "protocol.h"


void wrongCodeError(std::string function, Protocol expected, Protocol received)
{
	std::cerr << "Error in MessageHandler: " << function << ": expected " << static_cast<int>(expected) << ", got " << static_cast<int>(received) << std::endl;
}

MessageHandler::MessageHandler(std::shared_ptr<Connection> connection)
{
	conn = connection;
}

void MessageHandler::sendCode(Protocol code) const
{
	sendByte(static_cast<unsigned char>(code));
}

void MessageHandler::sendInt(int value) const
{
	sendByte((value >> 24) & 0xFF);
	sendByte((value >> 16) & 0xFF);
	sendByte((value >> 8) & 0xFF);
	sendByte(value & 0xFF);
}

void MessageHandler::sendIntParameter(int param) const
{
	sendCode(Protocol::PAR_NUM);
	sendInt(param);
}

void MessageHandler::sendStringParameter(const std::string&/* param*/) const
{}

Protocol MessageHandler::recvCode() const
{
	Protocol code = static_cast<Protocol>(recvByte());
	return code;
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
	Protocol code = recvCode();
	if (code != Protocol::PAR_NUM) {
		wrongCodeError("recvIntParameter()", Protocol::PAR_NUM, code);

	}
	return recvInt();
}

std::string MessageHandler::recvStringParameter() const
{
	return "unimplemented";
}

void MessageHandler::sendByte(unsigned char code) const
{
	conn->write(code);
}

unsigned char MessageHandler::recvByte() const
{
	return conn->read();
}

