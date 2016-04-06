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
  numberOfNewsGroups = newsGroups;
  Database(nextId);

}

DiskDatabase::~DiskDatabase(){
  remove(root+"//"+"meta");
  ofstream out(root+"//"+"meta");
  out << nextGroupId << " " << numberOfNewsGroups;
  out.close();

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
  DIR *dir = NULL;
  dir = opendir(root+"//"+to_string(newsGroupId));
  if(dir == NULL){
    closedir(dir);
    // The news group does already exist.
    throw runtime_error("The group already exists!");
  }
  closedir(dir);
  mkdir(root+"//"+to_string(nextGroupId));
  ofstream out(root+"//"+to_string(nextGroupId)+"//"+"meta");
  out << "0 " << "0 " << newsGroupName << endl;
  ++nextGroupId;
  ++numberOfNewsGroups;
  out.close();


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
  --numberOfNewsGroups;
  system("rm -r"+root+"//"+to_string(newsGroupId));
}

vector<std::pair<int, std::string> > DiskDatabase::getNewsGroups(){

}
map<int, Article> DiskDatabase::getArticlesInNewsGroup(int newsGroupId);
int DiskDatabase::numberOfNewsGroups();
int DiskDatabase::numberOfArticlesInNewsGroup(int newsGroupId);
