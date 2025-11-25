#include "woody.h"

void initialize_woody(t_woody *woody)
{
	woody->fd = -1;
	woody->size = 0;
	woody->map = NULL;
	bzero(&woody->st, sizeof(struct stat));
}

int main(int argc, char **argv)
{
	t_woody file;
	int		elf_h;
	initialize_woody(&file);

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
		woody64(file);
	}
	else if (elf_h == 3)
	{
		woody32(file);
	}
	else
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
