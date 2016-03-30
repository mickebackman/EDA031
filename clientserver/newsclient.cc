#include <string>
#include "messagehandler.h"
#include <memory>
#include <iostream>
#include "connection.h"
#include "protocol.h"

using namespace std;

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
	
	cout << "Enter commands: ";
	string commandLine;
	MessageHandler mh(conn);
	boolean inGroup = false;
	string name;
	while (getline(cin, commandLine)) {
		try {
			switch(command){
				case "list":
					if(!inGroup) {
						wh.writeByte(Protocol::COM_LIST_NG);
						wh.writeByte(Protocol::COM_END);
						inGroup = false;
					} else {
						cin >> name;
						wh.writeByte(Protocol::COM_LIST_ART);
						wh.writeNumber(name);
						wh.writeByte(Protocol::COM_END);
					}
					waitForAnswer();
				break;
				case "read":
					if(inGroup) {
						
					}
					Protocol::COM_GET_ART 
					CreateNewsGroup(mh, db);
				break;
				case "delete":
					Protocol::COM_DELETE_NG:
					Protocol::COM_DELETE_ART
					DeleteNewsGroup(mh, db);
				break;
				case "create":
					Protocol::COM_CREATE_NG:
					Protocol::COM_CREATE_ART
					ListArticles(mh, db);
				break;
			}
		} catch (ConnectionClosedException&) {
			cout << " No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

