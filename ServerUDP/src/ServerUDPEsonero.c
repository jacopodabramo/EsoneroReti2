#if defined WIN32

#include <winsock.h>

#else

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif

#include <float.h>
#include "protocol.h"
#include <stdio.h>
#include <string.h>

//Prototypes
float calculator(struct Operation);

float sub(int,int);

float mult(int,int);

float add(int,int);

float division(int,int);


 int main(int argc, char* argv[]) {
 	 #if defined WIN32
	 WSADATA wsaData;
	 int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	 if (iResult != 0) {
		 	 printf ("error at WSASturtup\n");
		 	 return EXIT_FAILURE;
	 }
 	 #endif

	 int sock;
	 struct sockaddr_in echoServAddr;
	 struct sockaddr_in echoClntAddr;
	 int cliAddrLen;
	 int recvMsgSize;
	 struct hostent *host;
	 struct Operation op;



	 // CREAZIONE DELLA SOCKET
	 if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		 	 ErrorHandler("socket() failed");

	 //INDIRIZZO DEL SERVER
	 memset(&echoServAddr, 0, sizeof(echoServAddr));
	 echoServAddr.sin_family = AF_INET;
	 if(argc > 1){
	 			//command line values
	 			echoServAddr.sin_port = htons(atoi(argv[2]));
	 			echoServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	     }
	 	else{
	 	 	    	//default values
	 				echoServAddr.sin_port = htons(PORT);
	 				echoServAddr.sin_addr.s_addr = inet_addr(ADDR);
	 	}

	 // BIND DELLA SOCKET
	 if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0)
		 ErrorHandler("bind() failed");

	 // RICEZIONE DELLA STRINGA ECHO DAL CLIENT
	 while(1) {
		 printf("Waiting for a client...\n");
		while(1){
		 cliAddrLen = sizeof(echoClntAddr);
		 if((recvMsgSize = recvfrom(sock,(char*)(struct Operation*)&op, sizeof(struct Operation), 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) != sizeof(struct Operation)){
			 ErrorHandler("recvfrom() receive different number of bytes than expected");
			 return -1;
		 }
		 host = gethostbyaddr((char *) &(echoClntAddr.sin_addr), 4, AF_INET);


		 if(op.op == '=') {
			 printf("Richiesta operazione '%c dal client %s, ip %s\n",op.op,host->h_name,inet_ntoa(echoClntAddr.sin_addr));
		     printf("\n");
		     break;
		 }
		 else {
			 printf("Richiesta operazione '%c %d %d' dal client %s, ip %s\n",op.op,op.number1,op.number2,host->h_name,inet_ntoa(echoClntAddr.sin_addr));
		     // Calculating the result
		     float result = calculator(op);

		     // Sending result
		     if ((recvMsgSize = sendto(sock, (char*)(float*)&result, sizeof(float), 0, (struct sockaddr *)&echoClntAddr,sizeof(echoClntAddr))) !=  sizeof(float)){
		    	 	 	 	 ErrorHandler("sendto() sent different number of bytes than expected");
		     	 	 	 	 return -1;
		     }
		    }
		}
	 }

   closesocket(sock);
   clearwinsock();
    return 0;
 }


 /*
  * Parameters:
  *           a,b: operators
  * Return:
  *           float: result of division
  */
 float division(int a,int b){
 	if(b == 0) return FLT_MAX  ;
 	float toTfloata = (float)a;
 	float toTfloatb = (float)b;
 	return toTfloata/toTfloatb;
 }

 /*
  * Parameters:
  *           a,b: operators
  * Return:
  *           float: result of addition
  */
 float add(int a,int b) {
 	return a+b;
 }

 /*
  * Parameters:
  *           a,b: operators
  * Return:
  *           float: result of multiplication
  */
 float mult(int a,int b) {
 	return a*b;
 }

 /*
  * Parameters:
  *           a,b: operators
  * Return:
  *           float: result of subtraction
  */
 float sub(int a,int b) {
 	return a-b;
 }

 /*
  * Parameters:
  *           op: struct
  * Return:
  *           float: parsing operation and getting result
  */
 float calculator(struct Operation op){
 	if(op.op == '+') {
 		return add(op.number1,op.number2);
 	}
 	if(op.op == '-') {
 		return sub(op.number1,op.number2);
 	}
 	if(op.op == 'x') {
 		return mult(op.number1,op.number2);
 	}
 	if(op.op == '/') {
 		return division(op.number1,op.number2);
 	}
 	return 0;
 }
