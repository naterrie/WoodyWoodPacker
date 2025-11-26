#include "woody.h"

static void initialize_woody(t_woody *woody, t_encrypt_data *enc_data)
{
	woody->fd = -1;
	woody->size = 0;
	woody->map = NULL;
	bzero(&woody->st, sizeof(struct stat));
	enc_data->offset = 0;
	enc_data->size = 0;
	enc_data->entrypoint = 0;
	bzero(enc_data->key, sizeof(uint32_t) * 4);
}

static	void test_stub(t_encrypt_data *enc_data)
{
	char	secret[] = "This is a secret message!\nPlease keep it safe.\n";
	char	secret_size = ft_strlen(secret) + 1;

	generate_key(enc_data->key);
	xtea_encrypt_buff(secret, secret_size, enc_data->key);
}

int main(int argc, char **argv)
{
	t_woody			file;
	t_encrypt_data	enc_data;

	initialize_woody(&file, &enc_data);

	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (check_file_format(&file, argv[1]) != EXIT_SUCCESS)
		return (EXIT_FAILURE);

	test_stub(&enc_data);

	return (EXIT_SUCCESS);
}
