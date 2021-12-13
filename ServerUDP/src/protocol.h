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

/*
 *Parameters:
 *             character
 *Return:
 *             1 if the caracter is a number
 *             0 otherwise
 *
 */
int number(char toVerify){
    if(toVerify == '0' || toVerify == '1' || toVerify == '2' ||
        toVerify == '3' || toVerify == '4' || toVerify == '5' ||
        toVerify == '6' || toVerify == '7' || toVerify == '8' ||
        toVerify == '9') return 1; //toVerify is a number
    return 0;
}


/*
 * Parameters:
 *              input: a string
 *              nameServer: is a reference to save the name of Server
 *              port: is a reference to save the port's number
 * Return:
 *              integer:1 if the string is in a correct form
 *                      0 otherwise
 *
 */
int tokenizer(char input[],char nameServer[],int *port){
    if(input[0] == ':') return 0;
    char p[100];
    int i = 0;
    for(i = 0; i < strlen(input); i++){
        if(input[i] == ':') break;
        nameServer[i] = input[i];
    }
    nameServer[i] = '\0';
    i++;
    if(i >= strlen(input)) return 0; // input is not in correct form
    int j;
    int k = 0;
    for(j = i; j < strlen(input); j++){
        if(number(input[j]) == 0) return 0; // the port is not a number
        p[k] = input[j];
        k++;
    }
    p[k] = '\0';
    *port = atoi(p);
    return 1;
}

#endif /* PROTOCOL_H_ */
