#include "woody.h"

int	woody64(t_woody woody)
{
	(void)woody;
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)woody.map;
	Elf64_Phdr *program_header = (Elf64_Phdr *)(woody.map + elf_header->e_phoff);
	void **segments = malloc(sizeof(void *) * elf_header->e_phnum);
	if (!segments)
		return (EXIT_FAILURE);

	for (int i = 0; i < elf_header->e_phnum; i++)
		segments[i] = NULL;


	for (int i = 0; i < elf_header->e_phnum; i++)
	{
		if (program_header[i].p_type == PT_LOAD)
		{
			segments[i] = mmap(NULL,
				program_header[i].p_memsz,
				PROT_READ | PROT_WRITE | PROT_EXEC,
				MAP_PRIVATE | MAP_ANONYMOUS,
				-1,
				0);
			if (segments[i] == MAP_FAILED)
			{
				dprintf(2, "Memory allocation for segment failed\n");
				free(segments);
				return (EXIT_FAILURE);
			}
		}
	}

	void *entry = (void *)(elf_header->e_entry);
	((void(*)())entry)();
	return (EXIT_SUCCESS);
}

int	woody32(t_woody woody)
{
	(void)woody;
	return (EXIT_SUCCESS);
}
