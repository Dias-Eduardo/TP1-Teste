// FILE NAME  : main.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2022-04-11   Initial version.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "../src/crypt.h"

//-----------------------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------------------
int
main(void) {
  int i, type, enc_dec;
  unsigned int cipher[4];

  printf("-- Crypt. Algorithms (START) --\n");

  // Write key
  unsigned int key[] = {
    0xDEADBEEFL, 0x01234567L, 0x89ABCDEFL
  };

  // Write type
  // 0: SEED, 1: AES-128, 2: AES-192, 3: AES-256
  // 4: Camellia-128, 5: Camellia-192, 6: Camellia-256
  type = 3;

  //---------------------------------------------------------------------------
  printf("\n-- AES-256 (ENC) --\n");

  // Write plantext
  unsigned int plan[4] = {0xA5A5A5A5L, 0x01234567L, 0xFEDCBA98L, 0x5A5A5A5AL};
  printf("%08X %08X %08X %08X (Plan)\n", plan[0], plan[1], plan[2], plan[3]);

  // Write enc_dec
  enc_dec = 1;

  // Call Crypt. Function
  crypt(key, plan, type, enc_dec, cipher);
  
  printf("%08X %08X %08X %08X (Cipher)\n", cipher[0], cipher[1], cipher[2], cipher[3]);
  
  if ((cipher[0] != 0x206BF119) || (cipher[1] != 0x7A085AC8) ||
	  (cipher[2] != 0xB31B2AD0) || (cipher[3] != 0x4843FFC1)) {
    printf("CRYPT: ERROR!!!\n"); 	  	
  }
  else {
  	printf("CRYPT: OK\n");
  }

  //---------------------------------------------------------------------------
  printf("\n-- AES-256 (DEC) --\n");

  // Write plantext
  for (i=0; i<4; i++)
    plan[i] = cipher[i];
  printf("%08X %08X %08X %08X (Plan)\n", plan[0], plan[1], plan[2], plan[3]);

  // Write enc_dec
  enc_dec = 0;

  // Call Crypt. Function
  crypt(key, plan, type, enc_dec, cipher);
  
  printf("%08X %08X %08X %08X (Cipher)\n", cipher[0], cipher[1], cipher[2], cipher[3]);
  
  if ((cipher[0] != 0xA5A5A5A5) || (cipher[1] != 0x01234567) ||
	  (cipher[2] != 0xFEDCBA98) || (cipher[3] != 0x5A5A5A5A)) {
    printf("CRYPT: ERROR!!!\n"); 	  	
  }
  else {
  	printf("CRYPT: OK\n");
  }

  printf("\n-- Crypt. Algorithms (END) --\n"); 
  return 0;
}
