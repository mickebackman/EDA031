#include <string>
#include <map>
#include <algorithm>
#include "newsgroup.h"
#include "article.h"
#include <iostream>

using namespace std;

void
NewsGroup::addArticle(std::string name, std::string author, std::string text){
	auto it = articles.insert(make_pair(nextArticleId, Article(name, author,text, nextArticleId)));
	if (!it.second){
		throw runtime_error("Could not insert article by " + author + " in group: " + groupName);
	}
	++nextArticleId;
}

bool NewsGroup::deleteArticle(int id){
		return articles.erase(id);
}

Article NewsGroup::getArticle(size_t id){
	auto it = articles.find(id);
	if (it == articles.end()){
		throw runtime_error("Article could not be found");
	}
	else return (*it).second;
}

NewsGroup& NewsGroup::operator=(NewsGroup& rhs){
	groupName = rhs.groupName;
	groupId = rhs.groupId;
	nextArticleId = rhs.nextArticleId;
	articles = rhs.articles;
	return *this;
}
