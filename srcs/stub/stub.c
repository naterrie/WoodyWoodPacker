#include <sys/mman.h>
#include <unistd.h>

typedef unsigned long  uint64_t;
typedef unsigned int   uint32_t;
typedef unsigned long  size_t;

typedef struct  s_stub_metadata {
    unsigned char *text_offset;
    size_t         text_size;
    void          *original_entrypoint;
    uint32_t       key[4];
}   t_stub_metadata;

static t_stub_metadata metadata;

void	xtea_decrypt_buff(unsigned char *buffer, size_t size, const uint32_t key[4])
{
	const uint32_t  delta = 0x9E3779B9;
	uint32_t        v0, v1;
	uint32_t        sum;

	size_t	padding = 8 - (size % 8);
	if (padding == 0)
		padding = 0x08;

	size += padding;

	for (size_t i = 0; i + 8 <= size; i += 8)
	{
		v0 = (buffer[i] << 24) | (buffer[i+1] << 16) | (buffer[i+2] << 8) | (buffer[i+3]);
		v1 = (buffer[i+4] << 24) | (buffer[i+5] << 16) | (buffer[i+6] << 8) | (buffer[i+7]);

		sum = delta * 32;

		for (int j = 0; j < 32; j++)
		{
			v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[(sum >> 11) & 3]);
			sum -= delta;
			v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
		}

		buffer[i] = (v0 >> 24) & 0xFF;
		buffer[i+1] = (v0 >> 16) & 0xFF;
		buffer[i+2] = (v0 >> 8) & 0xFF;
		buffer[i+3] = v0 & 0xFF;
		buffer[i+4] = (v1 >> 24) & 0xFF;
		buffer[i+5] = (v1 >> 16) & 0xFF;
		buffer[i+6] = (v1 >> 8) & 0xFF;
		buffer[i+7] = v1 & 0xFF;
	}

	for (size_t i = 0; i < padding; i++)
		buffer[size - 1 - i] = 0x90;

}

void    stub(void)
{
    if (write(1, "....WOODY....\n", 14) == -1)
        return;

    mprotect(metadata.text_offset, metadata.text_size, PROT_READ | PROT_WRITE | PROT_EXEC);

    xtea_decrypt_buff((unsigned char *)metadata.text_offset, metadata.text_size, metadata.key);

    ((void(*)()) metadata.original_entrypoint)();
}
