// FILE NAME  : camellia.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2022-04-11   Initial version.
//-----------------------------------------------------------------------------

#include "camellia.h"

//-----------------------------------------------------------------------------
// Camellia: 128/192/256-bits
//-----------------------------------------------------------------------------

// Rotate Left circular shift 32 bits
static uint32_t ROL_32(uint32_t x, uint32_t n)
{
	return x << n | x >> (32 - n);
}

// Rotate Left circular shift 128 bits
void ROL_128(uint64_t* y, const uint64_t* x, uint32_t n)
{
	uint64_t temp = x[0];
	y[0] = (x[0] << n) | (x[1] >> (64 - n));
	y[1] = (x[1] << n) | (temp >> (64 - n));
}

uint64_t F(uint64_t F_IN, uint64_t KE)
{
	uint64_t x;
	uint8_t t1, t2, t3, t4, t5, t6, t7, t8;
	uint8_t y1, y2, y3, y4, y5, y6, y7, y8;

	x = F_IN ^ KE;
	t1 = x >> 56;
	t2 = (uint8_t)(x >> 48);
	t3 = (uint8_t)(x >> 40);
	t4 = (uint8_t)(x >> 32);
	t5 = (uint8_t)(x >> 24);
	t6 = (uint8_t)(x >> 16);
	t7 = (uint8_t)(x >> 8);
	t8 = (uint8_t)x;
	t1 = sbox1[t1];
	t2 = sbox2[t2];
	t3 = sbox3[t3];
	t4 = sbox4[t4];
	t5 = sbox2[t5];
	t6 = sbox3[t6];
	t7 = sbox4[t7];
	t8 = sbox1[t8];
	y1 = t1 ^ t3 ^ t4 ^ t6 ^ t7 ^ t8;
	y2 = t1 ^ t2 ^ t4 ^ t5 ^ t7 ^ t8;
	y3 = t1 ^ t2 ^ t3 ^ t5 ^ t6 ^ t8;
	y4 = t2 ^ t3 ^ t4 ^ t5 ^ t6 ^ t7;
	y5 = t1 ^ t2 ^ t6 ^ t7 ^ t8;
	y6 = t2 ^ t3 ^ t5 ^ t7 ^ t8;
	y7 = t3 ^ t4 ^ t5 ^ t6 ^ t8;
	y8 = t1 ^ t4 ^ t5 ^ t6 ^ t7;
	return ((uint64_t)y1 << 56) | ((uint64_t)y2 << 48) | ((uint64_t)y3 << 40) | ((uint64_t)y4 << 32) |
		((uint64_t)y5 << 24) | ((uint64_t)y6 << 16) | ((uint64_t)y7 << 8) | y8;
}

uint64_t FL(uint64_t FL_IN, uint64_t KE)
{
	uint32_t x1, x2;
	uint32_t k1, k2;
	x1 = FL_IN >> 32;
	x2 = (uint32_t)FL_IN;
	k1 = KE >> 32;
	k2 = (uint32_t)KE;
	x2 = x2 ^ ROL_32((x1 & k1), 1);
	x1 = x1 ^ (x2 | k2);
	return ((uint64_t)x1 << 32) | x2;
}

uint64_t FLINV(uint64_t FLINV_IN, uint64_t KE)
{
	uint32_t y1, y2;
	uint32_t k1, k2;
	y1 = FLINV_IN >> 32;
	y2 = (uint32_t)FLINV_IN;
	k1 = KE >> 32;
	k2 = (uint32_t)KE;
	y1 = y1 ^ (y2 | k2);
	y2 = y2 ^ ROL_32((y1 & k1), 1);
	return ((uint64_t)y1 << 32) | y2;
}

