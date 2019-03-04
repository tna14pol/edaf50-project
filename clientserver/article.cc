#include "article.h"

Article::Article(string title, string author, string text, id_t id_nbr)
{
	this->title = title;
	this->author = author;
	this->text = text;
	this->id_nbr = id_nbr;
}
