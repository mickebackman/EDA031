#ifndef DATABASE_H
#define DATABASE_H


 class Database{
public:
   Database();
   virtual void addArticle(int newsGroupId, std::string name, std::string author, std::string text);
   virtual void addNewsGroup(std::string name);
   virtual Article getArticle(int newsGroupId, int articleId);
   virtual void deleteArticle(int newsGroupId, int articleId);
   virtual void deleteNewsGroup(int newsGroupId);
   virtual std::string listNewsGroups();
   virtual std::string listArticlesInNewsGroup( int newsGroupId);

 private:
   int nextGroupId;

};

#endif
