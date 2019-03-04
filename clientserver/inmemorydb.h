#ifndef IN_MEMORY_DB_H
#define IN_MEMORY_DB_H

#include <string>
#include <vector>
#include <tuple>
#include <utility> // std::pair

#include "databaseinterface.h"
#include "newsgroup.h"
#include "article.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class InMemoryDB :public DatabaseInterface {
public:
//	InMemoryDB();
	std::vector<std::pair<id_t,string>> list_news_groups();
	bool create_news_group(string);
	bool delete_news_group(id_t);
	std::vector<std::pair<id_t, string>> list_articles(id_t);
	bool create_article(id_t, string, string, string);
	StatusCode delete_article(id_t, id_t);
	std::tuple<StatusCode, string, string, string> get_article(id_t, id_t);
private:
	NewsGroup* findNewsGroup(id_t);
	std::vector<NewsGroup> newsGroups;
	id_t next_id;
};

#endif
