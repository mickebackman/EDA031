#ifndef ARTICLE_H
#define ARTICLE_H

class Article {

	public:
	//Creates an new article
	Article(std::string name, std::string author, std::string text, int id) : articleName(name), articleAuthor(author), articleText(text), articleId(id){}
	Article(const Article& art) : articleName(art.articleName),  articleAuthor(art.articleAuthor), articleText(art.articleText), articleId(art.articleId){}
	//Returns the articleId
	int getId() const {return articleId;};
	std::string getAuthor() const {return articleAuthor;}
	std::string getText() const {return articleText;}
	std::string getName() const {return articleName;}
	//copy assignment operator.
	Article& operator=(const Article& rhs){
		articleName = rhs.articleName;
		articleAuthor = rhs.articleAuthor;
		articleText = rhs.articleText;
		articleId = rhs.articleId;
		return *this;
	}
	/*
	Move operator.
	*/
	Article& operator=(const Article&& rhs){
		articleName = rhs.articleName;
		articleAuthor = rhs.articleAuthor;
		articleText = rhs.articleText;
		articleId = rhs.articleId;
		return *this;
	}

	private:
	std::string articleName;
	std::string articleAuthor;
	std::string articleText;
	int articleId;
};

#endif
