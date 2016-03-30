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
   // Throws 0 if the group does not exist and 1 if the article does not exist
   virtual Article getArticle(int newsGroupId, int articleId);
   // Deletes the specified article from the specified group. 
   // Throws 0 if the group does not exist and 1 if the article does not exist
   virtual void deleteArticle(int newsGroupId, int articleId);
   // deletes the specified group.
   virtual void deleteNewsGroup(int newsGroupId);
   // Returns a string representation of all of the groups.
   virtual std::vector<pair<int, std::string> getNewsGroups();
   // Returns a string representation of all the articles in the specified group.
   virtual std::set<Article> getArticlesInNewsGroup(int newsGroupId);
   //Returns the number of news groups.
   virual int numberOfNewsGroups();
   //Return number of articles in the specified news group.
   virtual int numberOfArticlesInNewsGroup(int newsGroupId);


 private:
   // The id for the group that is added next.
   int nextGroupId;

};

#endif
