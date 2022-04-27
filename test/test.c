#include "../src/crypt.h"
#include "unity.h"
#include "extras/fixture/src/unity_fixture.h"

#include "stdio.h"
#include "stdlib.h"
 
// Write type
// 0: SEED, 1: AES-128, 2: AES-192, 3: AES-256
// 4: Camellia-128, 5: Camellia-192, 6: Camellia-256
#define SEED 0
#define AES128 1
#define AES192 2
#define AES256 3
#define CAMELLIA128 4
#define CAMELLIA192 5
#define CAMELLIA256 6

uint32_t* key = NULL;
uint32_t* input = NULL;
uint32_t* output = NULL;

void testa_crypt(uint8_t type){
  	uint8_t enc_dec = 1;
  	uint32_t temp[4]; //Vetor temp irá armazenar a mensagem original
  	
  	for (int i=0; i<4; i++)
    		temp[i] = input[i]; //Copia os valores da mensagem no temp
  	
  	int ret = crypt(key, input, type, enc_dec, output); //Chama a função crypt
  	TEST_ASSERT_FALSE(ret); // Deve retornar zero
  	
  	for (int i=0; i<4; i++)
    		input[i] = output[i];
  	
  	enc_dec = 0;
	ret = crypt(key, input, type, enc_dec, output); //Chama a função crypt
	TEST_ASSERT_FALSE(ret); // Deve retornar zero
	
	//Os valores retornados agora devem ser os mesmos da mensagem original
	for(int i = 0; i < 4; i++){
		printf("%08X = %08X\n", output[i], temp[i]);
	}
	
	TEST_ASSERT_EQUAL_UINT32_ARRAY(temp, output, 4);
	
	//Restaura o valor do vetor input
	for (int i=0; i<4; i++)
    		input[i] = temp[i];
 }

TEST_GROUP(Crypt);

TEST_SETUP(Crypt)
{
	printf("\n");
	key = (uint32_t*)malloc(8 * sizeof(uint32_t));
	key[0] = 0xDEADBEEFL;
	key[1] = 0x01234567L; 
	key[2] = 0x89ABCDEFL; 
	key[3] = 0xDEADBEEFL;
	key[4] = 0xDEADBEEFL;
	key[5] = 0x01234567L;
	key[6] = 0x89ABCDEFL;
	key[7] = 0xDEADBEEFL;
	input = (uint32_t*)malloc(4 * sizeof(uint32_t));
	input[0] = 0xA5A5A5A5L;
	input[1] = 0x01234567L;
	input[2] = 0xFEDCBA98L;
	input[3] = 0x5A5A5A5AL;
	output = (uint32_t*)malloc(4 * sizeof(uint32_t));
	printf("\n");
}

TEST_TEAR_DOWN(Crypt)
{
	free(key);
	free(input);
	free(output);
}

TEST(Crypt, TestCrypt1)
{
	printf("SEED:\n");
	testa_crypt(SEED);
	printf("----------------------------------------\n");
	printf("AES-128:\n");
	testa_crypt(AES128);
	printf("----------------------------------------\n");
	printf("AES-192:\n");
	testa_crypt(AES192);
	printf("----------------------------------------\n");
	printf("AES-256:\n");
	testa_crypt(AES256);
	printf("----------------------------------------\n");
	printf("Camelia-128:\n");
	testa_crypt(CAMELLIA128);
	printf("----------------------------------------\n");
	printf("Camelia-192:\n");
	testa_crypt(CAMELLIA192);
	printf("----------------------------------------\n");
	printf("Camelia-256:\n");
	testa_crypt(CAMELLIA256);
	printf("----------------------------------------\n");
	
	TEST_ASSERT_TRUE(crypt(key, input, 7, 0, output)); // Testando com 7 no type (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key, input, 0, 2, output)); // Testando com 2 no enc_dec (valor incorreto)
}

