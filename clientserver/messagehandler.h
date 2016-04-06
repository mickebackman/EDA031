#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include "connection.h"

class MessageHandler {
	public:
		MessageHandler(const Connection& c) : conn(c) {}
		unsigned char readByte();
		void writeByte(unsigned char c);
		int readNumber();
		void writeNumber(int n);
		std::string readString();
		std::string readString(int n);
		void writeString(const std::string& s);
	private:
		const Connection& conn;
};

#endif
