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
extern int encrypt(char* ptext);
extern int decrypt(char* ctext, char* iv, char* key, bool random_mode);
extern int random_ops();

#endif // AES_H