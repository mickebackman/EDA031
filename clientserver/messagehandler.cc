#include "connection.h"
#include "messagehandler.h"
#include <string>

using namespace std;

int MessageHandler::readNumber(){
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MessageHandler::writeNumber(int n){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}

string MessageHandler::readString(){
	string s;
	char ch;
	while ((ch = conn.read()) != ’$’) {
		s += ch;
	}
	return s;
}

void MessageHandler::writeString(const string& s){
	for (char c : s) {
		conn->write(c);
	}
	conn->write(’$’);
}