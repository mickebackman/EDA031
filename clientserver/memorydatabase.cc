#include "memorydatabase.h"
#include <string>
#include <iostream>
#include "article.h"
#include "newsgroup.h"
#include <vector>
#include <map>

using namespace std;

void MemoryDatabase::addArticle(int newsGroupId, string name, string author, string text){
  try{
  groups.at(newsGroupId).addArticle(name, author, text);
 }catch(...){
   // Group does not exist
   throw 0;
 }

}


void MemoryDatabase::addNewsGroup(string newsGroupName){
    for(auto p : groups){
      if(p.second.getName() == newsGroupName){
        throw runtime_error("The group already exists!");
      }
    }
    groups.insert(make_pair(nextGroupId, NewsGroup(newsGroupName, nextGroupId)));
    ++nextGroupId;
}

Article MemoryDatabase::getArticle(int newsGroupId, int articleId){
  NewsGroup g;
  try{
    g = groups.at(newsGroupId);
  }catch(...){
    // group does not exist - 0
    throw 0;
  }
  try{
    return g.getArticle(articleId);
  }catch(exception& e){
    throw 1;
  }

}

void MemoryDatabase::deleteArticle(int newsGroupId, int articleId){

  NewsGroup g;
  try{
    g = groups.at(newsGroupId);
  }catch(...){
    // group does not exist - 0
    throw 0;
  }
  if(!g.deleteArticle(articleId)){
    throw 1;
  }

  }



void MemoryDatabase::deleteNewsGroup(int newsGroupId){
  auto it = groups.find(newsGroupId);
  if(it == groups.end()){
    throw runtime_error("The group does not exist!");
  }
  groups.erase(it);
}
vector<pair<int, string>> MemoryDatabase::getNewsGroups(){
  vector<pair<int, string>> result;

  for (auto p : groups){
      result.push_back(make_pair(p.first, p.second.getName()));
  }
  return result;
}
map<int, Article> MemoryDatabase::getArticlesInNewsGroup(int newsGroupId){
  NewsGroup g;
  try{
    g = groups.at(newsGroupId);
  }catch(...){
    // group does not exist - 0
    throw 0;
  }
  return g.getArticles();
}

int MemoryDatabase::numberOfNewsGroups(){
  return groups.size();
}

int MemoryDatabase::numberOfArticlesInNewsGroup(int newsGroupId){
  try{
  return getArticlesInNewsGroup(newsGroupId).size();
  }catch(...){
    throw 0;
  }
}
