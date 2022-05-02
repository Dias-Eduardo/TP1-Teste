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

uint32_t* key256 = NULL;
uint32_t* key192 = NULL;
uint32_t* key128 = NULL;
uint32_t* input = NULL;
uint32_t* output = NULL;

void testa_crypt(uint32_t* key, uint8_t type, const uint32_t* checker){
  	uint8_t enc_dec = 1;
  	uint32_t temp[4]; //Vetor temp irá armazenar a mensagem original
  	
  	//Printa a mensagem original
  	printf("Mensagem original:       ");
  	for(int i = 0; i < 4; i++){
  		printf(" %08X", input[i]);
  	}
  	printf("\n");
  	
  	//Copia os valores da mensagem no temp
  	for (int i=0; i<4; i++)
    		temp[i] = input[i]; 
  	
  	//Chama a função crypt para criptografar a mensagem com o type escolhido
  	int ret = crypt(key, input, type, enc_dec, output); 
  	TEST_ASSERT_FALSE(ret); // Deve retornar zero
  	
  	//Checa se o vetor retornado pela crypt corresponde ao checker enviado
  	TEST_ASSERT_EQUAL_UINT32_ARRAY(checker, output, 4);
  	
  	//Printa a mensagem criptografada
  	printf("Mensagem criptografada:  ");
  	for(int i = 0;i<4;i++)
    		printf(" %08X", output[i]);
  	printf("\n");
  	
  	for (int i=0; i<4; i++)
    		input[i] = output[i];
	
	//Chama a função crypt para decriptografar a mensagem criptografada anteriormente
  	enc_dec = 0;
	ret = crypt(key, input, type, enc_dec, output); 
	TEST_ASSERT_FALSE(ret); // Deve retornar zero

	//Checa se o vetor decriptado retornado corresponde à mensagem original
	TEST_ASSERT_EQUAL_UINT32_ARRAY(temp, output, 4);
	
	//Por último printa-se a mensagem decriptografada, que é igual a mensagem original (pelo menos é para ser)
	printf("Mensagem decriptografada:");
  	for(int i = 0;i<4;i++)
    		printf(" %08X", output[i]);
  	printf("\n");
	
	//Restaura o valor do vetor input
	for (int i=0; i<4; i++)
    		input[i] = temp[i];
 }

TEST_GROUP(Crypt);

TEST_SETUP(Crypt)
{
	printf("\n");
	key256 = (uint32_t*)malloc(8 * sizeof(uint32_t));
	key256[0] = 0xDEADBEEFL;
	key256[1] = 0x01234567L; 
	key256[2] = 0x89ABCDEFL; 
	key256[3] = 0xDEADBEEFL;
	key256[4] = 0xDEADBEEFL;
	key256[5] = 0x01234567L;
	key256[6] = 0x89ABCDEFL;
	key256[7] = 0xDEADBEEFL;
	key192 = (uint32_t*)malloc(6 * sizeof(uint32_t));
	key192[0] = 0xDEADBEEFL;
	key192[1] = 0x01234567L; 
	key192[2] = 0x89ABCDEFL; 
	key192[3] = 0xDEADBEEFL;
	key192[4] = 0xDEADBEEFL;
	key192[5] = 0x01234567L;
	key128 = (uint32_t*)malloc(4 * sizeof(uint32_t));
	key128[0] = 0xDEADBEEFL;
	key128[1] = 0x01234567L; 
	key128[2] = 0x89ABCDEFL; 
	key128[3] = 0xDEADBEEFL;
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
	free(key256);
	free(key192);
	free(key128);
	free(input);
	free(output);
}

TEST(Crypt, TestCrypt1)
{
	const uint32_t checker_SEED[4] = {0xD7B44460L, 0X6DE0D144L, 0x05D564C0L, 0x85FDC648L};
	const uint32_t checker_AES128[4] = {0x237549D4L, 0XCDCEA7BEL, 0x0FE7D162L, 0xCC9161D3L}; 
	const uint32_t checker_AES192[4] = {0xEBB4CD9EL, 0XDA7CBDE0L, 0x9E520F4CL, 0x54BE5A98L};
	const uint32_t checker_AES256[4] = {0x206BF119L, 0X7A085AC8L, 0xB31B2AD0L, 0x4843FFC1L}; 
	const uint32_t checker_CAMELLIA128[4] = {0xF44A91BCL, 0X2491ED44L, 0x8817C69EL, 0xD406846AL}; 
	const uint32_t checker_CAMELLIA192[4] = {0x63A02B61L, 0XC98393C0L, 0x9B58AE1DL, 0xDE934207L}; 
	const uint32_t checker_CAMELLIA256[4] = {0xEB79D244L, 0XB33BC5BAL, 0xEDF572EEL, 0x01CBC9F6L}; 

	printf("SEED:\n");
	testa_crypt(key128, SEED, checker_SEED);
	printf("----------------------------------------\n");
	printf("AES-128:\n");
	testa_crypt(key128, AES128, checker_AES128);		//Teste adicional
	printf("----------------------------------------\n");
	printf("AES-192:\n");
	testa_crypt(key192, AES192, checker_AES192);		//Teste adicional
	printf("----------------------------------------\n");
	printf("AES-256:\n");
	testa_crypt(key256, AES256, checker_AES256);		//Teste adicional
	printf("----------------------------------------\n");
	printf("Camellia-128:\n");
	testa_crypt(key128, CAMELLIA128, checker_CAMELLIA128);//Teste adicional
	printf("----------------------------------------\n");
	printf("Camellia-192:\n");
	testa_crypt(key192, CAMELLIA192, checker_CAMELLIA192);//Teste adicional
	printf("----------------------------------------\n");
	printf("Camellia-256:\n");
	testa_crypt(key256, CAMELLIA256, checker_CAMELLIA256);
	printf("----------------------------------------\n");

	TEST_ASSERT_TRUE(crypt(key128, input, -1, 0, output)); // Testando com -1 no type (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, 7, 0, output)); // Testando com 7 no type (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, SEED, -1, output)); // Testando com -1 no enc_dec (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, SEED, 2, output)); // Testando com 2 no enc_dec (valor incorreto)
	
	TEST_ASSERT_TRUE(crypt(key128, input, -1, 1, output)); // Testando com -1 no type (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, 7, 1, output)); // Testando com 7 no type (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, CAMELLIA256, -1, output)); // Testando com -1 no enc_dec (valor incorreto)
	TEST_ASSERT_TRUE(crypt(key128, input, CAMELLIA256, 2, output)); // Testando com 2 no enc_dec (valor incorreto)
	
}

