#include <algorithm> // std::sort

#include "inmemorydb.h"

std::vector<std::pair<id_t,string>> InMemoryDB::list_news_groups()
{
	std::vector<std::pair<id_t, string>> list;
	for (auto kv : newsGroups)
	{
		list.emplace_back(kv.first, kv.second.name);
	}
	
	std::sort(list.begin(), list.end());
	return list;
}

bool InMemoryDB::create_news_group(string name)
{
	//Have to find by name instead of id_nbr
	auto kv = newsGroups.begin();
	while (kv != newsGroups.end() && kv->second.name != name)
	{
		kv++;
	}
	if (kv != newsGroups.end()) // If news group already existed, return false
	{
		return false;
	}
	// Create news group and return true
	next_id++;
	newsGroups.emplace(next_id, NewsGroup(name, next_id));
	return true;
}

bool InMemoryDB::delete_news_group(id_t id_nbr)
{
	return newsGroups.erase(id_nbr) == 1;
}

std::vector<std::pair<id_t, string>> InMemoryDB::list_articles(id_t id_nbr)
{
	std::vector<std::pair<id_t, string>> list;
	try // try to get news group and collect all its articles
	{
		NewsGroup ng = newsGroups.at(id_nbr);
		for (auto kv : ng.articles)
		{
			list.emplace_back(kv.first, kv.second.title);
		}
	}
	catch (const std::out_of_range& oor)
	{
		throw NoSuchNewsGroupException();
	}
	std::sort(list.begin(), list.end());
	return list;
}

void InMemoryDB::create_article(id_t id_nbr, string title, string author, string text)
{
	// Check that news group exists
	if (newsGroups.count(id_nbr) == 0)
	{
		throw NoSuchNewsGroupException();
	}
	// Add article
	newsGroups.at(id_nbr).addArticle(title, author, text);
}

void InMemoryDB::delete_article(id_t ng_id, id_t art_id)
{
	// Check that news group exists
	if (newsGroups.count(ng_id) == 0)
	{
		throw NoSuchNewsGroupException();
	}
	// Check that article exists
	if (newsGroups.at(ng_id).articles.count(art_id) == 0)
	{
		throw NoSuchArticleException();
	}
	// Erase article
	auto result = newsGroups.at(ng_id).articles.erase(art_id);
	// Check that operation succeeded
	if (result == 0)
	{
		throw std::runtime_error("Unable to delete article");
	}
}

std::vector<string> InMemoryDB::get_article(id_t ng_id, id_t art_id)
{
	// Check that news group exists
	if (newsGroups.count(ng_id) == 0)
	{
		throw NoSuchNewsGroupException();
	}
	// Check that article exists
	if (newsGroups.at(ng_id).articles.count(art_id) == 0)
	{
		throw NoSuchArticleException();
	}
	// Get article
	Article a = newsGroups.at(ng_id).articles.at(art_id);
	std::vector<string> list;
	list.push_back(a.title);
	list.push_back(a.author);
	list.push_back(a.text);
	return list;
}
