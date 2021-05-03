#include "aes.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){

	if(argc <= 2){
		printf("Error!\n");
		return 0;
	}

	if (strcmp(argv[1], "-e") == 0){
		encrypt(argv[2]);
	} else if (strcmp(argv[1], "-d") == 0){
		decrypt(argv[2]);
	} else {
		printf("Error!\n");
	}


	return 0;
}