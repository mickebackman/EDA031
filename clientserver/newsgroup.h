#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <set>
#include "article.h"

class NewsGroup {
	public:
	NewsGroup(std::string name, int id) : groupName(name), groupId(id), nextArticleId(0){}

	// Creates an new article and adds it to the list. Throws runtime_error upon error inserting article.
	void addArticle(std::string name, std::string author, std::string text);

	// Deletes an article with the articleId Id
	bool deleteArticle(int id);
	int getId() {return groupId;}

	// Returns the set of articles
	set<Article> getArticles() {return articles;}

	private:
	std::string groupName;
	int nextArticleId;
	int groupId;
	set<Article> articles;
};

bool operator<(NewsGroup lhs, NewsGroup rhs){
	return lhs.getId() < rhs.getId();
}
bool operator==(NewsGroup lhs, NewsGroup rhs){
	return lhs.getId() == rhs.getId();
}
#endif