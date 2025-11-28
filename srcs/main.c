#include "woody.h"

static void initialize(t_woody *original_file, t_woody *new_file, t_woody_meta *metadata)
{
	original_file->fd = -1;
	original_file->size = 0;
	original_file->map = NULL;
	original_file->size = 0;

	new_file->fd = -1;
	new_file->size = 0;
	new_file->map = NULL;
	new_file->size = 0;

	metadata->text_offset = 0;
	metadata->text_size = 0;
	metadata->original_entrypoint = 0;
	bzero(metadata->key, sizeof(uint32_t) * 4);
}

int main(int argc, char **argv)
{
	int				elf_h;
	t_woody			original_file;
	t_woody			new_file;
	t_woody_meta	metadata;

	initialize(&original_file, &new_file, &metadata);

	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (check_original_file_format(&original_file, argv[1]) != EXIT_SUCCESS)
		return (EXIT_FAILURE);

	elf_h = check_elf_header(&original_file);

	if (cpy_file(&original_file) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	
	munmap(original_file.map, original_file.size);
	

	if (check_new_file_format(&new_file, FILENAME) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	
	if (elf_h == 2)
	{
		woody64(&new_file, &metadata);
	}
	else if (elf_h == 3)
	{
		woody32(&new_file, &metadata);
	}
	else
		return (EXIT_FAILURE);

	generate_key(metadata.key);
	dprintf(1, "KEY: %X %X %X %X\n", metadata.key[0], metadata.key[1], metadata.key[2], metadata.key[3]);

	unsigned char	*text_content = new_file.map + metadata.text_offset;
	size_t	padding = 8 - (metadata.text_size % 8);
	if (padding == 0)
		padding = 0x08;
	memset(text_content + metadata.text_size, padding, padding);
	xtea_encrypt_buff(text_content, metadata.text_size + padding, metadata.key);

	munmap(new_file.map, new_file.size);

	return (EXIT_SUCCESS);
}
