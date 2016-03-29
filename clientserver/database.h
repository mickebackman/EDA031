#ifndef DATABASE_H
#define DATABASE_H

//Interface for the different database classes.

 class Database{
public:
  // Adds an article to the database.
   virtual void addArticle(int newsGroupId, std::string name, std::string author, std::string text);
   // Adds a news group to the database.
   virtual void addNewsGroup(std::string newsGroupName);
   // Returns the article with articleId in the specified group.
   virtual Article getArticle(int newsGroupId, int articleId);
   // Deletes the specified article from the specified group.
   virtual void deleteArticle(int newsGroupId, int articleId);
   // deletes the specified group.
   virtual void deleteNewsGroup(int newsGroupId);
   // Returns a string representation of all of the groups.
   virtual std::string listNewsGroups();
   // Returns a string representation of all the articles in the specified group.
   virtual std::string listArticlesInNewsGroup(int newsGroupId);

 private:
   // The id for the group that is added next. 
   int nextGroupId;

};

#endif
