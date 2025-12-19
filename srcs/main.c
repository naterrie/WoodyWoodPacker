#include "woody.h"

static int	cleanup(t_file *original_file, t_file *new_file, int exit_code)
{
	if (original_file->map)
		munmap(original_file->map, original_file->size);
	if (new_file->map)
		munmap(new_file->map, new_file->size);
	return (exit_code);
}

static void initialize(t_file *original_file, t_file *new_file, t_file_meta *metadata)
{
	original_file->fd = -1;
	original_file->size = 0;
	original_file->map = NULL;
	original_file->filename = NULL;

	new_file->fd = -1;
	new_file->size = 0;
	new_file->map = NULL;
	new_file->filename = NULL;

	metadata->text_offset = 0;
	metadata->text_vaddr = 0;
	metadata->text_size = 0;
	metadata->original_entrypoint = 0;
	metadata->stub_offset = 0;
	metadata->stub_vaddr = 0;
	metadata->stub_size = 0;
	metadata->text_padding = 0;
	metadata->required_filesize = 0;
	ft_memset(metadata->key, 0, sizeof(metadata->key));
}

int main(int ac, char **av)
{
	int				elf_h;
	t_file			original_file;
	t_file			new_file;
	t_file_meta		metadata;

	initialize(&original_file, &new_file, &metadata);

	if (ac != 2)
	{
		dprintf(2, "Usage: %s <file>\n", av[0]);
		return (EXIT_FAILURE);
	}

	original_file.filename = av[1];
	new_file.filename = FILENAME;

	if (check_file_format(&original_file, O_RDONLY, PROT_READ, MAP_PRIVATE) != EXIT_SUCCESS)
		return (cleanup(&original_file, &new_file, EXIT_FAILURE));

	generate_key(metadata.key);
	dprintf(1, "KEY: %X %X %X %X\n", metadata.key[0], metadata.key[1], metadata.key[2], metadata.key[3]);

	elf_h = check_elf_header(&original_file);
	if (elf_h == ELFCLASS64)
	{
	    if (analyze_file64(&original_file, &metadata) != EXIT_SUCCESS)
	        return (cleanup(&original_file, &new_file, EXIT_FAILURE));
	}
	else if (elf_h == ELFCLASS32)
	{
	    if (woody32(&original_file, &metadata) != EXIT_SUCCESS)
	        return (cleanup(&original_file, &new_file, EXIT_FAILURE));
	}
	else
		return (cleanup(&original_file, &new_file, EXIT_FAILURE));

	metadata.text_padding = 8 - (metadata.text_size % 8);

	metadata.stub_size =
		srcs_stub_stub_bin_len +
		sizeof(uint64_t) * 3 +
		sizeof(uint32_t) * 4;

	metadata.required_filesize =
		original_file.size +
		metadata.stub_size +
		metadata.text_padding;

	if (cpy_file(&original_file, &new_file, metadata.required_filesize) != EXIT_SUCCESS)
		return (cleanup(&original_file, &new_file, EXIT_FAILURE));

	if (check_file_format(&new_file, O_RDWR, PROT_READ | PROT_WRITE, MAP_SHARED) != EXIT_SUCCESS)
		return (cleanup(&original_file, &new_file, EXIT_FAILURE));

	return(cleanup(&original_file, &new_file, EXIT_SUCCESS));
}
