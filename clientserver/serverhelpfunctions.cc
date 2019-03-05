#include "serverhelpfunctions.h"

InMemoryDB db = InMemoryDB();
/*
 * Throws a runtime exception
 */
void protocolViolation(std::string msg)
{
	throw std::runtime_error("Protocol violation: " + msg);
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
	catch (NoSuchNewsGroupException&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
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
	catch (NoSuchNewsGroupException&)
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
	catch (NoSuchNewsGroupException&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	catch (NoSuchArticleException&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
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
		mh->sendStringParameter(t.at(0));
		mh->sendStringParameter(t.at(1));
		mh->sendStringParameter(t.at(2));
	}
	catch (NoSuchNewsGroupException&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	catch (NoSuchArticleException&)
	{
		mh->sendCode(Protocol::ANS_NAK);
		mh->sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
	}
	mh->sendCode(Protocol::ANS_END);
}
