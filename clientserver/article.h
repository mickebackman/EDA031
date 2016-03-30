#ifndef ARTICLE_H
#define ARTICLE_H

class Article {

	public:
	//Creates an new article
	Article(std::string name, std::string author, std::string text, int id) : articleName(name), articleAuthor(author), articleText(text), articleId(id){}

	//Returns the articleId
	int getId() {return articleId};
	std::string getAuthor() {return articleAuthor;}
	std::string getText() {return articleText;}
	std::string getName() {return articleName;}

	private:
	std::string articleName;
	std::string articleAuthor;
	std::string articleText;
	int articleId;
};

bool operator<(Article lhs, Article rhs){
	return lhs.getId() < rhs.getId();
}
bool operator==(Article lhs, Article rhs){
	return lhs.getId() == rhs.getId();
}
bool operator==(Article lhs, int rhs){
	return lhs.getId() == rhs;
}

#endif