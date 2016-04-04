#include <string>
#include <set>
#include <algorithm>
#include "newsgroup.h"
#include "article.h"

using namespace std;

void
NewsGroup::addArticle(std::string name, std::string author, std::string text){
	auto it = articles.insert(Article(name, author,text, nextArticleId));
	if (!it.second){
		throw runtime_error("Could not insert article by " + author + " in group: " + groupName);
	}
	++nextArticleId;
}

bool NewsGroup::deleteArticle(int id){
	auto it = articles.end();
	return (articles.erase(remove_if(articles.begin(), articles.end(), 
		( [id] (const Article& art) {return id == art.getId();}))) != it);
}

Article NewsGroup::getArticle(size_t id){
	auto it = articles.find(Article("","","",id));
	if (it == articles.end()){
		throw runtime_error("Article could not be found");
	}
	else return *it;
}

// NewsGroup& NewsGroup::operator=(NewsGroup&& rhs){
// 	groupName =  std::move(rhs.getName());
// 	groupId =  std::move(rhs.groupId);
// 	nextArticleId =  std::move(rhs.nextArticleId);
// 	articles =  std::move(rhs.articles);
// 	delete rhs;
// 	return *this;
// }

NewsGroup& NewsGroup::operator=(NewsGroup& rhs){
	groupName = rhs.groupName;
	groupId = rhs.groupId;
	nextArticleId = rhs.nextArticleId;
	articles = rhs.articles;
	return *this;
}
