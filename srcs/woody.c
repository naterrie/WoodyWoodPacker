#include "woody.h"

int	woody64(t_woody woody)
{
	(void)woody;
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)woody.map;
	Elf64_Phdr *program_header = (Elf64_Phdr *)(woody.map + elf_header->e_phoff);

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
			memcpy(segments, woody.map + program_header[i].p_offset, program_header[i].p_filesz);
			if (program_header[i].p_memsz > program_header[i].p_filesz)
			{
				bzero(segments + program_header[i].p_filesz,
					program_header[i].p_memsz - program_header[i].p_filesz);
			}
		}
	}
	void *entry_point = (void *)(elf_header->e_entry);
	printf("Entry point at address: %p\n", entry_point);

	printf("Woody64 processing completed successfully.\n");
	munmap(woody.map,woody.size);
	return (EXIT_SUCCESS);
}

int	woody32(t_woody woody)
{
	(void)woody;
	return (EXIT_SUCCESS);
}
