#ifndef DATABASE_H
#define DATABASE_H
#include "article.h"
#include <vector>
#include <map>
#include <string>

//Interface for the different database classes.

 class Database{
public:
   Database(int id = 0) : nextGroupId(id) {}
  // Adds an article to the database.
   virtual void addArticle(int newsGroupId, std::string name, std::string author, std::string text) = 0;
   // Adds a news group to the database.
   virtual void addNewsGroup(std::string newsGroupName) = 0;
   // Returns the article with articleId in the specified group.
   // Throws 0 if the group does not exist and 1 if the article does not exist
   virtual Article getArticle(int newsGroupId, int articleId) = 0;
   // Deletes the specified article from the specified group.
   // Throws 0 if the group does not exist and 1 if the article does not exist
   virtual void deleteArticle(int newsGroupId, int articleId) = 0;
   // deletes the specified group.
   virtual void deleteNewsGroup(int newsGroupId) = 0;
   // Returns a string representation of all of the groups.
   virtual std::vector<std::pair<int, std::string> > getNewsGroups() = 0;
   // Returns a string representation of all the articles in the specified group.
   virtual std::map<int, Article> getArticlesInNewsGroup(int newsGroupId) = 0;
   //Returns the number of news groups.
   virtual int numberOfNewsGroups() = 0;
   //Return number of articles in the specified news group.
   virtual int numberOfArticlesInNewsGroup(int newsGroupId) = 0;


 protected:
   // The id for the group that is added next.
   int nextGroupId;

};

#endif
