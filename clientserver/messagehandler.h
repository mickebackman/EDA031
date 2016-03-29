#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"

class MessageHandler {
	public:
		MessageHandler(const Connection& c) : conn(c) {}
		int readNumber();
		void writeNumber(int n);
		std::string readString();
		void writeString(const std::string& s);
	private:
		Connection& conn; 
};

#endif