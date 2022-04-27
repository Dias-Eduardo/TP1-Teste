// FILE NAME  : crypt.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2022-04-11   Initial version.
//-----------------------------------------------------------------------------

#include "crypt.h"

//-----------------------------------------------------------------------------
// CRYPT
//-----------------------------------------------------------------------------
int crypt(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t* output) {
  if (enc_dec == 0 || enc_dec == 1){
    switch (type) {
      case 0  : seed(key, input, enc_dec, output); break;
	  case 1  : aes(key, input, type, enc_dec, output); break;
	  case 2  : aes(key, input, type, enc_dec, output); break;
	  case 3  : aes(key, input, type, enc_dec, output); break;
	  case 4  : camellia(key, input, type, enc_dec, output); break;
	  case 5  : camellia(key, input, type, enc_dec, output); break;
	  case 6  : camellia(key, input, type, enc_dec, output); break;
	  default : return 1; //Undefined type
    }
  } else {
	return 1; //enc_dec argument must be 0 or 1
  }
  return 0;
}
