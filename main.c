#include "aes.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){

	if(argc <= 1){
		printf("Error!\n");
		return 0;
	}

	if (strcmp(argv[1], "-e") == 0){
		int out = encrypt(argv[2]);
		if (out == -1){
			printf("Invalid plaintext!\n");
		} else {
			printf("Encryption successful!\n");
		}
	} else if (strcmp(argv[1], "-d") == 0){
		if(argc <= 3) {
			printf("Error!\n");
			return 0;
		}
		decrypt(argv[2], argv[3], argv[4], false);
	} else if (strcmp(argv[1], "-r") == 0){
		random_ops();
	} else {
		printf("Invalid arguments!\n");
	}


	return 0;
}