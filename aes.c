
#include "aes.h"
#define MAX_BIT 128
#define MAX_BYTE 16

// s-box mappings
static const uint8_t S_BOX[256] = {
	//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

// inverse s-box mapping
static const uint8_t INV_S_BOX[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

// round constants to calculate the keys
static const uint8_t R_CON[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

uint8_t init_vector[16]  = {0};
uint8_t secret_key[16] = {0};
uint8_t round_keys[11*16];
uint8_t text_state[16001];

bool random_flag = false;

/*--------------------------------------------UTIL FUNCTIONS-------------------------------------*/


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
uint32_t gen_iv(){

	for (int i = 0 ; i < MAX_BYTE; i++){
		init_vector[i] = gen_alphanum();
	}
}


/*
	Function to generate initialization vector
	@params None
	@return (int) 0 if successful, -1 if failed
*/
uint32_t gen_key(){

	for (int i = 0 ; i < MAX_BYTE; i++){
		secret_key[i] = gen_alphanum();
	}
}

/*
	Function to print the encryption result
	@params None
	@return None
*/
void print_encrypt_result(int str_len) {

	printf("IV: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%02x", init_vector[i]);
	}
	printf("\nSECRET KEY: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%02x", secret_key[i]);
	}
	printf("\nCIPHERTEXT: ");
	for(int i = 0 ; i < str_len; i++){
		printf("%02x", text_state[i]);
	}
	printf("\n");
}

/*
	Function to print the decryption result
	@params None
	@return None
*/
void print_decrypt_result(int str_len) {

	printf("IV: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%02x", init_vector[i]);
	}
	printf("\nSECRET KEY: ");
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%02x", secret_key[i]);
	}
	printf("\nPLAINTEXT: ");
	for(int i = 0 ; i < str_len; i++){
		printf("%c", text_state[i]);
	}
	printf("\n");
}

/*
	Function to print a sequence with label
	@param (char* state_name)		The label
	@param (uint8_t* current_state)	The state array
*/
void print_state(char* state_name, uint8_t* current_state){
	printf("%s:",state_name);
	for(int i = 0 ; i < MAX_BYTE; i++){
		printf("%x ", current_state[i]);
	};
	printf("\n");
}

/*
	Function to transpose a block
	@param (uint8_t* block) 			pointer to a block

*/
void transpose(uint8_t* block){
	for(int i = 0; i < 4; i++){
		for(int j = i + 1; j < 4; j++){
				uint8_t temp = block[4 * i + j];
				block[4 * i + j] = block[4 * j + i];
				block[4 * j + i] = temp;
		}
	}
}

/*
	Galois multiply
	ref: https://blog.csdn.net/shaosunrise/article/details/80174210
	@param (uint8_t u) integer 
	@param (uint8_t v) integer
	@returns (uint8_t) result
*/
uint8_t g_mul(uint8_t u, uint8_t v){
	uint8_t p = 0;

    for (int i = 0; i < 8; ++i) {
        if (u & 0x01) {
            p ^= v;
        }

        int flag = (v & 0x80);
        v <<= 1;
        if (flag) {
            v ^= 0x1B;  /* P(x) = x^8 + x^4 + x^3 + x + 1 */
        }

        u >>= 1;
    }

    return p;
}

/*
	Converts hex string to byte arrays
	@params (char* str) the string to be converted
	@params (int str_len) length of string
	@params (uint8_t* dst_arr) the destination array to be copied to
	@returns None
*/
void str_to_byte(char* str, int str_len, uint8_t* dst_arr){
    char* pos = str;
    for (int count = 0; count < str_len; count++) {
        sscanf(pos, "%2hhx", &dst_arr[count]);
        pos += 2;
    }
}


/*-----------------------------------------------------------------------------------------------*/

/*--------------------------------------------ADD ROUND KEYS-------------------------------------*/

