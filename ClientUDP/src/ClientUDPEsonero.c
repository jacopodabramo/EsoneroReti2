#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include "protocol.h"
#include <string.h>
#include <float.h>

//Prototypes
void printMessage(int);

int checkOp(char);

int check(char [], struct Operation *);

int main(int argc,char *argv[]) {

	#if defined WIN32

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);

	if (iResult != 0) {
	 printf ("error at WSASturtup\n");
	 return EXIT_FAILURE;
	}

 	#endif

	//Connection variables initialization
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	int fromSize;
	char echoString[ECHOMAX];
	int respStringLen;
	struct hostent *host;
	char nameServer[ECHOMAX];
	int port;

	//message to Server variables initialization
	struct Operation msg;


	//Server's address
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;

	if(argc > 1){
				//command line values
				if(tokenizer(argv[1],nameServer,&port) == 0){
					//default values
					printf("The input string is not in correct form, using default values \n");
					echoServAddr.sin_port = htons(PORT);
					echoServAddr.sin_addr.s_addr = inet_addr(ADDR);
				}else{
					host = gethostbyname(nameServer);
					if (host == NULL) {
						fprintf(stderr, "gethostbyname() failed.\n");
						exit(EXIT_FAILURE);
					}
					struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];
					echoServAddr.sin_port = htons(port);
					echoServAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*ina));
				}
    }
	else{
	 	    	//default values
				echoServAddr.sin_port = htons(PORT);
				echoServAddr.sin_addr.s_addr = inet_addr(ADDR);
	}

 //Socket's creation
 if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	 ErrorHandler("socket() failed");

 while(1){
	 //getting operation
	 int k = 1;

	 do {
	 	printMessage(k);
	 	fgets(echoString,sizeof(echoString),stdin);
	 	k = check(echoString,&msg);
	 } while (k != 1);

	 if(checkOp(msg.op)) {
	 	printf("first operator = %d\n", msg.number1);
	 	printf("second operator  = %d\n", msg.number2);
	 }

	 //sending data to server
	 if (sendto(sock, (char*)(struct Operation*)&msg, (int)sizeof(struct Operation), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) !=  (int)sizeof(struct Operation))
	  	 ErrorHandler("Errors during sending");

	 //stop connection
	 if(msg.op == '=') break;

	 // result
	 float a;

	 //receiving data from server
	 fromSize = sizeof(fromAddr);
	 respStringLen = recvfrom(sock, (char *) (float*)&a, ECHOMAX, 0,(struct sockaddr*)&fromAddr, &fromSize);
	 if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
	 	 fprintf(stderr, "Error: received a packet from unknown source.\n");
	 	 exit(EXIT_FAILURE);
	  }
	 host = gethostbyaddr((char *) &(fromAddr.sin_addr),4, AF_INET);

	 if(msg.op == '/' && a == FLT_MAX  ) {
	     		printf("Error division by zero\n");
	 } else {
		 if(respStringLen < 0) printf("no data has been received\n");
		 else printf("Received result from server: %s, ip: %s: %d %c %d = %.3f\n", host->h_name,inet_ntoa(fromAddr.sin_addr),msg.number1,msg.op,msg.number2,a);
	 }

 }
 	 printf("Closing connection ...");
 	 closesocket(sock);
 	 ClearWinSock();
 	 //return EXIT_SUCCESS;
}


/*
 * Parameters:
 *             integer:k value to understand which kind of message should be printed
 *
 * Return:
 *             void function, it prints the message
 */
void printMessage(int k){
	if(k == 1) {
		printf("Enter a command in a format:[operation] [first operate] [second operate] \n");
	} else if(k == -1){
		printf("The numbers are not in the range. The range is between -10.000 and 10.000\n");
		printf("Enter a command in a format:[operation] [first operate] [second operate] \n");
	}else{
		printf("The command doesn't exist, the format is:[operation] [first operate] [second operate] \n");
	}
}

/*
 * Parameters:
 *             a: character
 * Return:
 *             integer: 1 if input is an operation,
 *                      0 otherwise
 */
int checkOp(char a){
    if(a == '+' ||  a == 'x' || a == '/' ||  a == '-') return 1;
    return 0;
}

/*
 * Parameters:
 *             in: User's message
 *             op: mapping user's message to operation struct
 * Return:
 *             integer: 1 if user's message is valid
 *                      0 otherwise
 */
int check(char in[], struct Operation *op) {
	if(checkOp(in[0]) == 1 && in[1] == ' '){
		op->op = in[0];
	}else if(in[0] == '=' && strlen(in) == 2){
		op->op = in[0];
		return 1;
	}
	else return 0;// false
	int k = 0;
	int i = 2;
	while(k<2){
		int j = 0;
		char num[10];
		if(in[i] == '-'){
			num[j] = in[i];
			j++;
			i++;
		}
		while(isdigit(in[i])){
			num[j] = in[i];
			j++;
			i++;
		}
		num[j] = '\0';

		if(in[i] == ' ' || i == strlen(in) - 1){
			if(k == 0){
				op->number1 = atoi(num);
				i++;
			}else op->number2 = atoi(num);
		}else return 0;
		k++;
	}
	if(i == strlen(in) -1){
		if((op->number1 > 10000 || op->number1 < -10000)||(op->number2 > 10000 || op->number2 < -10000)){
			return -1;
		} else return 1;
	}else return 0;
}
