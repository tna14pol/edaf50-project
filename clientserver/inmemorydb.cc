#include <algorithm> // std::find

#include "inmemorydb.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers
/*
InMemoryDB::InMemoryDB()
{
	next_id = 0;
	newsGroups = vector();
}
*/
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

void InMemoryDB::delete_news_group(id_t)
{

}

void InMemoryDB::list_articles(id_t)
{

}

void InMemoryDB::create_article(id_t, string, string, string)
{

}

void InMemoryDB::delete_article(id_t, id_t)
{

}

void InMemoryDB::get_article(id_t, id_t)
{

}
