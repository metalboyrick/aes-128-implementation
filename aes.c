#include "aes.h"
#include <stdio.h>
#include <string.h>


char* encrypt(char* ptext){
	printf("encrypt: %s\n", ptext);
	return ptext;
}

char* decrypt(char* ctext){
	printf("decrypt: %s\n", ctext);
	return ctext;
}

