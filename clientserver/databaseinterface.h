/*
 * Interface DatabaseInterface -- all database implementations must
 * implement this interface.
 */
#ifndef DATABSE_INTERFACE_H
#define DATABSE_INTERFACE_H

#include <string>
#include <vector>
#include <tuple>
#include <utility> // std::pair

#include "nosuchnewsgroupexception.h"
#include "nosucharticleexception.h"

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class DatabaseInterface {
public:
	virtual ~DatabaseInterface() = default;
	
	/*
	 * List newsgroups
	 * 
	 * Returns a vector containing pairs of the
	 * identification numbers and titles of the groups.
	 * 
	 * The vector is sorted using the id numbers.
	 * 
	 */
	virtual std::vector<std::pair<id_t, string>> list_news_groups() = 0;
	
	/*
	 * Create a newsgroup
	 * 
	 * Returns false if a news group by that name
	 * already exists; otherwise true.
	 */
	virtual bool create_news_group(string) = 0;
	 
	/*
	 * Delete a newsgroup
	 * 
	 * Returns false if a news group by that name
	 * did not exist; otherwise true.
	 */
	virtual bool delete_news_group(id_t) = 0;
	 
	/*
	 * List articles in a newsgroup
	 * 
	 * Returns a vector containing pairs of the
	 * identification numbers and titles of the articles.
	 * 
	 * The vector is sorted using the id numbers.
	 * 
	 * If the specified news group does not exist,
	 * throws a NoSuchNewsGroupException.
	 */
	virtual std::vector<std::pair<id_t, string>> list_articles(id_t) = 0;
	 
	/*
	 * Create an article
	 * 
	 * If the specified news group does not exist,
	 * throws a NoSuchNewsGroupException.
	 */
	virtual void create_article(id_t, string, string, string) = 0;
	 
	/*
	 * Delete an article
	 * 
	 * If the specified news group does not exist,
	 * throws a NoSuchNewsGroupException.
	 
	 * If the news group exists but not the article,
	 * throws a NoSuchArticleException.
	 */
	virtual void delete_article(id_t, id_t) = 0;
	 
	/*
	 * Get an article
	 * 
	 * Returns a tuple containing the title, author, and text of the article.
	 * 
	 * If the specified news group does not exist,
	 * throws a NoSuchNewsGroupException.
	 
	 * If the news group exists but not the article,
	 * throws a NoSuchArticleException.
	 */
	virtual std::tuple<string, string, string> get_article(id_t, id_t) = 0;

};

#endif
