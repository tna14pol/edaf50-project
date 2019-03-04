#include "newsgroup.h"
#include <algorithm> //std::find_if

NewsGroup::NewsGroup(string name, id_t id_nbr)
: id_nbr(id_nbr), name(name), next_article_nbr(0)
{
	articles = std::vector<Article>();
}

bool NewsGroup::addArticle(string, string, string)
{
	return false;
}

const Article* NewsGroup::getArticle(id_t id) const
{
	auto p = std::find_if(articles.begin(), articles.end(),
		[&] (Article art)
		{
			return art.id_nbr == id;
		});
	return (p != articles.end()) ? nullptr : &*p;
}

bool NewsGroup::removeArticle(id_t)
{
	return false;
}
