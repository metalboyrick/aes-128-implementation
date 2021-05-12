#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// helper variables
extern uint8_t init_vector[16];
extern uint8_t secret_key[16];

// interface functions for encrypting and decrypting
extern char* encrypt(char* ptext);
extern char* decrypt(char* ctext, char* iv, char* key);

#endif // AES_H