#include "../src/crypt.h"
#include "unity.h"
#include "extras/fixture/src/unity_fixture.h"

#include <stdio.h>
 
 // Write type
 // 0: SEED, 1: AES-128, 2: AES-192, 3: AES-256
 // 4: Camellia-128, 5: Camellia-192, 6: Camellia-256
 
 void testa_crypt(uint32_t* key, uint32_t* input, uint8_t type){
  	uint8_t enc_dec = 1;
  	uint32_t cipher[4], temp[4]; //Vetor temp irá armazenar a mensagem original
  	
  	for (int i=0; i<4; i++)
    		temp[i] = input[i]; //Copia os valores da mensagem no temp
  	 	
  	int ret = crypt(key, input, type, enc_dec, cipher);
  	TEST_ASSERT_FALSE(ret); // Deve retornar zero
  	
  	 for (int i=0; i<4; i++)
    		input[i] = cipher[i];
  	
  	enc_dec = 0;
	ret = crypt(key, input, type, enc_dec, cipher);
	TEST_ASSERT_FALSE(ret); // Deve retornar zero
	
	//Os valores retornados agora devem ser os mesmos da mensagem original
	for(int i = 0; i < 4; i++){
		printf("%08X = %08X\n", cipher[i], temp[i]);
	}
	
	TEST_ASSERT_EQUAL_UINT32_ARRAY(temp, cipher, 4);
	
	//Restaura o valor do vetor input
	for (int i=0; i<4; i++)
    		input[i] = temp[i];
 }

TEST_GROUP(Crypt);

TEST_SETUP(Crypt)
{
	printf("\n");
}

TEST_TEAR_DOWN(Crypt)
{
}

TEST(Crypt, TestCrypt1)
{
	unsigned int plan[4] = {0xA5A5A5A5L, 0x01234567L, 0xFEDCBA98L, 0x5A5A5A5AL};
	unsigned int key[8] = {
	    0xDEADBEEFL, 0x01234567L, 0x89ABCDEFL, 0xDEADBEEFL,
	    0xDEADBEEFL, 0x01234567L, 0x89ABCDEFL, 0xDEADBEEFL
	};
	
	testa_crypt(key, plan, 3);
	printf("----------------------------------------\n");
	testa_crypt(key, plan, 0);

	
}

TEST(Crypt, TestCrypt2)
{

}
