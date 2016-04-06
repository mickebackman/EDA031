#ifndef DISKDATABASE_H
#define DISKDATABASE_H

#include <map>
#include <string>
#include "database.h"
#include "newsgroup.h"
#include "article.h"
#include <vector>
#include <stdlib.h>
#include <dirent.h>

//Every directory includes a meta file. In the root directory the file includes
//the next group id followed by the number of news groups. In the news groups it
// includes the next article id, the number of articles and the name of the news group.

class DiskDatabase : public Database{

public:
   DiskDatabase();
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
   // The root directory.
   string root;
   int numberOfNewsGroups;

};

#endif
