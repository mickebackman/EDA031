#ifndef ARTICLE_H
#define ARTICLE_H

class Article {

	public:
	//Creates an new article
	Article(std::string name, std::string author, std::string text, int id) : articleName(name), articleAuthor(author), articleText(text), articleId(id){}

	//Returns the articleId
	getId() {return articleId};

	private:
	std::string articleName;
	std::string articleAuthor;
	std::string articleText;
	int articleId;
};
