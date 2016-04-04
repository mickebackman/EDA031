#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <map>
#include "article.h"

class NewsGroup {

	public:
	NewsGroup(std::string name = "", int id = -1) : groupName(name), groupId(id), nextArticleId(0){}
	NewsGroup(const NewsGroup &n) : groupName(n.groupName), groupId(n.groupId), nextArticleId(n.nextArticleId){}
	// Creates an new article and adds it to the list. Throws runtime_error upon error inserting article.
	void addArticle(std::string name, std::string author, std::string text);

	// Deletes an article with the articleId Id
	bool deleteArticle(int id);
	Article getArticle(size_t id);
	int getId() {return groupId;}
	std::string getName(){return groupName;}
	// NewsGroup& operator=(NewsGroup&& rhs);
	NewsGroup& operator=(NewsGroup& rhs);

	// Returns the set of articles
	std::map<int, Article> getArticles() {return articles;}

	private:
	std::string groupName;
	int groupId;
	int nextArticleId;
	std::map<int, Article> articles;
};

bool operator<(NewsGroup lhs, NewsGroup rhs){
	return lhs.getId() < rhs.getId();
}
bool operator==(NewsGroup lhs, std::string rhs){
	return lhs.getName() == rhs;
}
bool operator==(NewsGroup lhs, int rhs){
	return lhs.getId() == rhs;
}

#endif
