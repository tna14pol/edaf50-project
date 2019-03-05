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
	 * The articles gets the its identification number
	 * from member variable next_article_nbr,
	 * which is incremented before being used in
	 * this function.
	 */
	void addArticle(string, string, string);
	const id_t id_nbr;
	const string name;
	std::unordered_map<id_t, const Article> articles;
private:
	// Used to assign identification numbers to new articles
	id_t next_article_nbr;
};
#endif
