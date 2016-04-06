#include "connection.h"
#include "messagehandler.h"
#include <string>
#include <iostream>

using namespace std;

int MessageHandler::readNumber(){
	unsigned char byte1 = conn.read();
	unsigned char byte2 = conn.read();
	unsigned char byte3 = conn.read();
	unsigned char byte4 = conn.read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MessageHandler::writeNumber(int value){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}

string MessageHandler::readString(){
	cout<<"read"<<endl;
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
		cout<<ch<<endl;
	}
	cout<<"end of while"<<endl;
	return s;
}

string MessageHandler::readString(int n){
	cout<<"read"<<endl;
	string s;
	char ch;
	for (size_t i = 0; i != n;++i) {
		ch = conn.read();
		s += ch;
		cout<<ch<<endl;
	}
	cout<<"end of while"<<endl;
	return s;
}

void MessageHandler::writeString(const string& s){
	for (char c : s) {
		conn.write(c);
		cout<<c<<endl;
	}
	//cout<<"$"<<endl;
	//conn.write('$');
}

unsigned char MessageHandler::readByte(){
	return conn.read();

}

void MessageHandler::writeByte(unsigned char c){
	conn.write(c);
}
