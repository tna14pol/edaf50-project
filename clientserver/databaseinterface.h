/*
 * Interface NameServerInterface -- all name server implementations must
 * implement this interface.
 */
#ifndef DATABSE_INTERFACE_H
#define DATABSE_INTERFACE_H


#include <string>
#include <vector>
#include <tuple>
#include <utility> // std::pair

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

enum class StatusCode {
	OK                 = 0,	// News group and article exists
	NO_SUCH_NEWS_GROUP = 1,	// News group does not exist
	NO_SUCH_ARTICLE    = 2	// News group exists but does not have the article
};

class DatabaseInterface {
public:
	virtual ~DatabaseInterface() = default;
	
	/*
	 * List newsgroups
	 * 
	 * Returns a vector containing pairs of the
	 * identification numbers and titles of the groups
	 */
	virtual std::vector<std::pair<const id_t, string>> list_news_groups() = 0;
	
	/*
	 * Create a newsgroup
	 * 
	 * Returns false if a news group by that name
	 * already exists; otherwise true
	 */
	virtual bool create_news_group(string) = 0;
	 
	/*
	 * Delete a newsgroup
	 * 
	 * Returns false if a news group by that name
	 * did not exist; otherwise true
	 */
	virtual bool delete_news_group(id_t) = 0;
	 
	/*
	 * List articles in a newsgroup
	 * 
	 * Returns a vector containing pairs of the
	 * identification numbers and titles of the articles
	 * 
	 * If the specified news group does not exist,
	 * returns NULL
	 */
	virtual std::vector<std::pair<const id_t, string>> list_articles(id_t) = 0;
	 
	/*
	 * Create an article
	 * 
	 * Returns false if news group does not exist; otherwise true
	 */
	virtual bool create_article(id_t, string, string, string) = 0;
	 
	/*
	 * Delete an article
	 * 
	 * Returns NO_SUCH_NEWS_GROUP if the news group does
	 * not exist; NO_SUCH_ARTICLE if the news group exists
	 * but does not contain the article; otherwise OK
	 */
	virtual StatusCode delete_article(id_t, id_t) = 0;
	 
	/*
	 * Get an article
	 * 
	 * Returns a tuple containing a status code and the title,
	 * author, and text of the article.
	 * 
	 * If the news group does not exist, the status code is NO_SUCH_NEWS_GROUP.
	 * If the news group exists but not the article, the status code is NO_SUCH_ARTICLE.
	 * Otherwise the status code is OK;
	 */
	virtual std::tuple<StatusCode, string, string, string> get_article(id_t, id_t) = 0;

};

#endif
