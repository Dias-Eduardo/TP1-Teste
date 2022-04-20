// FILE NAME  : seed.c
// AUTHOR     : Rafael Garibotti
// DEVELOPERS : Rafael Garibotti
// E-mail     : rafael.garibotti@pucrs.br
//-----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION   DATE         DESCRIPTION
// 1.0       2022-04-11   Initial version.
//-----------------------------------------------------------------------------

#include "seed.h"

//-----------------------------------------------------------------------------
// SEED: 128-bits
//-----------------------------------------------------------------------------

// S-Box substitution layer
static uint32_t
G(uint32_t x) {
	return ss0[x & 0xFF] ^ ss1[(x >> 8) & 0xFF] ^ ss2[(x >> 16) & 0xFF] ^ ss3[(x >> 24) & 0xFF];
}

// Diffusion layer
static void
F(uint32_t R0, uint32_t R1, uint32_t Ki0, uint32_t Ki1, uint32_t* out0, uint32_t* out1) {
	*out1 = (R0 ^ Ki0) ^ (R1 ^ Ki1);
	*out1 = G(*out1);
	*out0 = *out1 + (R0 ^ Ki0);
	*out0 = G(*out0);
	*out1 += *out0;
	*out1 = G(*out1);
	*out0 += *out1;
}

void
seed_init(SeedContext* context, uint32_t* key) {
	const uint32_t keys[4] = { key[0], key[1], key[2], key[3] };
	uint32_t temp;
	int i;

	for (i = 0; i < 16; i++) {
		// generate subkeys as high and low nibbles, making it as 32 subkeys
		context->subkeys[i * 2] = G(keys[0] + keys[2] - KC[i]);
		context->subkeys[i * 2 + 1] = G(keys[1] - keys[3] + KC[i]);

		if (i % 2 != 0) {
			// odd rounds: Key0 || Key1 = (Key0 || Key1) >>> 8
			temp = key[1] >> 8 | key[0] << 24;
			key[0] = key[0] >> 8 | key[1] << 24;
			key[1] = temp;
		}
		else {
			// even rounds: Key2 || Key3 = (Key2 || Key3) <<< 8
			temp = key[3] >> 8 | key[2] << 24;
			key[2] = key[2] >> 8 | key[3] << 24;
			key[3] = temp;
		}
	}
}

void
seed_enc(SeedContext* context, const uint32_t* block, uint32_t* out) {
	int i;
	uint32_t temp0;
	uint32_t temp1;
	// subkey is ascending in encryption
	uint32_t subkey = 0;
	// left 64 bits of block divided into 2 32 bits parts
	uint32_t l0 = block[0];
	uint32_t l1 = block[1];
	// right 64 bits of block divided into 2 32 bits parts
	uint32_t r0 = block[2];
	uint32_t r1 = block[3];

	for (i = 0; i < NR_ROUNDS - 1; i++) {
		F(r0, r1, context->subkeys[subkey], context->subkeys[subkey + 1], &temp0, &temp1);

		temp0 ^= l0;
		temp1 ^= l1;

		l0 = r0;
		l1 = r1;

		r0 = temp0;
		r1 = temp1;

		subkey += 2;
	}

	// last round we update l instead of r
	F(r0, r1, context->subkeys[subkey], context->subkeys[subkey + 1], &temp0, &temp1);

	l0 ^= temp0;
	l1 ^= temp1;

	out[0] = l0;
	out[1] = l1;
	out[2] = r0;
	out[3] = r1;
}

void
seed_dec(SeedContext* context, const uint32_t* block, uint32_t* out) {
	int i;
	uint32_t temp0;
	uint32_t temp1;
	// subkey is descending in decryption
	uint32_t subkey = 31;
	// left 64 bits of block divided into 2 32 bits parts
	uint32_t l0 = block[0];
	uint32_t l1 = block[1];
	// right 64 bits of block divided into 2 32 bits parts
	uint32_t r0 = block[2];
	uint32_t r1 = block[3];

	for (i = 0; i < NR_ROUNDS - 1; i++) {
		F(r0, r1, context->subkeys[subkey - 1], context->subkeys[subkey], &temp0, &temp1);

		temp0 ^= l0;
		temp1 ^= l1;

		l0 = r0;
		l1 = r1;

		r0 = temp0;
		r1 = temp1;

		subkey -= 2;
	}

	// last round we update l instead of r
	F(r0, r1, context->subkeys[subkey - 1], context->subkeys[subkey], &temp0, &temp1);

	l0 ^= temp0;
	l1 ^= temp1;

	out[0] = l0;
	out[1] = l1;
	out[2] = r0;
	out[3] = r1;
}

void
seed(uint32_t* key, uint32_t* input, uint8_t enc_dec, uint32_t *output) {
  SeedContext context;
  
  seed_init(&context, key);
  if (enc_dec)
    seed_enc(&context, input, output);
  else
    seed_dec(&context, input, output);
}