#include "diskdatabase.h"
#include <string>
#include <iostream>
#include "article.h"
#include "newsgroup.h"
#include <vector>
#include <map>
#include <stdlib.h>
#include <dirent.h>

using namespace std;

DiskDatabase::DiskDatabase(){
  root("newsroot");
  int nextId = 0;
  int newsGroups = 0;
  if(mkdir(root) == 0){
    ofstream out(root+"//"+"meta");
    out << "0 " << "0" << endl;
    out.close();
  }else{
    ifstream in(root+"//"+"meta");
    in >> nextId >> newsGroups;
    in.close();
  }
  nbrOfNewsGroups = newsGroups;
  Database(nextId);

}

void DiskDatabase::addArticle(int newsGroupId, std::string name, std::string author, std::string text){
  DIR *dir = NULL;
  dir = opendir(root+"//"+to_string(newsGroupId));
  if(dir == NULL){
    // The news group does not exist.
    closedir(dir);
    throw 0;
  }
    closedir(dir);
    ifstream in(root+"//"+to_string(newsGroupId)+"meta");
    int nextId, nbrOfArticles;
    string groupName;
    in >> nextId >> nbrOfArticles >> groupName;
    ofstream out(root+"//"+to_string(newsGroupId)+"//"+nextId);
    out << name << endl << author << endl << text << endl;
    out.close();
    in.close();
    remove(root+"//"+to_string(newsGroupId)+"meta");
    outstream metaOut(root+"//"+to_string(newsGroupId)+"meta");
    metaOut << to_string(nextId + 1) << " " << to_string(nbrOfArticles + 1) << " " << groupName << endl;
    metaOut.close();
}


void DiskDatabase::addNewsGroup(std::string newsGroupName){
  auto v = getNewsGroups();
  for( auto p : v){
    if(p.second == newsGroupName){
      // The news group does already exist.
      throw runtime_error("The group already exists!");
    }
  }
  mkdir(root+"//"+to_string(nextGroupId));
  ofstream out(root+"//"+to_string(nextGroupId)+"//"+"meta");
  out << "0 " << "0 " << newsGroupName << endl;
  ++nextGroupId;
  ++nbrOfNewsGroups;
  remove(root+"//"+"meta");
  ofstream outMeta(root+"//"+"meta");
  outMeta << nextGroupId << " " << nbrOfNewsGroups;
  out.close();
  outMeta.close();
}

Article DiskDatabase::getArticle(int newsGroupId, int articleId){
  DIR *dir = NULL;
  dir = opendir(root+"//"+to_string(newsGroupId));
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  ifstream in(root+"//"+to_string(newsGroupId)+"//"+to_string(articleId));
  if(in.fail()){
    //The file does not exist.
    throw 1;
    }
  string name;
  string author;
  string text;
  getline(in, name);
  getline(in, author);
  getline(in, text);
  return Article(name, author, text, articleId);
}

void DiskDatabase::deleteArticle(int newsGroupId, int articleId){
  DIR *dir = NULL;
  dir = opendir(root+"//"+to_string(newsGroupId));
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  if(remove(root+"//"+to_string(newsGroupId)+"//"+to_string(articleId)) != 0){
    //The file does not exist.
    throw 1;
  }
  ifstream in(root+"//"+to_string(newsGroupId)+"meta");
  int nextId, nbrOfArticles;
  string groupName;
  in >> nextId >> nbrOfArticles >> groupName;
  remove(root+"//"+to_string(newsGroupId)+"meta");
  outstream metaOut(root+"//"+to_string(newsGroupId)+"meta");
  metaOut << nextId << " " << to_string(nbrOfArticles - 1) << " " << groupName << endl;
  metaOut.close();

}

void DiskDatabase::deleteNewsGroup(int newsGroupId){
  DIR *dir = NULL;
  dir = opendir(root+"//"+to_string(newsGroupId));
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  --nbrOfNewsGroups;
  system("rm -r "+root+"//"+to_string(newsGroupId));
  remove(root+"//"+"meta");
  ofstream outMeta(root+"//"+"meta");
  outMeta << nextGroupId << " " << nbrOfNewsGroups;
  out.close();
  outMeta.close();
}

vector<pair<int, string> > DiskDatabase::getNewsGroups(){
  vector<string> newsGroups;
  vector<pair<int, string> result;
  dirent* de;
  DIR* = opendir(root);
    while (true){
      de = readdir(dp);
      if (de == NULL){
        break;
      }
      result.push_back(string( de->d_name));
      }
    closedir(dp);
    for(string group : newsGroups){
      ifstream in(root+"//"+group+"//"+"meta");
      int junk, junk2;
      string name;
      in >> junk >> junk >> name;
      result.emplace_back(stoi(group), name);
      in.close();
    }
    return result;
  }


map<int, Article> DiskDatabase::getArticlesInNewsGroup(int newsGroupId){
  dirent* de;
  DIR* dp = opendir(root+"//"+to_string(newsGroupId));
  if(dp == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  vector<string> articles;
  map<int, Articles> result;

    while (true){
      de = readdir(dp);
      if (de == NULL){
        break;
      }
      articles.push_back(string( de->d_name));
      }
    closedir(dp);
    for(string art : articles){
      ifstream in(root+"//"+to_string(newsGroupId)+"//"+art);
      string name, author, text;
      in >> name >> author >> text;
      result.insert(make_pair(stoi(art), Article(name, author, text, stoi(art))));
      in.close();
    }
    return result;
}
int DiskDatabase::numberOfNewsGroups(){
  return nbrOfNewsGroups;
}
int DiskDatabase::numberOfArticlesInNewsGroup(int newsGroupId){
  return getArticlesInNewsGroup().size();
}
