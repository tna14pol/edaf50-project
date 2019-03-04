#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include <unordered_map>
#include "article.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class NewsGroup {
public:
	NewsGroup()=delete;
	NewsGroup(string, id_t);
	/* 
	 * Creates an article and adds it to this newsgroup
	 */
	void addArticle(string, string, string);
	const id_t id_nbr;
	const string name;
	std::unordered_map<id_t, const Article> articles;
//	std::vector<Article> articles;
private:
	id_t next_article_nbr;
};
#endif
