#include "newsgroup.h"
#include <algorithm> //std::find_if

NewsGroup::NewsGroup(string name, id_t id_nbr)
: id_nbr(id_nbr), name(name), next_article_nbr(0)
{
	articles = std::unordered_map<id_t, const Article>();
}

void NewsGroup::addArticle(string title, string author, string text)
{
	next_article_nbr++;
	articles.emplace(next_article_nbr, Article(title, author, text, next_article_nbr));
}
