#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include <set>
#include <string>
#include "newsgroup.h"
#include "article.h"

 class MemoryDatabase{
public:
   MemoryDatabase(): nextGroupId(0){};
   void addArticle(int newsGroupName, std::string name, std::string author, std::string text);
   void addNewsGroup(std::string newsGroupName);
   Article getArticle(int newsGroupId, int articleId);
   void deleteArticle(int newsGroupId, int articleId);
   void deleteNewsGroup(int newsGroupId);
   std::string listNewsGroups();
   std::string listArticlesInNewsGroup(int newsGroupId);

 private:
   int nextGroupId;
   // A set containing all the news groups. 
   std::set<NewsGroup> groups;

};

#endif
