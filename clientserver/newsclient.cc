#include <string>
#include "messagehandler.h"
#include <memory>
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
			c = mh.readByte();
			case Protocol::ANS_CREATE_NG:
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Newsgroup created" << endl;
					return true;
				}else if (c == Protocol::ANS_NAK){
					if (mh.readByte() == Protocol::ERR_NG_ALREADY_EXISTS){
						if (mh.readByte() == Protocol::ANS_END){
							cout << "Newsgroup could not be created: Newsgroup already exist" << endl;
							return true;
						}
					}
				}
			}
			return false;
			case Protocol::ANS_CREATE_ART:
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Article created" << endl;
					return true;
				}else if (c == Protocol::ANS_NAK){
					if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
						if (mh.readByte() == Protocol::ANS_END){
							cout << "Article could not be created: Newsgroup does not exist" << endl;
							return true;
						}
					}
				}
			}
			return false;

			case Protocol::ANS_DELETE_NG:
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Newsgroup deleted" << endl;
					return true;
				}else if (c == Protocol::ANS_NAK){
					if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
						if (mh.readByte() == Protocol::ANS_END){
							cout << "Newsgroup could not be deleted: Newsgroup doest not exist" << endl;
							return true;
						}
					}
				}
			}
			return false;

			case Protocol::ANS_DELETE_ART:
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::ANS_END){
					cout << "Article deleted" << endl;
					return true;
				}else if (c == Protocol::ANS_NAK){
					unsigned char e = mh.readByte();
					if (e == Protocol::ERR_NG_DOES_NOT_EXIST){
						if (mh.readByte() == Protocol::ANS_END){
							cout << "Article could not be deleted: Newsgroup doest not exist" << endl;
							return true;
						}
					}else if(e == Protocol::ERR_ART_DOES_NOT_EXIST){
						cout << "Article could not be deleted: Article does not exist" << endl;
					}
				}
			}
			return false;

			case Protocol::ANS_LIST_NG:
			if (c == Protocol::PAR_NUM){
				int n = mh.readNumber();
				string out;
				for (int i = 0; i != n; ++i){
					if (mh.readByte() == Protocol::PAR_NUM){
						out.append(to_string(mh.readNumber()));
						out.append(" ");
						if (mh.readByte() == Protocol::PAR_STRING){
							mh.readNumber();
							out.append(mh.readString());
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
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::PAR_NUM){
					int n = mh.readNumber();
					for (int i = 0; i != n; ++i){
						if (mh.readByte() == Protocol::PAR_NUM){
							out.append(to_string(mh.readNumber()));
							out.append(" ");
							if (mh.readByte() == Protocol::PAR_STRING){
								out.append(mh.readString() + "\n");

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
			if (c == Protocol::ANS_ACK){
				if (mh.readByte() == Protocol::PAR_STRING){
					mh.readNumber();
					out.append(mh.readString() + "\n");
					if (mh.readByte() == Protocol::PAR_STRING){
						mh.readNumber();
						out.append(mh.readString() + "\n");
						if (mh.readByte() == Protocol::PAR_STRING){
							mh.readNumber();
							out.append(mh.readString() + "\n");
							if(mh.readByte() == Protocol::ANS_END){
								cout << out << endl;
								return true;
							}
						}
					}
				}
			}else if (c == Protocol::ANS_NAK){
				if (mh.readByte() == Protocol::ERR_NG_DOES_NOT_EXIST){
					cout << "Could not get article: Newsgroup does not exist" << endl;
					return true;
				} else if(mh.readByte() == Protocol::ERR_ART_DOES_NOT_EXIST){
					cout << "Could not get article: Newsgroup does not exist" << endl;
					return true;
				}
			}
			return false;
		} //end switch
	} //end while
}	//end function
int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: newsclient host-name port-number" << endl;
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
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	cout << "Welcome to NewsApp, Commands: \n listGroups \n listArticles \n createGroup \n createArticle \n deleteGroup \n deleteArticle groupId \"articleName\" \"articleAuthor\" \"articleText\" \n readArticle";
	string command;
	string article;
	MessageHandler mh(conn);
	string groupName;
	string articleName;
	string articleAuthor;
	string articleText;
	int groupId;
	int articleId;
	while (cin >> command) {
		try {
			if(command =="listGroups"){
				mh.writeByte(Protocol::COM_LIST_NG);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "listArticles"){
				mh.writeByte(Protocol::COM_LIST_ART);
				cin >> groupId;
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "createGroup"){
				mh.writeByte(Protocol::COM_CREATE_NG);
				getline(cin, groupName);
				mh.writeString(groupName);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "createArticle"){
				mh.writeByte(Protocol::COM_CREATE_ART);
				cin >> groupId;
				getline(cin, article);

				auto start = article.find_first_of("\"");
				auto end = article.find_first_of("\"", start+1);
				articleName = article.substr(start+1, end-start);

				start = article.find_first_of("\"", end+1);
				end = article.find_first_of("\"", start);
				articleAuthor = article.substr(start+1, end-start);

				start = article.find_first_of("\"", end+1);
				end = article.find_first_of("\"", start);
				articleText = article.substr(start+1, end-start);

				mh.writeNumber(groupId);
				mh.writeString(articleName);
				mh.writeString(articleAuthor);
				mh.writeString(articleText);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "deleteGroup"){
				mh.writeByte(Protocol::COM_DELETE_NG);
				cin >> groupId;
				mh.writeNumber(groupId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "deleteArticle"){
				mh.writeByte(Protocol::COM_DELETE_ART);
				cin >> groupId >> articleId;
				mh.writeNumber(groupId);
				mh.writeNumber(articleId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}
			else if( command == "readArticle"){
				mh.writeByte(Protocol::COM_GET_ART);
				cin >> groupId >> articleId;
				mh.writeNumber(groupId);
				mh.writeNumber(articleId);
				mh.writeByte(Protocol::COM_END);
				if (!waitForAnswer(mh)){
					cout << "Got something wrong off protocol from server" << endl;
				}
			}else{
				cout << "Cannot recognize command.";
			}
		} catch (ConnectionClosedException&) {
			cout << " No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}
