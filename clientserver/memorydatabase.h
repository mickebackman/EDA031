#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include <map>
#include <string>
#include "database.h"
#include "newsgroup.h"
#include "article.h"
#include <vector>

 class MemoryDatabase : public Database{
public:
   MemoryDatabase(): Database(0){}
   void addArticle(int newsGroupId, std::string name, std::string author, std::string text);
   void addNewsGroup(std::string newsGroupName);
   Article getArticle(int newsGroupId, int articleId);
   void deleteArticle(int newsGroupId, int articleId);
   void deleteNewsGroup(int newsGroupId);
   std::vector<std::pair<int, std::string> > getNewsGroups();
   std::map<int, Article> getArticlesInNewsGroup(int newsGroupId);
   int numberOfNewsGroups();
   int numberOfArticlesInNewsGroup(int newsGroupId);

 private:
   // A map containing all the news groups with id as key.
   std::map<int, NewsGroup> groups;

};

#endif
