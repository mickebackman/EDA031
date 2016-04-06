#include <string>
#include "messagehandler.h"
#include <memory>
#include <algorithm>
#include <iostream>
#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"

using namespace std;

bool waitForAnswer(MessageHandler& mh) {
	unsigned char c = mh.readByte();
	while(true) {
		string out = "";
		switch (c){
			case Protocol::ANS_CREATE_NG:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Newsgroup created\n" << endl;
					return true;
				}
			}else if (c == Protocol::ANS_NAK){
				if (mh.readByte() == Protocol::ERR_NG_ALREADY_EXISTS){
					if (mh.readByte() == Protocol::ANS_END){
						cout << "Newsgroup could not be created: Newsgroup already exists\n" << endl;
						return true;
					}
				}
				
			}
			return false;
			case Protocol::ANS_CREATE_ART:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Article created\n" << endl;
					return true;
				}
			}else if (c == Protocol::ANS_NAK){
				if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
					if (mh.readByte() == Protocol::ANS_END){
						cout << "Article could not be created: Newsgroup does not exist\n" << endl;
						return true;
					}
				
			}
			return false;
			
			case Protocol::ANS_DELETE_NG:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Newsgroup deleted\n" << endl;
					return true;
				}
			}else if (c == Protocol::ANS_NAK){
				if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
					if (mh.readByte() == Protocol::ANS_END){
						cout << "Newsgroup could not be deleted: Newsgroup doest not exist\n" << endl;
						return true;
					}
				}
				
			}
			return false;
			
			case Protocol::ANS_DELETE_ART:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Article deleted\n" << endl;
					return true;
				}
			}else if (c == Protocol::ANS_NAK){
				unsigned char e = mh.readByte();
				if (e == Protocol::ERR_NG_DOES_NOT_EXIST){
					if (mh.readByte() == Protocol::ANS_END){
						cout << "Article could not be deleted: Newsgroup doest not exist\n" << endl;
						return true;
					}
				}else if(e == Protocol::ERR_ART_DOES_NOT_EXIST){
					cout << "Article could not be deleted: Article does not exist\n" << endl;
				}
				
			}
			return false;
			
			case Protocol::ANS_LIST_NG:
			c = mh.readByte();
			if (c == Protocol::PAR_NUM){
				int n = mh.readNumber();
				string out;
				for (int i = 0; i != n; ++i){
					if (mh.readByte() == Protocol::PAR_NUM){
						out.append(to_string(mh.readNumber()));
						out.append(" ");
						if (mh.readByte() == Protocol::PAR_STRING){
							int nbr = mh.readNumber();
							out.append(mh.readString(nbr));
							out.append("\n");
						}
					}
				}
				if(mh.readByte() == Protocol::ANS_END){
					cout << out << endl;
					return true;
				}
			}
			return false;
			
			case Protocol::ANS_LIST_ART:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::PAR_NUM){
					int n = mh.readNumber();
					for (int i = 0; i != n; ++i){
						if (mh.readByte() == Protocol::PAR_NUM){
							out.append(to_string(mh.readNumber()));
							out.append(" ");
							if (mh.readByte() == Protocol::PAR_STRING){
								int nbr = mh.readNumber();
								out.append(mh.readString(nbr) + "\n");
							}
						}
					}
					if (mh.readByte() == Protocol::ANS_END){
						cout << out << endl;
						return true;
					}
				}
			}
			return false;
			case Protocol::ANS_GET_ART:
			c = mh.readByte();
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::PAR_STRING){
					int nbr = mh.readNumber();
					out.append(mh.readString(nbr) + " From: ");
					if (mh.readByte() == Protocol::PAR_STRING){
						nbr = mh.readNumber();
						out.append(mh.readString(nbr) + "\n");
						if (mh.readByte() == Protocol::PAR_STRING){
							nbr = mh.readNumber();
							out.append(mh.readString(nbr) + "\n");
							if(mh.readByte() == Protocol::ANS_END){
								cout << out << endl;
								return true;
							}
						}
					}
				}
			}else if (c == Protocol::ANS_NAK){
				if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
					cout << "Could not get article: Newsgroup does not exist\n" << endl;
					return true;
				} else if(mh.readByte() == Protocol::ERR_ART_DOES_NOT_EXIST){
					cout << "Could not get article: Article does not exist\n" << endl;
					return true;
				}
			}
			return false;
		} //end switch
	} //end while
}	//end function
int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: newsclient host-name port-number\n" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed\n" << endl;
		exit(1);
	}
	
	cout << "********************************************************" << endl;
	cout << "Welcome to NewsApp, Commands: \n listGroups \n listArticles groupId \n createGroup groupName \n createArticle groupId \"articleName\" \"articleAuthor\" \"articleText\" \n deleteGroup groupId \n deleteArticle groupId articleId \n readArticle groupId articleId \n help \n";
	cout << "********************************************************\n" << endl;
	string command;
	string article;
	MessageHandler mh(conn);
	string groupName;
	string articleName;
	string articleAuthor;
	string articleText;
	int groupId;
	int articleId;
	while (true) {
		cout << "news> ";
		cin >> command;
		transform(command.begin(), command.end(), command.begin(), ::tolower);
		try {
			if(command =="listgroups"){
				mh.writeByte(Protocol::COM_LIST_NG);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "listarticles"){
				mh.writeByte(Protocol::COM_LIST_ART);
				cin >> groupId;
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "creategroup"){
				mh.writeByte(Protocol::COM_CREATE_NG);
				getline(cin, groupName);
				mh.writeByte(Protocol::PAR_STRING);
				mh.writeNumber(groupName.length());
				mh.writeString(groupName);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "createarticle"){
				mh.writeByte(Protocol::COM_CREATE_ART);
				cin >> groupId;
				getline(cin, article);
				
				auto start = article.find_first_of("\"");
				auto end = article.find_first_of("\"", start+1);
				articleName = article.substr(start+1, end-start-1);
				
				start = article.find_first_of("\"", end+1);
				end = article.find_first_of("\"", start+1);
				articleAuthor = article.substr(start+1, end-start-1);
				
				start = article.find_first_of("\"", end+1);
				end = article.find_first_of("\"", start+1);
				articleText = article.substr(start+1, end-start-1);
				
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(groupId);
				
				mh.writeByte(Protocol::PAR_STRING);
				mh.writeNumber(articleName.length());
				mh.writeString(articleName);
				
				mh.writeByte(Protocol::PAR_STRING);
				mh.writeNumber(articleAuthor.length());
				mh.writeString(articleAuthor);
				
				mh.writeByte(Protocol::PAR_STRING);
				mh.writeNumber(articleText.length());
				mh.writeString(articleText);
				
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "deletegroup"){
				mh.writeByte(Protocol::COM_DELETE_NG);
				cin >> groupId;
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "deletearticle"){
				mh.writeByte(Protocol::COM_DELETE_ART);
				cin >> groupId >> articleId;
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(articleId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "readarticle"){
				mh.writeByte(Protocol::COM_GET_ART);
				cin >> groupId >> articleId;
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::PAR_NUM);
				mh.writeNumber(articleId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server\n" << endl;
				}
			}
			else if( command == "help" ){
				cout << "********************************************************" << endl;
				cout << "Welcome to NewsApp, Commands: \n listGroups \n listArticles groupId \n createGroup groupName \n createArticle groupId \"articleName\" \"articleAuthor\" \"articleText\" \n deleteGroup groupId \n deleteArticle groupId articleId \n readArticle groupId articleId \n help \n";
				cout << "********************************************************\n" << endl;
			}else{
				cout << "Cannot recognize command.\n" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << " No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}
