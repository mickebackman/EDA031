#include "diskdatabase.h"
#include <string>
#include <iostream>
#include <fstream>
#include "article.h"
#include "newsgroup.h"
#include <vector>
#include <map>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>

using namespace std;

DiskDatabase::DiskDatabase(){
  root = "newsroot";
  string command = "mkdir -p "+root;
  system(command.c_str());
  string path = root+"/meta";
  ofstream out(path);
  out << "0 " << "0" << endl;
  out.close();
  ifstream in(path);
  in >> nextGroupId >> nbrOfNewsGroups;
  in.close();
}

void DiskDatabase::addArticle(int newsGroupId, std::string name, std::string author, std::string text){
  DIR *dir = NULL;
  string path = root+"/"+to_string(newsGroupId);
  dir = opendir(path.c_str());
  if(dir == NULL){
    // The news group does not exist.
    closedir(dir);
    throw 0;
  }
    closedir(dir);
    path+= "/meta";
    ifstream in(path);
    int nextId, nbrOfArticles;
    string groupName;
    in >> nextId >> nbrOfArticles;
    getline(in, groupName);
    getline(in, groupName);
    string pathFile = root+"/"+to_string(newsGroupId)+"/"+to_string(nextId);
    ofstream out(pathFile);
    out << name << endl << author << endl << text << endl;
    out.close();
    in.close();
    remove(path.c_str());
    ofstream metaOut(path);
    metaOut << to_string(nextId + 1) << " " << to_string(nbrOfArticles + 1) << endl << groupName << endl;
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
  string command = "mkdir -p "+root+"/"+to_string(nextGroupId);
  system(command.c_str());
  ofstream out(root+"/"+to_string(nextGroupId)+"/meta");
  out << "0 " << "0" << endl << newsGroupName << endl;
  ++nextGroupId;
  ++nbrOfNewsGroups;
  string path = root+"/meta";
  remove(path.c_str());
  ofstream outMeta(root+"/meta");
  outMeta << nextGroupId << " " << nbrOfNewsGroups << endl;
  out.close();
  outMeta.close();
}

Article DiskDatabase::getArticle(int newsGroupId, int articleId){
  DIR *dir = NULL;
  string path = root+"/"+to_string(newsGroupId);
  dir = opendir(path.c_str());
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  ifstream in(root+"/"+to_string(newsGroupId)+"/"+to_string(articleId));
  if(in.fail()){
    //The file does not exist.
    throw 1;
    }
  string name, author, textLine, text;
  getline(in, name);
  getline(in, author);
  getline(in,text);
  while(getline(in, textLine)){
    text += "\n";
    text += textLine;
  }

  return Article(name, author, text, articleId);
}

void DiskDatabase::deleteArticle(int newsGroupId, int articleId){
  DIR *dir = NULL;
  string path = root+"/"+to_string(newsGroupId);
  dir = opendir(path.c_str());
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  path+=+"/"+to_string(articleId);
  if(remove(path.c_str()) != 0){
    //The file does not exist.
    throw 1;
  }
  path = root+"/"+to_string(newsGroupId)+"/meta";
  ifstream in(path);
  int nextId, nbrOfArticles;
  string groupName;
  in >> nextId >> nbrOfArticles;
  getline(in, groupName);
  getline(in, groupName);
  remove(path.c_str());
  ofstream metaOut(path);
  metaOut << nextId << " " << to_string(nbrOfArticles - 1) << endl << groupName << endl;
  metaOut.close();
  in.close();

}

void DiskDatabase::deleteNewsGroup(int newsGroupId){
  DIR *dir = NULL;
  string path = root+"/"+to_string(newsGroupId);
  dir = opendir(path.c_str());
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  --nbrOfNewsGroups;
  string command = "rm -r "+root+"/"+to_string(newsGroupId);
  system(command.c_str());
  path = root+"/meta";
  remove(path.c_str());
  ofstream outMeta(root+"/meta");
  outMeta << nextGroupId << " " << nbrOfNewsGroups;
  outMeta.close();
}

vector<pair<int, string> > DiskDatabase::getNewsGroups(){
  vector<string> newsGroups;
  vector<pair<int, string> > result;
  dirent* de;
  DIR* dir = opendir(root.c_str());
    while (true){
      de = readdir(dir);
      if (de == NULL){
        break;
      }
      string groupId = string(de->d_name);
      if(groupId != "meta" && groupId != "." && groupId != ".."  && groupId != ".DS_Store"){
        cout << "FEL" << endl;
        cout << groupId << endl;
            newsGroups.push_back(groupId);
      }
    }
    closedir(dir);
    for(string group : newsGroups){
      ifstream in(root+"//"+group+"//"+"meta");
      int junk;
      string name;
      in >> junk >> junk;
      getline(in, name);
      getline(in, name);
      cout <<  "Name: " << name << endl;
      result.emplace_back(stoi(group), name);
      in.close();
    }

  return result;
}


map<int, Article> DiskDatabase::getArticlesInNewsGroup(int newsGroupId){
  dirent* de;
  string path = root+"//"+to_string(newsGroupId);
  DIR* dir = opendir(path.c_str());
  if(dir == NULL){
    closedir(dir);
    // The news group does not exist.
    throw 0;
  }
  vector<string> articles;
  map<int, Article> result;

    while (true){
      de = readdir(dir);
      if (de == NULL){
        break;
      }
      string groupId = string(de->d_name);
      if(groupId != "meta" && groupId != "." && groupId != ".." && groupId != ".DS_Store" ){
      articles.push_back(string( de->d_name));
    }
      }
    closedir(dir);
    for(string art : articles){
      ifstream in(root+"//"+to_string(newsGroupId)+"//"+art);
      string name, author, textLine, text;
      getline(in, name);
      getline(in, author);
      getline(in, text);
      while(getline(in, textLine)){
        text += "\n";
        text += textLine;
      }
      result.insert(make_pair(stoi(art), Article(name, author, text, stoi(art))));
      in.close();
    }
    return result;
}
int DiskDatabase::numberOfNewsGroups(){
  return nbrOfNewsGroups;
}
int DiskDatabase::numberOfArticlesInNewsGroup(int newsGroupId){
  return getArticlesInNewsGroup(newsGroupId).size();
}
