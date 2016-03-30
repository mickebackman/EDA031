#include <string>
#include <vector>
#include "messagehandler.h"
#include "memorydatabase.h"
#include "database.h"
#include "diskdatabase.h"
#include "protocol.h"
#include "article.h"
#include "newsgroup.h"
#include <memory>
#include <iostream>

using namespace std;

bool ListNewsGroup(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == COM_END){
		try{
			vector<pair<int, string> v = db.getNewsGroups();
		}
		catch (exception& e){
			mh.writeByte(ANS_LIST_NG);
			mh.writeByte(PAR_NUM);
			mh.writeNumber(0);
			mh.writeByte(ANS_END);
			return;
		}
		// Answer on form: ANS_LIST_NG num_p [num_p string_p]* ANS_END
		mh.writeByte(ANS_LIST_NG);
		mh.writeByte(PAR_NUM);
		mh.writeNumber(v.size());
		for (pair<int,string> p : v){
			mh.writeByte(PAR_NUM);
			mh.writeNumber(p.first);
			mh.writeByte(PAR_STRING);
			mh.writeNumber(p.second.size());
			mh.writeString(p.second);
		}
		mh.writeByte(ANS_END);
		return true;
	} else {
		return false;
	}

}

bool CreateNewsGroup(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	int n; 
	string s;
	unsigned char c;
	if (c == PAR_STRING){
		n = mh.readNumber();
		s = mh.readString();
		char c = mh.readByte();
		try{
			db.addNewsGroup(s);
			mh.writeByte(ANS_CREATE_NG);
			mh.writeByte(ANS_ACK);
			mh.writeByte(ANS_END);
		} catch(exception& e){
		 mh.writeByte(ANS_CREATE_NG);
		 mh.writeByte(ANS_NAK);
		 mh.writeByte(ERR_NG_ALREADY_EXISTS);

		}
		return true;
	}
	else{
		// Protocol was not followed
		return false;
	}
}

bool DeleteNewsGroup(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == PAR_NUM){
		int deleteid = mh.readNumber();
		unsigned char e = mh.readByte();
		mh.writeByte(ANS_DELETE_NG);
		try {
			db.deleteNewsGroup(deleteid);
			mh.writeByte(ANS_ACK);
		}catch(exception& e){
			mh.writeByte(ANS_NAK);
			mh.writeByte(ERR_NG_DOES_NOT_EXIST);
		}
		mh.writeByte(ANS_END);
		return true;
	}else{
		// Protocol was not followed
		return false;
	}
}


bool ListArticles(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == PAR_NUM){
		int newsGroupId = mh.readNumber();
		unsigned char e = mh.readByte();
		mh.writeByte(ANS_LIST_ART);
		try {
			set<Article> s = db.getArticlesInNewsGroup(newsGroupId);
			mh.writeByte(ANS_ACK);
			mh.writeByte(PAR_NUM);
			mh.writeNumber(s.size());
			for(Article a : s){
				mh.writeByte(PAR_NUM);
				mh.writeNumber(a.getId());
				mh.writeByte(PAR_STRING);
				mh.writeNumber(a.getName().size());
				mh.writeString(a.getName());
			}
		}catch(exception& e){
			mh.writeByte(ANS_NAK);
			mh.writeByte(ERR_NG_DOES_NOT_EXIST);

		}
		mh.writeByte(ANS_END);
		return true;
	}
	else{
		// Protocol was not followed
		return false;
}
}

bool CreateArticle(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == PAR_NUM){
		int newsGroupId = mh.readNumber();
		if (mh.readByte() == PAR_STRING){
			mh.readNumber();
			string title = mh.readString();
			if (mh.readByte() == PAR_STRING){
				mh.readNumber();
				string author = mh.readString();
				if (mh.readByte() == PAR_STRING{
					mh.readNumber();
					string text = mh.readString();
					if (mh.readByte() == COM_END){
						// All is good. Respond
						mh.writeByte(ANS_CREATE_ART);
						try{
							db.addArticle(newsGroupId, title, author,text);
							mh.writeByte(ANS_ACK);
							mh.writeByte(ANS_END);
							
						}catch(exception& e){
							mh.writeByte(ANS_NAK);
							mh.writeByte(ANS_END);
						}
						return true;
					}
				}
			}
		}
	}
	// Protocol was not followed
	return false;
}

