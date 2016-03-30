#include <string>
#include "messagehandler.h"
#include "memorydatabase.h"
#include "database.h"
#include "diskdatabase.h"
#include <memory>
#include <iostream>

using namespace std;

void ListNewsGroup(MessageHandler& mh, Database& db){
	//TODO: Göra dessa metoder.. Vi ska läsa in mera och tolka kommandot sen svara
	//genom messagehandler, men vi har ju ett objekt av messagehandler? Funkar detta då?
	//ska vi istället bara göra metoder som ligger löst vi kan kalla på? slipper vi göra 
	//nya messagehandlers i whilesatsen
}

void CreateNewsGroup(MessageHandler& mh, Database& db){

}

void DeleteNewsGroup(MessageHandler& mh, Database& db){

}

void ListArticles(MessageHandler& mh, Database& db){

}

void CreateArticle(MessageHandler& mh, Database& db){

}

void DeleteArticle(MessageHandler& mh, Database& db){

}

void GetArticle(MessageHandler& mh, Database& db){

}
void End(MessageHandler& mh, Database& db){

}

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
		cerr << "Error in databasechoice" << endl;
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
					ListNewsGroup(mh, db);
				break;
				case Protocol::COM_CREATE_NG:
					CreateNewsGroup(mh, db);
				break;
				case Protocol::COM_DELETE_NG:
					DeleteNewsGroup(mh, db);
				break;
				case Protocol::COM_LIST_ART:
					ListArticles(mh, db);
				break;
				case Protocol::COM_CREATE_ART:
					CreateArticle(mh, db);
				break;
				case Protocol::COM_DELETE_ART:
					DeleteArticle(mh, db);
				break;
				case Protocol::COM_GET_ART:
					GetArticle(mh, db);
				break;
				case Protocol::COM_END:
					End(mh, db);
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