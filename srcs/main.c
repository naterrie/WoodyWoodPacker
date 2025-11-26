#include "woody.h"

void initialize_woody(t_woody *woody)
{
	woody->fd = -1;
	woody->size = 0;
	woody->map = NULL;
	bzero(&woody->st, sizeof(struct stat));
}

static void test_crypto()
{
	uint32_t values[2] = {0x12345678, 0x9ABCDEF0};
	uint32_t key[4] = {0x0F1E2D3C, 0x4B5A6978, 0x8796A5B4, 0xC3D2E1F0};

	xtea_encrypt(values, key);

	dprintf(1, "\n");

	void *buffer = malloc(16);
	bzero(buffer, 16);
	((uint32_t *)buffer)[0] = 0x11223344;
	((uint32_t *)buffer)[1] = 0x55667788;
	xtea_encrypt_buff(buffer, 16, key);
	free(buffer);

	dprintf(1, "\n");

	void *buffer2 = malloc(70);
	for (size_t i = 0; i < 70; i++)
		((unsigned char *)buffer2)[i] = (unsigned char)i;
	xtea_encrypt_buff(buffer2, 70, key);
	free(buffer2);

	dprintf(1, "\n");

	char test_buffer[14] = "Hello, World!";
	xtea_encrypt_buff(test_buffer, 13, key);

	dprintf(1, "\n");
	uint32_t gen_key[4];
	generate_key(gen_key);
	dprintf(1, "Key generated: %x %x %x %x\n", gen_key[0], gen_key[1], gen_key[2], gen_key[3]);
}

int main(int argc, char **argv)
{
	t_woody file;
	initialize_woody(&file);

	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (check_file_format(&file, argv[1]) != EXIT_SUCCESS)
		return (EXIT_FAILURE);

	test_crypto();

	return (EXIT_SUCCESS);
}
