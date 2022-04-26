#include "../src/crypt.h"
#include "unity.h"
#include "extras/fixture/src/unity_fixture.h"

#include "stdio.h"
#include "stdlib.h"
 
 // Write type
 // 0: SEED, 1: AES-128, 2: AES-192, 3: AES-256
 // 4: Camellia-128, 5: Camellia-192, 6: Camellia-256
 

int keySize = 256; //Número de elementos da chave em bytes
uint32_t* key = NULL;
int inputSize = 128;//Número de elementos da entrada em bytes
uint32_t* input = NULL;
uint32_t* output = NULL;
 
void testa_crypt(uint32_t* key, uint32_t* input, uint8_t type, uint32_t* output){
  	uint8_t enc_dec = 1;
  	uint32_t temp[4]; //Vetor temp irá armazenar a mensagem original
  	
  	for (int i=0; i<4; i++)
    		temp[i] = input[i]; //Copia os valores da mensagem no temp
  	 	
  	int ret = crypt(key, input, type, enc_dec, output);
  	TEST_ASSERT_FALSE(ret); // Deve retornar zero
  	
  	 for (int i=0; i<4; i++)
    		input[i] = output[i];
  	
  	enc_dec = 0;
	ret = crypt(key, input, type, enc_dec, output);
	TEST_ASSERT_FALSE(ret); // Deve retornar zero
	
	//Os valores retornados agora devem ser os mesmos da mensagem original
	for(int i = 0; i < 4; i++){
		printf("%08X = %08X\n", output[i], temp[i]);
	}
	
	TEST_ASSERT_EQUAL_UINT32_ARRAY(temp, output, inputSize / 32);
	
	//Restaura o valor do vetor input
	for (int i=0; i<4; i++)
    		input[i] = temp[i];
 }

TEST_GROUP(Crypt);

TEST_SETUP(Crypt)
{
	key = (uint32_t*)malloc(keySize);
	key[0] = 0xDEADBEEFL;
	key[1] = 0x01234567L; 
	key[2] = 0x89ABCDEFL; 
	key[3] = 0xDEADBEEFL;
	key[4] = 0xDEADBEEFL;
	key[5] = 0x01234567L;
	key[6] = 0x89ABCDEFL;
	key[7] = 0xDEADBEEFL;
	input = (uint32_t*)malloc(inputSize);
	input[0] = 0xA5A5A5A5L;
	input[1] = 0x01234567L;
	input[2] = 0xFEDCBA98L;
	input[3] = 0x5A5A5A5AL;
	output = (uint32_t*)malloc(inputSize);
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
	testa_crypt(key, input, 0, output);
	printf("----------------------------------------\n");
	printf("AES-128:\n");
	testa_crypt(key, input, 1, output);
	printf("----------------------------------------\n");
	printf("AES-192:\n");
	testa_crypt(key, input, 2, output);
	printf("----------------------------------------\n");
	printf("AES-256:\n");
	testa_crypt(key, input, 3, output);
	printf("----------------------------------------\n");
	printf("Camelia-128:\n");
	testa_crypt(key, input, 4, output);
	printf("----------------------------------------\n");
	printf("Camelia-192:\n");
	testa_crypt(key, input, 5, output);
	printf("----------------------------------------\n");
	printf("Camelia-256:\n");
	testa_crypt(key, input, 6, output);
	printf("----------------------------------------\n");
	
	crypt(key, input, 7, 0, output); //Testando com 7 no type (valor incorreto)
	crypt(key, input, 0, 2, output); // Testando com 2 no enc_dec (valor incorreto)
}

