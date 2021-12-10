#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define ECHOMAX 255
#define PORT 56700
#define ADDR "127.0.0.1"

struct Operation {
	char op;
	int number1;
	int number2;
};


void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}

void ClearWinSock() {
 #if defined WIN32
 WSACleanup();
 #endif
}

#endif /* PROTOCOL_H_ */
