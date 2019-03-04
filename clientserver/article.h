#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

using std::string;
using id_t = unsigned int;

class Article {
public:
	Article()=delete;
	Article(string, string, string, id_t);
	string title;
	string author;
	string text;
	id_t id_nbr;
};

#endif