/*
	Function to calculate all the key expansion for AES 128
	Note: only works for AES-128
	@param (uint8_t* key) current key state to be scheduled (an array of 18, 1 byte each )
	@return (uint8_t*) The new scheduled key
*/
void key_expansion(uint8_t* key){
	static const int N = 4;
	static const int R = 11;

	int loop_count = 4 * R;
	for(int i = 0; i < loop_count; i++){
		if (i < N) {
			round_keys[i * 4] = key[i * 4];
			round_keys[i * 4 + 1] = key[i * 4 + 1];
			round_keys[i * 4 + 2] = key[i * 4 + 2];
			round_keys[i * 4 + 3] = key[i * 4 + 3];
		} else if ((i >= N) && (i % N == 0)) {
			round_keys[i * 4] = (round_keys[(i - N) * 4] ^ S_BOX[round_keys[(i - 1) * 4 + 1]]) ^ R_CON[i/N];
			round_keys[i * 4 + 1] = (round_keys[(i - N) * 4 + 1] ^ S_BOX[round_keys[(i - 1) * 4 + 2]]);
			round_keys[i * 4 + 2] = (round_keys[(i - N) * 4 + 2] ^ S_BOX[round_keys[(i - 1) * 4 + 3]]);
			round_keys[i * 4 + 3] = (round_keys[(i - N) * 4 + 3] ^ S_BOX[round_keys[(i - 1) * 4]]);
		} else {
			round_keys[i * 4] = round_keys[(i - N) * 4] ^ round_keys[(i - 1) * 4];
			round_keys[i * 4 + 1] = round_keys[(i - N) * 4 + 1] ^ round_keys[(i - 1) * 4 + 1];
			round_keys[i * 4 + 2] = round_keys[(i - N) * 4 + 2] ^ round_keys[(i - 1) * 4 + 2];
			round_keys[i * 4 + 3] = round_keys[(i - N) * 4 + 3] ^ round_keys[(i - 1) * 4 + 3];
		}

	}

	// transpose the round keys
	uint8_t* current_block;
	for(int i = 0; i < 11; i++){
		current_block = round_keys + 16 * i;
		transpose(current_block);
	}

}

/*
	Function to add an n-th expanded round key
	@param (uint8_t* block) 			pointer to a block
	@param (int round_key_index)		The n-th iteration of the round key used
*/
void add_round_key(uint8_t* block, int round_key_index){
	uint8_t* round_key_pt = round_keys + (round_key_index * 16);
	for (int i = 0 ; i < 16; i++){
		block[i] ^= round_key_pt[i];
	}
}

/*-----------------------------------------------------------------------------------------------*/

/*--------------------------------------------S-BOX SUBSTITUTION---------------------------------*/

/*
	Function to perform the s-box substitution to a particular block
	@params (uint8_t* block) pointer to a block
*/
void s_box_sub(uint8_t* block){
	for(int i = 0 ; i < 16; i++){
		block[i] = S_BOX[block[i]];
	}
}

/*
	Function to perform inverse s-box substitution
	@params (uint8_t* block) pointer to a block
*/
void inv_s_box_sub(uint8_t* block){
	for(int i = 0 ; i < 16; i++){
		block[i] = INV_S_BOX[block[i]];
	}
}

/*-----------------------------------------------------------------------------------------------*/

/*--------------------------------------------SHIFT ROWS-----------------------------------------*/

/*
	Function to perform the shift row operation on a block of the plaintext
	@params (uint8_t* block) pointer to a block
	@returns None
*/
void shift_rows(uint8_t* block){

	// helper variables
	uint8_t temp = 0;
	uint8_t temp1 = 0;

	// row 0 is not shifted at all
	// shift row 1 
	temp = block[4];
	block[4] = block[5];
	block[5] = block[6];
	block[6] = block[7];
	block[7] = temp;

	// shift row 2
	temp = block[8];
	temp1 = block[9];
	block[8] = block[10];
	block[9] = block[11];
	block[10] = temp;
	block[11] = temp1;

	// shift row 3
	temp = block[15];
	block[15] = block[14];
	block[14] = block[13];
	block[13] = block[12];
	block[12] = temp;

}

/*
	Function to revert the shift rows operation
	@params (uint8_t* block) pointer to a block
	@returns None
*/
void inv_shift_rows(uint8_t* block){
		// helper variables
	uint8_t temp = 0;
	uint8_t temp1 = 0;

	// row 0 is not shifted at all
	// shift row 1 
	temp = block[7];
	block[7] = block[6];
	block[6] = block[5];
	block[5] = block[4];
	block[4] = temp;

	// shift row 2
	temp = block[8];
	temp1 = block[9];
	block[8] = block[10];
	block[9] = block[11];
	block[10] = temp;
	block[11] = temp1;

	// shift row 3
	temp = block[12];
	block[12] = block[13];
	block[13] = block[14];
	block[14] = block[15];
	block[15] = temp;
}

/*-----------------------------------------------------------------------------------------------*/

/*--------------------------------------MIX COLUMNS----------------------------------------------*/

/*
	Function to perform the mix columns
	@params (uint8_t* block) pointer to a block
	@returns None
*/
void mix_columns(uint8_t* block){
	uint8_t res_block[16] = {0};
	for(int i = 0 ; i < 16; i++){
		if(i < 4) 
			res_block[i] = g_mul(2, block[i]) ^ g_mul(3, block[i+4]) ^ block[i+8] ^ block[i+12];
		else if(i >= 4 && i < 8)
			res_block[i] = block[i-4] ^ g_mul(2, block[i]) ^ g_mul(3, block[i+4]) ^ block[i+8];
		else if(i >= 8 && i < 12)
			res_block[i] = block[i-8] ^ block[i-4] ^ g_mul(2, block[i]) ^ g_mul(3, block[i+4]);
		else
			res_block[i] = g_mul(3, block[i-12]) ^ block[i-8] ^ block[i-4] ^ g_mul(2, block[i]);
	}

	for(int i = 0 ; i < 16; i++){
		block[i] = res_block[i];
	}

}

