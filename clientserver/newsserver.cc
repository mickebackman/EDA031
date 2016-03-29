

int main(int argc, char* argv[]){

	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
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

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;
				case Protocol::COM_LIST_NG:

				break;

				deafult:
				// DÖDA DEN SOM FAN - disconnecta
				break;



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