bool DeleteArticle(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == PAR_NUM){
		int newsGroupId = mh.readNumber();
		if (mh.readByte() == PAR_NUM){
			int artId = mh.readNumber();
			if (mh.readByte() == COM_END){
			mh.writeByte(ANS_DELETE_ART);
			try{
				db.deleteArticle();
				mh.writeByte(ANS_ACK);
				}catch(int e){
					mh.writeByte(ANS_NAK);
			 		if(e==0){ // newsgroup error
			 			mh.writeByte(ERR_NG_DOES_NOT_EXIST);
			 		}else { // article error
			 			mh.writeByte(ERR_ART_DOES_NOT_EXIST);
			 		}
				}
			mh.writeByte(ANS_END);
			return true;
			}
		}
	}
	// Does not follow protocol
	return false;
}


bool GetArticle(MessageHandler& mh, Database& db){
	unsigned char c = mh.readByte();
	if (c == PAR_NUM){
		int newsGroupId = mh.readNumber();
		if (mh.readByte() == PAR_NUM){
			int artId = mh.readNumber();
			if (mh.readByte() == COM_END){
				mh.writeByte(ANS_GET_ART);
				try{
					Article a = db.getArticle(newsGroupId, artId);
					mh.writeByte(ANS_ACK);
					mh.writeByte(PAR_STRING);
					mh.writeNumber(a.getName().size());
					mh.writeString(a.getName());
					mh.writeByte(PAR_STRING);
					mh.writeNumber(a.getAuthor().size());
					mh.writeString(a.getAuthor());
					mh.writeByte(PAR_STRING);
					mh.writeNumber(a.getText().size());
					mh.writeString(a.getText());
				}catch (int e){
					mh.writeByte(ANS_NAK);
					if (e==0){
						mh.writeByte(ERR_NG_DOES_NOT_EXIST);
					}else{
						mh.writeByte(ERR_ART_DOES_NOT_EXIST);
					}
				}
				mh.writeByte(ANS_END);
				return true;
			}
		}
	}
	// Does not follow protocol
	return false;
}

// string_p: PAR_STRING N char1 char2 ... charN // N is the number of characters
// num_p: PAR_NUM N // N is the number

int main(int argc, char* argv[]){

	if (argc != 3) {
		cerr << "Usage: myserver port-number storage-type(disk/memory)" << endl;
		exit(1);
	}
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	// Arguments for starting the program with the choice of db-type
	// TODO: db kanske behöver vara på heapen?
	Database db; 
	if (argv[2].compare("disk")){
		db = new DiskDatabase();
	} else if (argv[2].compare("memory")){
		db = new MemoryDatabase();
	}else {
		cerr << "Error in database choice" << endl;
		exit(1);
	}

	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			MessageHandler mh(conn);
			try {
			unsigned char command = mh.readByte();
			switch(command){
				case Protocol::COM_LIST_NG:
					if (!ListNewsGroup(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in listing news groups, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_CREATE_NG:
					if(!CreateNewsGroup(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in creating a news group, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_DELETE_NG:
					if(!DeleteNewsGroup(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in deleting a news group, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_LIST_ART:
					if(!ListArticles(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in listing articles, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_CREATE_ART:
					if(!CreateArticle(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in creating article, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_DELETE_ART:
					if(!DeleteArticle(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in deleting article, disconnecting client" << endl;
					}
				break;
				case Protocol::COM_GET_ART:
					if(!GetArticle(mh, db)){
					server.deregisterConnection(conn);
					cout << "Protocol not followed in getting article, disconnecting client" << endl;
					}
				break;

				deafult:

					server.deregisterConnection(conn);
					cout << "Wrong command received, disconnecting client" << endl;
					break;

			}

			} catch (ConnectionClosedException&) {
			server.deregisterConnection(conn);
		cout << "Client closed connection" << endl;
			}
		} else {
		conn = make_shared<Connection>();
		server.registerConnection(conn);
		cout << "New client connects" << endl;
		}
	}

}