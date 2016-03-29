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
	return (articles.erase(remove_if(articles.begin(), articles.end(), 
		( [] (int articleId) {return id == articleId;}))) > 0);
}

Article NewsGroup::getArticle(int id){
	auto it = articles.find(id);
	if (it == articles.end()){
		throw runtime_error("Article could not be found");
	}
	else return *it;
}