void camellia_init(CamelliaContext* context, const uint64_t* key, uint16_t keyLen)
{
	uint8_t i;
	uint64_t temp[2];

	uint64_t KL[2];
	uint64_t KR[2];
	uint64_t KA[2];
	uint64_t KB[2];
	uint64_t D1;
	uint64_t D2;

	// generate KL and KR
	if (keyLen == 128)
	{
		// 18 (nr rounds) / 6 (nr rounds required for each feistel iteration)
		context->feistelIterations = 3;
		context->nrSubkeys = 26;

		KL[0] = key[0];
		KL[1] = key[1];
		KR[0] = 0;
		KR[1] = 0;
	}
	else if (keyLen == 192 || keyLen == 256)
	{
		// 24 (nr rounds) / 6 (nr rounds required for each feistel iteration)
		context->feistelIterations = 4;
		context->nrSubkeys = 34;

		KL[0] = key[0];
		KL[1] = key[1];
		KR[0] = key[2];
		KR[1] = key[3];

		// special treatment for 192-bits key
		if (keyLen == 192)
		{
			KR[1] = ~key[2];
		}
	}
	else
	{
		//TODO create return status
		return;
	}

	// generate KA and KB
	D1 = KL[0] ^ KR[0];
	D2 = KL[1] ^ KR[1];
	D2 = D2 ^ F(D1, sigma[0]);
	D1 = D1 ^ F(D2, sigma[1]);
	D1 = D1 ^ KL[0];
	D2 = D2 ^ KL[1];
	D2 = D2 ^ F(D1, sigma[2]);
	D1 = D1 ^ F(D2, sigma[3]);
	KA[0] = D1;
	KA[1] = D2;
	D1 = KA[0] ^ KR[0];
	D2 = KA[1] ^ KR[1];
	D2 = D2 ^ F(D1, sigma[4]);
	D1 = D1 ^ F(D2, sigma[5]);
	KB[0] = D1;
	KB[1] = D2;

	// generate subkeys
	i = 0;
	if (keyLen == 128)
	{
		context->k[i++] = KL[0];
		context->k[i++] = KL[1];
		context->k[i++] = KA[0];
		context->k[i++] = KA[1];
		ROL_128(temp, KL, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 30 + 15 = 45 left circular shift
		ROL_128(temp, temp, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 30 + 15 = 45 left circular shift
		ROL_128(temp, temp, 15);
		context->k[i++] = temp[0];
		ROL_128(temp, KL, 30); // 30 + 30 = 45 left circular shift
		ROL_128(temp, temp, 30);
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 30 + 30 = 45 left circular shift
		ROL_128(temp, temp, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 30 + 30 + 17 = 77 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 17);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 30 + 30 + 30 + 4 = 94 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 4);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 30 + 30 + 30 + 4 = 94 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 4);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 30 + 30 + 30 + 21 = 111 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 21);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 30 + 30 + 30 + 21 = 111 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 21);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
	}
	else
	{
		context->k[i++] = KL[0];
		context->k[i++] = KL[1];
		context->k[i++] = KB[0];
		context->k[i++] = KB[1];
		ROL_128(temp, KR, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KR, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KB, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30);
		ROL_128(temp, temp, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30);
		ROL_128(temp, temp, 15);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30);
		ROL_128(temp, temp, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KR, 30);
		ROL_128(temp, temp, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KB, 30);
		ROL_128(temp, temp, 30);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 60 + 17 = 77 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 17);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 60 + 17 = 77 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 17);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KR, 30); // 60 + 34 = 94 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 34);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KA, 30); // 60 + 34 = 94 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 34);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KL, 30); // 60 + 51 = 111 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 21);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
		ROL_128(temp, KB, 30); // 60 + 51 = 111 left circular shift
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 30);
		ROL_128(temp, temp, 21);
		context->k[i++] = temp[0];
		context->k[i++] = temp[1];
	}
}

