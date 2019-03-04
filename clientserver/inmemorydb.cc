#include <algorithm> // std::sort
#include <iostream>

#include "inmemorydb.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers


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
	if (kv != newsGroups.end())
	{
		return false;
	}
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
	NewsGroup ng = newsGroups.at(id_nbr);
	std::vector<std::pair<id_t, string>> list;
	for (auto kv : ng.articles)
	{
		list.emplace_back(kv.first, kv.second.title);
	}
	
	std::sort(list.begin(), list.end());
	return list;
}

void InMemoryDB::create_article(id_t id_nbr, string title, string author, string text)
{
	newsGroups.at(id_nbr).addArticle(title, author, text);
}

void InMemoryDB::delete_article(id_t ng_id, id_t art_id)
{
	try
	{
		if (newsGroups.at(ng_id).articles.erase(art_id) == 0)
		{
			throw std::out_of_range("article");
		}
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "No such news group: " << oor.what() << "\n";
		throw std::out_of_range("news group");
	}
}

std::tuple<string, string, string> InMemoryDB::get_article(id_t ng_id, id_t art_id)
{
	try
	{
		NewsGroup ng = newsGroups.at(ng_id);
		
		try
		{
			Article a = ng.articles.at(art_id);
			return std::make_tuple(a.title, a.author, a.text);
		}
		catch (const std::out_of_range& oor)
		{
			std::cerr << "No such article: " << oor.what() << "\n";
			throw std::out_of_range("article");
		}
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "No such news group: " << oor.what() << "\n";
		throw std::out_of_range("news group");
	}
}