/* 
	Function to invert the mix columns
	@params (uint8_t* block) pointer to a block
	@returns None
*/
void inv_mix_columns(uint8_t* block){
	uint8_t res_block[16] = {0};
	for(int i = 0 ; i < 16; i++){
		if(i < 4) 
			res_block[i] = g_mul(0x0e, block[i]) ^ g_mul(0x0b, block[i+4]) ^ g_mul(0x0d, block[i+8]) ^ g_mul(0x09, block[i+12]);
		else if(i >= 4 && i < 8)
			res_block[i] = g_mul(0x09, block[i-4]) ^ g_mul(0x0e, block[i]) ^ g_mul(0x0b, block[i+4]) ^ g_mul(0x0d, block[i+8]);
		else if(i >= 8 && i < 12)
			res_block[i] = g_mul(0x0d, block[i-8]) ^ g_mul(0x09, block[i-4]) ^ g_mul(0x0e, block[i]) ^ g_mul(0x0b, block[i+4]);
		else
			res_block[i] = g_mul(0x0b, block[i-12]) ^ g_mul(0x0d, block[i-8]) ^ g_mul(0x09, block[i-4]) ^ g_mul(0x0e, block[i]);
	}

	for(int i = 0 ; i < 16; i++){
		block[i] = res_block[i];
	}
}
/*-----------------------------------------------------------------------------------------------*/


int encrypt(char* ptext){

	// generate the secret keys and the init vectors
	gen_key();
	gen_iv();

	//get the string length
	size_t ptext_len = strlen(ptext);
	size_t block_len = 0;

	// check for length
	if(ptext_len % 16 != 0) return -1;
	else block_len = ptext_len / 16;

	// copy the state 
	strcpy(text_state, ptext);

	// expand the randomly generated keys
	key_expansion(secret_key);

	// TRANSPOSE ALL BLOCKS
	uint8_t* current_block;
	for(int n = 0 ; n < block_len; n++){
		current_block = text_state + (16*n);
		transpose(current_block);

		// initial round key addition
		add_round_key(current_block, 0);
	}

	

	// ONE BLOCK ONLY
	for(int r = 1 ; r < 10; r++){
		for(int n = 0 ; n < block_len; n++){
			// set the current block
			current_block = text_state + (16*n);

			// perform the cipher
			s_box_sub(current_block);
			shift_rows(current_block);
			mix_columns(current_block);
			add_round_key(current_block, r);
		}
	}
	
	// last round without mix columns
	for(int n = 0 ; n < block_len; n++){
		// set the current block
		current_block = text_state + (16*n);

		// perform the cipher
		s_box_sub(current_block);
		shift_rows(current_block);
		add_round_key(current_block, 10);
	}

	// TRANSPOSE ALL BLOCKS BACK
	for(int n = 0 ; n < block_len; n++){
		current_block = text_state + (16*n);
		transpose(current_block);
	}

	// print the results
	print_encrypt_result(ptext_len);

	return 0;
}

int decrypt(char* ctext, char* iv, char* key){

	//get the string length
	size_t ctext_len = strlen(ctext);
	size_t block_len = 0;

	// check for length
	if(ctext_len % 16 != 0) return -1;
	else block_len = ctext_len / 16;


	// copy all contents to the local arrays
	str_to_byte(key, 16, secret_key);
	str_to_byte(iv, 16, init_vector);
	str_to_byte(ctext, ctext_len, text_state);

	// expand the randomly generated keys
	key_expansion(secret_key);


	// TRANSPOSE ALL BLOCKS
	uint8_t* current_block;
	for(int n = 0 ; n < block_len; n++){
		current_block = text_state + (16*n);
		transpose(current_block);
	}

	// last round without mix columns
	for(int n = 0 ; n < block_len; n++){
		// set the current block
		current_block = text_state + (16*n);

		// perform the cipher
		add_round_key(current_block, 10);
		inv_shift_rows(current_block);
		inv_s_box_sub(current_block);
	}

	// subsequent rounds
	for(int r = 1 ; r < 10; r++){
		for(int n = 0 ; n < block_len; n++){
			// set the current block
			current_block = text_state + (16*n);

			// decipher
			add_round_key(current_block, 10 - r);
			inv_mix_columns(current_block);
			inv_shift_rows(current_block);
			inv_s_box_sub(current_block);
		}
	}
	
	// TRANSPOSE ALL BLOCKS BACK
	for(int n = 0 ; n < block_len; n++){
		
		current_block = text_state + (16*n);

		// add the first round key 
		add_round_key(current_block, 0);

		transpose(current_block);
	}

	print_decrypt_result(ctext_len / 2);
	return 0;
}

