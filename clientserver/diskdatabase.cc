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

//Loads next group Id and number of news groups from file if a root directory is
//present. Otherwise one is created and a meta file as well.
DiskDatabase::DiskDatabase(){
  //The name of the root directory.
  root = "newsroot";
  //The path to the root meta file.
  string path = root+"/meta";
  ifstream in(path);
  if(in.good()){
    in >> nextGroupId >> nbrOfNewsGroups;
  }else{
    string command = "mkdir -p "+root;
    system(command.c_str());
    ofstream out(path);
    out << "0 " << "0" << endl;
    out.close();
  }
    in.close();
}

void DiskDatabase::addArticle(int newsGroupId, std::string name, std::string author, std::string text){
  DIR *dir = NULL;
  //The path to the news group in which the article is to be added.
  string path = root+"/"+to_string(newsGroupId);
  dir = opendir(path.c_str());
  if(dir == NULL){
    // The news group does not exist.
    closedir(dir);
    throw 0;
  }
    closedir(dir);

    //Extract nextId, number of articles and the group name from the meta file.
    path+= "/meta";
    ifstream in(path);
    int nextId, nbrOfArticles;
    string groupName;
    in >> nextId >> nbrOfArticles;
    getline(in, groupName);
    getline(in, groupName);
    in.close();
    ++nextId;
    ++nbrOfArticles;
    updateGroupMeta(path, nextId, nbrOfArticles, groupName);
    //The path to the new article file.
    string pathFile = root+"/"+to_string(newsGroupId)+"/"+to_string(nextId);
    ofstream out(pathFile);
    //Writes the article to file.
    out << name << endl << author << endl << text << endl;
    out.close();

}

//Updates the meta file specified by path. It changes the nextId
// and the number of articles to the specified values.
void DiskDatabase::updateGroupMeta(string path, int nextId, int nbrOfArticles, string groupName){
  remove(path.c_str());
  ofstream out(path);
  out << to_string(nextId) << " " << to_string(nbrOfArticles) << endl << groupName << endl;
  out.close();
}

//Updates the root meta file with the current values of nextGroupId and nbrOfNewsGroups.
void DiskDatabase::updateRootMeta(){
  string path = root+"/meta";
  remove(path.c_str());
  ofstream out(path);
  out << nextGroupId << " " << nbrOfNewsGroups << endl;
  out.close();
}

void DiskDatabase::addNewsGroup(std::string newsGroupName){
  auto v = getNewsGroups();
  for( auto p : v){
    if(p.second == newsGroupName){
      // The news group does already exist.
      throw runtime_error("The group already exists!");
    }
  }
  // System command to make the new group directory.
  string command = "mkdir -p "+root+"/"+to_string(nextGroupId);
  system(command.c_str());
  ofstream out(root+"/"+to_string(nextGroupId)+"/meta");
  out << "0 " << "0" << endl << newsGroupName << endl;
  ++nextGroupId;
  ++nbrOfNewsGroups;
  updateRootMeta();

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
    //Constructs an article object from file.
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
  in.close();
  --nbrOfArticles;
  updateGroupMeta(path, nextId, nbrOfArticles, groupName);

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
  //System command for removing the specified news group directory.
  string command = "rm -r "+root+"/"+to_string(newsGroupId);
  system(command.c_str());
  updateRootMeta();
}

vector<pair<int, string> > DiskDatabase::getNewsGroups(){
  //Vector to store the names of the news group directories (ids).
  vector<string> newsGroups;
  //Vector to store the ids and names of the news groups.
  vector<pair<int, string> > result;
  dirent* de;
  DIR* dir = opendir(root.c_str());
  //Iterates through all the news group directories and adding the ids to the Vector
  //newsGroups.
    while (true){
      de = readdir(dir);
      if (de == NULL){
        break;
      }
      string groupId = string(de->d_name);
      //Ignore any meta files.
      if(groupId != "meta" && groupId != "." && groupId != ".."  && groupId != ".DS_Store"){
            newsGroups.push_back(groupId);
      }
    }
    closedir(dir);
    //Iterate through the news groups meta files and and extract the name.
    for(string group : newsGroups){
      ifstream in(root+"//"+group+"//"+"meta");
      int junk;
      string name;
      //We are only interested in the name so we just throw away the other information
      //in the meta file.
      in >> junk >> junk;
      getline(in, name);
      getline(in, name);
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

  //Vector to store the name of the article files (ids).
  vector<string> articles;
  //Map to be filled with article objects and returned.
  map<int, Article> result;

  //Iterates through the articles in the news group and adds the name of the files
  //to the vector articles.
    while (true){
      de = readdir(dir);
      if (de == NULL){
        break;
      }
      string groupId = string(de->d_name);
      //Do not add the file if it is a meta file of some kind.
      if(groupId != "meta" && groupId != "." && groupId != ".." && groupId != ".DS_Store" ){
      articles.push_back(string( de->d_name));
    }
      }
    closedir(dir);
    //Iterates over all the articles found in the news group, creates an article
    //object and inserts it in the result map.
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
