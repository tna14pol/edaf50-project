#include "messagehandler.h"
#include "databaseinterface.h"
#include "inmemorydb.h"

void protocolViolation(std::string);

void list_news_groups(MessageHandler*);

void create_news_group(MessageHandler*);

void delete_news_group(MessageHandler*);

void list_articles(MessageHandler*);

void create_article(MessageHandler*);

void delete_article(MessageHandler*);

void get_article(MessageHandler*);
