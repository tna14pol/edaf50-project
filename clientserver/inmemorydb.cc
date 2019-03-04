#include <algorithm> // std::find

#include "inmemorydb.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers


NewsGroup* InMemoryDB::findNewsGroup(id_t id_nbr)
{
	auto p = std::find_if(newsGroups.begin(), newsGroups.end(),
		[&] (NewsGroup ng)
		{
			return ng.id_nbr == id_nbr;
		});
	return (p != newsGroups.end()) ? nullptr : &*p;
}

std::vector<std::pair<id_t,string>> InMemoryDB::list_news_groups()
{
	std::vector<std::pair<id_t, string>> list;
	for (NewsGroup ng : newsGroups)
	{
		list.emplace_back(ng.id_nbr, ng.name);
	}
	return list;
}

bool InMemoryDB::create_news_group(string name)
{
	auto result = std::find_if(newsGroups.begin(), newsGroups.end(),
		[&] (NewsGroup ng)
		{
			return ng.name == name;
		});
	if (result != newsGroups.end())
	{
		return false;
	}
	next_id++;
	newsGroups.emplace_back(name, next_id);
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