void camellia_enc(const CamelliaContext* context, const uint64_t* block, uint64_t* out)
{
	// D[0] is D1 and D[1] is D2
	uint64_t D[2] = { block[0], block[1] };
	uint16_t subkey = 0;
	uint16_t dIndex;
	uint16_t oppositeIndex;
	uint16_t round;
	uint16_t feistelIteration;

	D[0] ^= context->k[subkey++]; // Prewhitening
	D[1] ^= context->k[subkey++];

	// if 128-bits key then its 18 rounds divided into 3 feistel iterations
	// if 192/256-bits key then its 24 rounds and divided into 4 feistel iterations
	for (feistelIteration = 0; feistelIteration < context->feistelIterations; feistelIteration++)
	{
		// each feistel iteration is 6 rounds
		for (round = 1; round <= 6; round++)
		{
			// calculate index
			dIndex = round % 2;
			oppositeIndex = (~dIndex & 0x1);

			// D1 is calculated in even rounds and D2 in odd rounds
			D[dIndex] ^= F(D[oppositeIndex], context->k[subkey++]);
		}

		// do not insert FL and FLINV functions in last iteration
		if (feistelIteration != (context->feistelIterations - 1))
		{
			// between each feistel iteration FL and FLINV functions are inserted
			D[0] = FL(D[0], context->k[subkey++]);
			D[1] = FLINV(D[1], context->k[subkey++]);
		}
	}

	D[1] ^= context->k[subkey++]; // Postwhitening
	D[0] ^= context->k[subkey++];

	// copy cipher text to output
	out[0] = D[1];
	out[1] = D[0];
}

void camellia_dec(const CamelliaContext* context, const uint64_t* block, uint64_t* out)
{
	// D[0] is D1 and D[1] is D2
	uint64_t D[2] = { block[0], block[1] };
	uint16_t subkey = context->nrSubkeys - 1;
	uint16_t dIndex;
	uint16_t oppositeIndex;
	uint16_t round;
	uint16_t feistelIteration;

	// Prewhitening
	D[1] ^= context->k[subkey--];
	D[0] ^= context->k[subkey--];

	// if 128-bits key then its 18 rounds divided into 3 feistel iterations
	// if 192/256-bits key then its 24 rounds and divided into 4 feistel iterations
	for (feistelIteration = 0; feistelIteration < context->feistelIterations; feistelIteration++)
	{
		// each feistel iteration is 6 rounds
		for (round = 1; round <= 6; round++)
		{
			// calculate index
			dIndex = round % 2;
			oppositeIndex = (~dIndex & 0x1);

			// D1 is calculated in even rounds and D2 in odd rounds
			D[dIndex] ^= F(D[oppositeIndex], context->k[subkey--]);
		}

		// do not insert FL and FLINV functions in last iteration
		if (feistelIteration != (context->feistelIterations - 1))
		{
			// between each feistel iteration FL and FLINV functions are inserted
			D[0] = FL(D[0], context->k[subkey--]);
			D[1] = FLINV(D[1], context->k[subkey--]);
		}
	}

	// Postwhitening
	D[0] ^= context->k[subkey--];
	D[1] ^= context->k[subkey--];

	// copy cipher text to output
	out[0] = D[1];
	out[1] = D[0];
}

void
camellia(uint32_t* key, uint32_t* input, uint8_t type, uint8_t enc_dec, uint32_t *output) {
  CamelliaContext context;
  int i, keysize;
  uint64_t k[4];
  uint64_t v[2];
  uint64_t d[2];
  
  // 4 to 128 bits, 5 to 192 bits, 6 to 256 bits
  if (type == 6) {
    keysize = 256;
  }
  else if (type == 5) {
	keysize = 192;
  }
  else {
	keysize = 128;
  }
  
  for(i = 0; i < type-2; i++) {
    k[i] = ((uint64_t)key[2*i] << 32) + (uint64_t)key[2*i+1];
  }
  v[0] = ((uint64_t)input[0] << 32) + (uint64_t)input[1];
  v[1] = ((uint64_t)input[2] << 32) + (uint64_t)input[3];

  camellia_init(&context, k, keysize);

  if (enc_dec)
    camellia_enc(&context, v, d);
  else
    camellia_dec(&context, v, d);
  
  output[0] = (uint32_t)(d[0] >> 32);
  output[1] = (uint32_t)(d[0]);
  output[2] = (uint32_t)(d[1] >> 32);
  output[3] = (uint32_t)(d[1]);
}