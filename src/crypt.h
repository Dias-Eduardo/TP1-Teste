// FILE NAME  : crypt.h
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2022-04-11   Initial version.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void aes(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output);

void camellia(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t *output);

void seed(uint32_t* key, uint32_t* input, uint8_t enc_dec, uint32_t *output);

int crypt(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output);
