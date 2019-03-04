#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include "article.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class NewsGroup {
public:
	NewsGroup()=delete;
	NewsGroup(string, id_t);
	// Creates an article and adds it to this newsgroup
	bool addArticle(string, string, string);
	Article* getArticle(id_t) const;
	// Removes an article from this newsgroup
	bool removeArticle(id_t);
	string name;
	id_t id_nbr;
private:
	id_t next_article_nbr;
};
#endif
