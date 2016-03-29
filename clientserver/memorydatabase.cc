
using namespace std;

void MemoryDatabase::addArticle(int newsGroupId, string name, string author, string text){
  auto it = groups.find(newsGroupName);
  if(it == groups.end()){
    throw runtime_error("The news group does not exist!");
  }
  try{
   it->addArticle(name, author, text);
 }catch(...){
   throw;
 }
}
void MemoryDatabase::addNewsGroup(string newsGroupName){
    auto p = groups.insert(NewsGroup(nextGroupId, newsGroupName));
    if(!p.second){
      throw runtime_error("The group already exists!");
    }
    ++nextGroupId;
}

Article MemoryDatabase::getArticle(int newsGroupId, int articleId){
  auto it = groups.find(newsGroupId);
  if(it == groups.end()){
      throw runtime_error("The group does not exist!");
  }
  try{
    return it->getArticle(articleId);
  }catch(...){
    throw;
  }
}
void MemoryDatabase::deleteArticle(int newsGroupId, int articleId){
  auto it = groups.find(newsGroupId);
  if(it == groups.end()){
    throw runtime_error("The group does not exist!");
  }

  if(!(it->deleteArticle(articleId))){
    throw runtime_error("The article does not exist!");
  }
}

void MemoryDatabase::deleteNewsGroup(int newsGroupId){
  auto it = groups.find(newsGroupId);
  if(it == groups.end()){
    throw runtime_error("The group does not exist!");
  }
  groups.erase(it);
}
string MemoryDatabase::listNewsGroups();
string MemoryDatabase::listArticlesInNewsGroup(int newsGroupId);
