#include "newsgroup.h"


NewsGroup::NewsGroup(string name, id_t id_nbr)
{
	this->name = name;
	this->id_nbr = id_nbr;
	next_article_nbr = 0;
}

bool NewsGroup::addArticle(string, string, string)
{
	return false;
}

Article* NewsGroup::getArticle(id_t) const
{
	return nullptr;
}

bool NewsGroup::removeArticle(id_t)
{
	return false;
}
