#include "woody.h"

void    generate_key(uint32_t key[4])
{
	int         fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		bzero(key, sizeof(uint32_t) * 4);
		return ;
	}

	ssize_t rd = read(fd, key, sizeof(uint32_t) * 4);
    if (rd != sizeof(uint32_t) * 4)
		bzero(key, sizeof(uint32_t) * 4);

	close(fd);
}

void	xtea_encrypt_buff(void *buffer, char *path, size_t size, const uint32_t key[4], int padding)
{
	const uint32_t  delta = 0x9E3779B9;
	uint32_t        v0, v1;
	uint32_t        sum;
	int				fd;
	unsigned char   buff[4096];

	bzero(buff, 4096);
	memcpy(buff, buffer, size);

	for (size_t i = size; i < size + padding; i++)
		buff[i] = (unsigned char)padding;
	size += padding;

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

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		dprintf(2, "Couldn't create encrypted_secret file\n");
		return ;
	}
	write(fd, buff, size);
	close(fd);
	
}

void    xtea_decrypt_buff(char	*path, size_t size, const uint32_t key[4], int padding)
{
	const uint32_t  delta = 0x9E3779B9;
	uint32_t        v0, v1;
	uint32_t        sum;
	unsigned char   *buff;
	int				fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		dprintf(2, "Couldn't open encrypted_secret for reading\n");
		return ;
	}
	buff = malloc(size);
	read(fd, buff, size);
	close(fd);

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

	size -= padding;
	buff[size] = '\0';
}