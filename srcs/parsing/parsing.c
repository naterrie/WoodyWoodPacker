#include "woody.h"

int check_original_file_format(t_file *original_file)
{
	char *map;
	original_file->fd = open(original_file->filename, O_RDONLY);

	if (original_file->fd < 0)
	{
		dprintf(2, "Couldn't open file descriptor\n");
		return (EXIT_FAILURE);
	}

	original_file->size = lseek(original_file->fd, 0, SEEK_END);
	if (original_file->size == (size_t)-1)
	{
		dprintf(2, "Couldn't determine file size\n");
		return (EXIT_FAILURE);
	}

	original_file->map = mmap(NULL, original_file->size, PROT_READ, MAP_PRIVATE, original_file->fd, 0);
	if (original_file->map == MAP_FAILED)
	{
		dprintf(2, "Memory mapping failed\n");
		return (EXIT_FAILURE);
	}
	close(original_file->fd);

	map = (char *)original_file->map;
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

int check_new_file_format(t_file *new_file)
{
	char *map;
	new_file->fd = open(new_file->filename, O_RDWR);

	if (new_file->fd < 0)
	{
		dprintf(2, "Couldn't open file descriptor\n");
		return (EXIT_FAILURE);
	}

	new_file->size = lseek(new_file->fd, 0, SEEK_END);
	if (new_file->size == (size_t)-1)
	{
		dprintf(2, "Couldn't determine file size\n");
		return (EXIT_FAILURE);
	}

	new_file->map = mmap(NULL, new_file->size, PROT_READ | PROT_WRITE, MAP_SHARED, new_file->fd, 0);
	if (new_file->map == MAP_FAILED)
	{
		dprintf(2, "Memory mapping failed\n");
		return (EXIT_FAILURE);
	}
	close(new_file->fd);

	map = (char *)new_file->map;
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

int	check_elf_header(t_file *file)
{
	char *map = (char *)file->map;

	if (map[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr *elf_header = (Elf64_Ehdr *)file->map;

		if (elf_header->e_version != EV_CURRENT)
		{
			dprintf(2, "Invalid ELF version\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
		{
			dprintf(2, "Not an executable file\n");
			return (EXIT_FAILURE);
		}
		if (elf_header->e_machine != EM_X86_64)
		{
			dprintf(2, "Unsupported architecture\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_shoff >= (Elf64_Off)file->size)
		{
			dprintf(2, "Invalid section header offset\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_phoff >= (Elf64_Off)file->size)
		{
			dprintf(2, "Invalid program header offset\n");
			return (EXIT_FAILURE);
		}
	}
	else if (map[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr *elf_header = (Elf32_Ehdr *)file->map;

		if (elf_header->e_version != EV_CURRENT)
		{
			dprintf(2, "Invalid ELF version\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
		{
			dprintf(2, "Not an executable file\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_machine != EM_386)
		{
			dprintf(2, "Unsupported architecture\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_shoff >= (Elf32_Off)file->size)
		{
			dprintf(2, "Invalid section header offset\n");
			return (EXIT_FAILURE);
		}

		if (elf_header->e_phoff >= (Elf32_Off)file->size)
		{
			dprintf(2, "Invalid program header offset\n");
			return (EXIT_FAILURE);
		}
	}
	else
	{
		dprintf(2, "Unknown ELF class\n");
		return (EXIT_FAILURE);
	}
	return map[EI_CLASS];
}
