#include <algorithm> // std::find

#include "inmemorydb.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers


NewsGroup* InMemoryDB::findNewsGroup(id_t id)
{
	return &(newsGroups.at(id));
}

std::vector<std::pair<const id_t,string>> InMemoryDB::list_news_groups()
{
	std::vector<std::pair<const id_t, string>> list;
	for (auto kv : newsGroups)
	{
		list.emplace_back(kv.first, kv.second.name);
	}
	return list;
}

bool InMemoryDB::create_news_group(string name)
{
	//Have to find by name instead of id_nbr
	auto kv = newsGroups.begin();
	while (kv != newsGroups.end() && kv->second.name == name)
	{
		kv++;
	}
	if (kv != newsGroups.end())
	{
		return false;
	}
	next_id++;
	newsGroups.emplace(next_id, NewsGroup(name, next_id));
	return true;
}

bool InMemoryDB::delete_news_group(id_t)
{
	return false;
}

std::vector<std::pair<const id_t, string>> InMemoryDB::list_articles(id_t id_nbr)
{
	NewsGroup* ng = findNewsGroup(id_nbr);
	std::vector<std::pair<const id_t, string>> list;
	if (ng == nullptr)
	{
		return list;
	}
	for (auto kv : ng->articles)
	{
		list.emplace_back(kv.first, kv.second.title);
	}
	return list;
}
/*
Changed InMemoryDB to match change of implementation in NewsGroup
regarding how articles are saved.


*/
bool InMemoryDB::create_article(id_t, string, string, string)
{
	return false;
}

StatusCode InMemoryDB::delete_article(id_t, id_t)
{
	return StatusCode::NO_SUCH_NEWS_GROUP;
}

std::tuple<StatusCode, string, string, string> InMemoryDB::get_article(id_t ng_id, id_t art_id)
{
	NewsGroup* ng = findNewsGroup(ng_id);
	if (ng == nullptr)
	{
		return std::make_tuple(StatusCode::NO_SUCH_NEWS_GROUP, "no title", "no author", "no text");
	}
	const Article* a = ng->getArticle(art_id);
	if (a == nullptr)
	{
		return std::make_tuple(StatusCode::NO_SUCH_ARTICLE, "no title", "no author", "no text");
	}
	return std::make_tuple(StatusCode::OK, a->title, a->author, a->text);
}
