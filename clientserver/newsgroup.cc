#include "newsgroup.h"
#include <algorithm> //std::find_if

NewsGroup::NewsGroup(string name, id_t id_nbr)
: id_nbr(id_nbr), name(name), next_article_nbr(0)
{
	articles = std::unordered_map<id_t, const Article>();
}

bool NewsGroup::addArticle(string, string, string)
{
	return false;
}
