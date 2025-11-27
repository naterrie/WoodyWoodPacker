#include "woody.h"

static void initialize(t_woody *woody, t_woody_meta *metadata)
{
	woody->fd = -1;
	woody->size = 0;
	woody->map = NULL;
	woody->size = 0;

	metadata->text_offset = 0;
	metadata->text_size = 0;
	metadata->original_entrypoint = 0;
	bzero(metadata->key, sizeof(uint32_t) * 4);
}

int main(int argc, char **argv)
{
	int				elf_h;
	t_woody			file;
	t_woody_meta	metadata;

	initialize(&file, &metadata);

	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (check_file_format(&file, argv[1]) != EXIT_SUCCESS)
		return (EXIT_FAILURE);

	elf_h = check_elf_header(&file);

	if (elf_h == 2)
	{
		woody64(&file, &metadata);
	}
	else if (elf_h == 3)
	{
		woody32(&file);
	}
	else
		return (EXIT_FAILURE);


	generate_key(metadata.key);
	dprintf(1, "KEY: %X %X %X %X\n", metadata.key[0], metadata.key[1], metadata.key[2], metadata.key[3]);

	unsigned char	*text_content = file.map + metadata.text_offset;

	size_t	padding = 8 - (metadata.text_size % 8);
	if (padding == 0)
		padding = 0x08;

	for (size_t i = 0; i < metadata.text_size; i++)
		printf("%X ", text_content[i]);
	printf("\n");

	memset(text_content + metadata.text_size, padding, padding);

	xtea_encrypt_buff(text_content, metadata.text_size + padding, metadata.key);

	stub(&metadata, text_content);
	cpy_file(&file);

	munmap(file.map, file.size);

	return (EXIT_SUCCESS);
}
