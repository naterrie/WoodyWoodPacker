#include "woody.h"

int	woody64(t_woody	*woody)
{
	(void)woody;
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)woody->map;
	Elf64_Phdr *program_header = (Elf64_Phdr *)(woody->map + elf_header->e_phoff);
	Elf64_Shdr *section_header = (Elf64_Shdr *)(woody->map + elf_header->e_shoff);
	Elf64_Shdr *sh_strtab = &section_header[elf_header->e_shstrndx];
	const char  *sh_strtab_p = woody->map + sh_strtab->sh_offset;
	Elf64_Shdr *text_sh = NULL;

	for (int i = 0; i < elf_header->e_shnum; i++)
	{
		const char *name = sh_strtab_p + section_header[i].sh_name;
		if (strcmp(name, ".text") == 0)
		{
			text_sh = &section_header[i];
			break;
		}
	}

	if (!text_sh)
	{
		dprintf(2, "Failed to find .text section\n");
		return (EXIT_FAILURE);
	}

	for (int i = 0; i < elf_header->e_phnum; i++)
	{
		if (program_header[i].p_type == PT_LOAD)
		{
			void *segments = mmap(NULL,
				program_header[i].p_memsz,
				PROT_READ | PROT_WRITE | PROT_EXEC,
				MAP_PRIVATE | 0x20, //  is 0x20, but school env does not define it
				-1,
				0);
			if (segments == MAP_FAILED)
			{
				dprintf(2, "Memory allocation for segment failed\n");
				return (EXIT_FAILURE);
			}
			memcpy(segments, woody->map + program_header[i].p_offset, program_header[i].p_filesz);
		}
	}
	void *entry_point = (void *)(elf_header->e_entry);

	printf(".text found: offset=0x%lx, addr=0x%lx, size=0x%lx\n",
		(unsigned long)text_sh->sh_offset, (unsigned long)text_sh->sh_addr, (unsigned long)text_sh->sh_size);
	printf("Entry point at address: %p\n", entry_point);

	munmap(woody->map, woody->size);
	return (EXIT_SUCCESS);
}

int	woody32(t_woody *woody)
{
	(void)woody;
	return (EXIT_SUCCESS);
}
