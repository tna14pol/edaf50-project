/*
 * Interface NameServerInterface -- all name server implementations must
 * implement this interface.
 */
#ifndef DATABSE_INTERFACE_H
#define DATABSE_INTERFACE_H


#include <string>
#include <vector>
#include <utility> // std::pair

using std::string;
using id_t = unsigned int; // Alias to use for identification numbers

class DatabaseInterface {
public:
	virtual ~DatabaseInterface() = default;
	
	/*
	 * List newsgroups
	 * 
	 * Returns a vector containing pairs of the
	 * identification numbers and titles of the groups
	 */
	virtual std::vector<std::pair<id_t,string>> list_news_groups() = 0;
	
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
	 * The returned string contains either
	 *    "ANS_ACK"
	 * or
	 * 	"ANS_NAK ERR_NG_DOES_NOT_EXIST"
	 */
	virtual void delete_news_group(id_t) = 0;
	 
	/*
	 * List articles in a newsgroup
	 * 
	 * The returned string contains
	 * 	the number of articles followed by the identification
	 * 	numbers and titles of the articles
	 * or
	 * 	"ANS_NAK ERR_NG_DOES_NOT_EXIST"
	 * 
	 */
	virtual void list_articles(id_t) = 0;
	 
	/*
	 * Create an article
	 * 
	 * The returned string contains either
	 *    "ANS_ACK"
	 * or
	 * 	"ANS_NAK ERR_NG_DOES_NOT_EXIST"
	 */
	virtual void create_article(id_t, string, string, string) = 0;
	 
	/*
	 * Delete an article
	 * 
	 * The returned string contains either
	 *    "ANS_ACK"
	 * or
	 * 	"ANS_NAK ERR_NG_DOES_NOT_EXIST"
	 * or
	 * 	"ANS_NAK ERR_ART_DOES_NOT_EXIST"
	 */
	virtual void delete_article(id_t, id_t) = 0;
	 
	/*
	 * Get an article
	 * 
	 * The returned string contains either
	 *    "ANS_ACK" followed by the title, author and text of the article
	 * or
	 * 	"ANS_NAK ERR_NG_DOES_NOT_EXIST"
	 * or
	 * 	"ANS_NAK ERR_ART_DOES_NOT_EXIST"
	 */
	virtual void get_article(id_t, id_t) = 0;

};

#endif
