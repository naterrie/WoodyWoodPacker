#include "woody.h"

int	check_file_format(t_file *file, int open_flags, int mmap_prot, int mmap_flags)
{
	char *map;
	file->fd = open(file->filename, open_flags);

	if (file->fd < 0)
	{
		dprintf(2, "Couldn't open file descriptor\n");
		return (EXIT_FAILURE);
	}

	file->size = lseek(file->fd, 0, SEEK_END);
	if (file->size == (size_t)-1)
	{
		dprintf(2, "Couldn't determine file size\n");
		close(file->fd);
		return (EXIT_FAILURE);
	}

	file->map = mmap(NULL, file->size, mmap_prot, mmap_flags, file->fd, 0);
	if (file->map == MAP_FAILED)
	{
		dprintf(2, "Memory mapping failed\n");
		close(file->fd);
		return (EXIT_FAILURE);
	}

	close(file->fd);

	map = (char *)file->map;
	if (map[EI_MAG0] != ELFMAG0 || \
		map[EI_MAG1] != ELFMAG1 || \
		map[EI_MAG2] != ELFMAG2 || \
		map[EI_MAG3] != ELFMAG3)
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

int	check_elf_header(t_file *file)
{
	char *map = (char *)file->map;

	if (map[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr *elf_header = (Elf64_Ehdr *)file->map;

		if (elf_header->e_version != EV_CURRENT)
		{
			dprintf(2, "Invalid ELF version\n");
			return (-1);
		}

		if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
		{
			dprintf(2, "Not an executable file\n");
			return (-1);
		}
		if (elf_header->e_machine != EM_X86_64)
		{
			dprintf(2, "Unsupported architecture\n");
			return (-1);
		}

		if (elf_header->e_shoff >= (Elf64_Off)file->size)
		{
			dprintf(2, "Invalid section header offset\n");
			return (-1);
		}

		if (elf_header->e_phoff >= (Elf64_Off)file->size)
		{
			dprintf(2, "Invalid program header offset\n");
			return (-1);
		}
	}
	else if (map[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr *elf_header = (Elf32_Ehdr *)file->map;

		if (elf_header->e_version != EV_CURRENT)
		{
			dprintf(2, "Invalid ELF version\n");
			return (-1);
		}

		if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
		{
			dprintf(2, "Not an executable file\n");
			return (-1);
		}

		if (elf_header->e_machine != EM_386)
		{
			dprintf(2, "Unsupported architecture\n");
			return (-1);
		}

		if (elf_header->e_shoff >= (Elf32_Off)file->size)
		{
			dprintf(2, "Invalid section header offset\n");
			return (-1);
		}

		if (elf_header->e_phoff >= (Elf32_Off)file->size)
		{
			dprintf(2, "Invalid program header offset\n");
			return (-1);
		}
	}
	else
	{
		dprintf(2, "Unknown ELF class\n");
		return (-1);
	}
	return map[EI_CLASS];
}
