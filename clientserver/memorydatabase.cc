#include "memorydatabase.h"
#include <string>
#include <iostream>
#include "article.h"
#include "newsgroup.h"
#include <vector>
#include <map>

using namespace std;

void MemoryDatabase::addArticle(int newsGroupId, string name, string author, string text){
  // FEL SOM FAN. Lös detta pls
  try{
  groups.at(newsGroupId).addArticle(name, author, text);
 }catch(...){
   // Group does not exist
   throw 0;
 }

}


void MemoryDatabase::addNewsGroup(string newsGroupName){
    auto p = groups.insert(make_pair(nextGroupId, NewsGroup(newsGroupName, nextGroupId)));
    if(!p.second){
      throw runtime_error("The group already exists!");
    }
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
  try{
    g.deleteArticle(articleId);
  }catch(exception& e){
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
set<Article> MemoryDatabase::getArticlesInNewsGroup(int newsGroupId){
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

int numberOfArticlesInNewsGroup(int newsGroupId){
  // NewsGroup g;
  // try{
  //   g = groups.at(newsGroupId);
  // }catch(...){
  //   // group does not exist - 0
  //   throw 0;
  // }
  // return g.getArticles().size();
  return getArticlesInNewsGroup(newsGroupId).size();
}
