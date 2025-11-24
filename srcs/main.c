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
