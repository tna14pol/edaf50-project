#ifndef IN_MEMORY_DB_H
#define IN_MEMORY_DB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <utility> // std::pair

#include "databaseinterface.h"
#include "newsgroup.h"
#include "article.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class InMemoryDB :public DatabaseInterface {
public:
	std::vector<std::pair<const id_t,string>> list_news_groups() override;
	bool create_news_group(string) override;
	bool delete_news_group(id_t) override;
	std::vector<std::pair<const id_t, string>> list_articles(id_t) override;
	bool create_article(id_t, string, string, string) override;
	StatusCode delete_article(id_t, id_t) override;
	std::tuple<StatusCode, string, string, string> get_article(id_t, id_t) override;
private:
	NewsGroup* findNewsGroup(id_t);
	std::unordered_map<id_t, NewsGroup> newsGroups;
	id_t next_id;
};

#endif
