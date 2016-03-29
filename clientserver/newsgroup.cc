#include <string>
#include <set>
#include <algorithm>


using namespace std;

bool
NewsGroup::addArticle(std::string name, std::string author, std::string text){
	auto it = articles.insert(Article(name, author,text, nextArticleId));
	if (!it.second){
		throw runtime_error("Could not insert article by " + author + " in group: " + groupName);
		return false;
	}
	++nextArticleId;
	return true;
}

bool NewsGroup::deleteArticle(int id){
	return (articles.erase(remove_if(articles.begin(), articles.end(), 
		( [] (int articleId) {return id == articleId;}))) > 0);
}
