#include "woody.h"

void    xtea_decrypt_block(uint32_t values[2], const uint32_t key[4])
{
    const uint32_t  delta = 0x9E3779B9;
    uint32_t        sum = delta * 32;
    uint32_t        v0 = values[0];
    uint32_t        v1 = values[1];
	const uint32_t  delta = 0x9E3779B9;
	uint32_t        sum = delta * 32;
	uint32_t        v0 = values[0];
	uint32_t        v1 = values[1];

    dprintf(1, "Value before decryption: %u %u\n", v0, v1);
	dprintf(1, "Value before decryption: %u %u\n", v0, v1);

    for (int i = 0; i < 32; i++)
    {
        v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
    }
	for (int i = 0; i < 32; i++)
	{
		v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
		sum -= delta;
		v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
	}

    dprintf(1, "Value after decryption: %u %u\n", v0, v1);
	dprintf(1, "Value after decryption: %u %u\n", v0, v1);
}