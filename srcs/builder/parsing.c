#include "woody.h"

int check_file_format(t_woody *woody, const char *filename)
{
	char *map;
	woody->fd = open(filename, O_RDONLY);
	
	if (woody->fd < 0)
	{
		dprintf(2, "Couldn't open file descriptor\n");
		return (EXIT_FAILURE);
	}

	if (fstat(woody->fd, &woody->st) < 0)
	{
		dprintf(2, "Couldn't get file stats\n");
		return (EXIT_FAILURE);
	}

	if (woody->st.st_size == 0)
	{
		dprintf(2, "File is empty\n");
		return (EXIT_FAILURE);
	}

	woody->map = mmap(NULL, woody->st.st_size, PROT_READ, MAP_PRIVATE, woody->fd, 0);
	if (woody->map == MAP_FAILED)
	{
		dprintf(2, "Memory mapping failed\n");
		return (EXIT_FAILURE);
	}
	
	map = (char *)woody->map;
	if (map[EI_MAG0] != 0x7F || map[EI_MAG1] != 0x45 || \
		map[EI_MAG2] != 0x4c || map[EI_MAG3] != 0x46)
	{
		dprintf(2, "Not an ELF file\n");
		return (EXIT_FAILURE);
	}

	if (map[EI_CLASS] != ELFCLASS64 && map[EI_CLASS] != ELFCLASS32)
	{
		dprintf(2, "Unsupported ELF class\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
