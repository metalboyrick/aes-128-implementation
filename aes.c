
#include "aes.h"
#define MAX_BIT 128
#define MAX_BYTE 16

uint8_t init_vector[16]  = {0};
uint8_t secret_key[16] = {0};
bool random_flag = false;

/*
	Function to generate alphanumeric characters
	@params None
	@return (uint8_t) a single random alphanumeric character
*/
uint8_t gen_alphanum(){
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    if(!random_flag){
    	srand( (unsigned) time(NULL) * getpid());
    	random_flag = true;
    }
    
    return alphanum[rand() % (sizeof(alphanum) - 1)];
}


/*
	Function to generate initialization vector
	@params None
	@return (int) 0 if successful, -1 if failed
*/
int gen_iv(){

	for (int i = 0 ; i < MAX_BYTE; i++){
		init_vector[i] = gen_alphanum();
	}
}


/*
	Function to generate initialization vector
	@params None
	@return (int) 0 if successful, -1 if failed
*/
int gen_key(){

	for (int i = 0 ; i < MAX_BYTE; i++){
		secret_key[i] = gen_alphanum();
	}
}

/*
	Function to print the encryption result
	@params None
	@return None
*/
void print_encrypt_result() {

	printf("IV: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%c", init_vector[i]);
	}
	printf("\nSECRET KEY: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%c", secret_key[i]);
	}
	printf("\n");
}

char* encrypt(char* ptext){

	// generate the secret keys and the init vectors
	gen_key();
	gen_iv();
	
	print_encrypt_result();

	return ptext;
}

char* decrypt(char* ctext){
	printf("decrypt: %s\n", ctext);
	return ctext;
}

