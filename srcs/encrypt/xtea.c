#include "woody.h"

void    generate_key(uint32_t key[4])
{
	int         fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return ;

	ssize_t rd = read(fd, key, sizeof(uint32_t) * 4);
    if (rd != sizeof(uint32_t) * 4)
		bzero(key, sizeof(uint32_t) * 4);

	close(fd);
}

void	xtea_encrypt_buff(void *buffer, size_t size, const uint32_t key[4])
{
	const uint32_t  delta = 0x9E3779B9;
	uint32_t        v0, v1;
	uint32_t        sum;

	unsigned char	*tmp_buffer = (unsigned char *)buffer;

	for (size_t i = 0; i + 8 <= size; i += 8)
	{
		sum = 0;
		v0 = (tmp_buffer[i] << 24) | (tmp_buffer[i+1] << 16) | (tmp_buffer[i+2] << 8) | (tmp_buffer[i+3]);
		v1 = (tmp_buffer[i+4] << 24) | (tmp_buffer[i+5] << 16) | (tmp_buffer[i+6] << 8) | (tmp_buffer[i+7]);

		for (int j = 0; j < 32; j++)
		{
			v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
			sum += delta;
			v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
		}

		tmp_buffer[i] = (v0 >> 24) & 0xFF;
		tmp_buffer[i+1] = (v0 >> 16) & 0xFF;
		tmp_buffer[i+2] = (v0 >> 8) & 0xFF;
		tmp_buffer[i+3] = v0 & 0xFF;
		tmp_buffer[i+4] = (v1 >> 24) & 0xFF;
		tmp_buffer[i+5] = (v1 >> 16) & 0xFF;
		tmp_buffer[i+6] = (v1 >> 8) & 0xFF;
		tmp_buffer[i+7] = v1 & 0xFF;
	}
}
