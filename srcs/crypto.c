#include "woody.h"

void    xtea_encrypt(uint32_t values[2], const uint32_t key[4])
{
    const uint32_t  delta = 0x9E3779B9;
    uint32_t        sum = 0;
    uint32_t        v0 = values[0];
    uint32_t        v1 = values[1];

    dprintf(1, "Value before encryption: %u %u\n", v0, v1);

    for (int i = 0; i < 32; i++)
    {
        v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }

    dprintf(1, "Value after encryption: %u %u\n", v0, v1);

	uint32_t values_bis[2] = {v0, v1};
    xtea_decrypt(values_bis, key);
}

void    xtea_decrypt(uint32_t values[2], const uint32_t key[4])
{
    const uint32_t  delta = 0x9E3779B9;
    uint32_t        sum = delta * 32;
    uint32_t        v0 = values[0];
    uint32_t        v1 = values[1];

    dprintf(1, "Value before decryption: %u %u\n", v0, v1);

    for (int i = 0; i < 32; i++)
    {
        v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
    }

    dprintf(1, "Value after decryption: %u %u\n", v0, v1);
}

void    xtea_encrypt_buff(void *buffer, size_t size, const uint32_t key[4])
{
    const uint32_t  delta = 0x9E3779B9;
    uint32_t        v0, v1;
    uint32_t        sum;
    unsigned char   *buff = (unsigned char *)buffer;

    dprintf(1, "Buffer before encryption:\n");
    for (size_t i = 0; i < size; i++)
        dprintf(1, "%02x ", buff[i]);
    dprintf(1, "\n");

    for (size_t i = 0; i + 8 <= size; i += 8)
    {
        sum = 0;
        v0 = (buff[i] << 24) | (buff[i+1] << 16) | (buff[i+2] << 8) | (buff[i+3]);
        v1 = (buff[i+4] << 24) | (buff[i+5] << 16) | (buff[i+6] << 8) | (buff[i+7]);

        for (int j = 0; j < 32; j++)
        {
            v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
            sum += delta;
            v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
        }

        buff[i] = (v0 >> 24) & 0xFF;
        buff[i+1] = (v0 >> 16) & 0xFF;
        buff[i+2] = (v0 >> 8) & 0xFF;
        buff[i+3] = v0 & 0xFF;
        buff[i+4] = (v1 >> 24) & 0xFF;
        buff[i+5] = (v1 >> 16) & 0xFF;
        buff[i+6] = (v1 >> 8) & 0xFF;
        buff[i+7] = v1 & 0xFF;
    }

    dprintf(1, "Buffer after encryption:\n");
    for (size_t i = 0; i < size; i++)
        dprintf(1, "%02x ", buff[i]);
    dprintf(1, "\n");

    xtea_decrypt_buff(buff, size, key);
}

void    xtea_decrypt_buff(void *buffer, size_t size, const uint32_t key[4])
{
    const uint32_t  delta = 0x9E3779B9;
    uint32_t        v0, v1;
    uint32_t        sum;
    unsigned char   *buff = (unsigned char *)buffer;

    dprintf(1, "Buffer before decryption:\n");
    for (size_t i = 0; i < size; i++)
        dprintf(1, "%02x ", buff[i]);
    dprintf(1, "\n");

    for (size_t i = 0; i + 8 <= size; i += 8)
    {
        sum = delta * 32;
        v0 = (buff[i] << 24) | (buff[i+1] << 16) | (buff[i+2] << 8) | (buff[i+3]);
        v1 = (buff[i+4] << 24) | (buff[i+5] << 16) | (buff[i+6] << 8) | (buff[i+7]);

        for (int j = 0; j < 32; j++)
        {
            v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
            sum -= delta;
            v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
        }

        buff[i] = (v0 >> 24) & 0xFF;
        buff[i+1] = (v0 >> 16) & 0xFF;
        buff[i+2] = (v0 >> 8) & 0xFF;
        buff[i+3] = v0 & 0xFF;
        buff[i+4] = (v1 >> 24) & 0xFF;
        buff[i+5] = (v1 >> 16) & 0xFF;
        buff[i+6] = (v1 >> 8) & 0xFF;
        buff[i+7] = (v1 & 0xFF);
    }

    dprintf(1, "Buffer after decryption:\n");
    for (size_t i = 0; i < size; i++)
        dprintf(1, "%02x ", buff[i]);
    dprintf(1, "\n");
}