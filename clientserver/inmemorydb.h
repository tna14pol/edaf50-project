#ifndef IN_MEMORY_DB_H
#define IN_MEMORY_DB_H

#include <unordered_map>

#include "databaseinterface.h"
#include "newsgroup.h"
#include "article.h"

class InMemoryDB :public DatabaseInterface {
public:
	std::vector<std::pair<id_t,string>> list_news_groups() override;
	bool create_news_group(string) override;
	bool delete_news_group(id_t) override;
	std::vector<std::pair<id_t, string>> list_articles(id_t) override;
	void create_article(id_t, string, string, string) override;
	void delete_article(id_t, id_t) override;
	std::vector<string> get_article(id_t, id_t) override;
private:
	std::unordered_map<id_t, NewsGroup> newsGroups;
	// next_id is used to assign identification numbers to new news groups
	id_t next_id;
};

#endif
