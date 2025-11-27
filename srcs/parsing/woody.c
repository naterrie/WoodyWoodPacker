#include "woody.h"

int	woody64(t_woody	*woody)
{
	(void)woody;
	Elf64_Ehdr	*elf_header = (Elf64_Ehdr *)woody->map;
	Elf64_Phdr	*program_header = (Elf64_Phdr *)(woody->map + elf_header->e_phoff);
	Elf64_Phdr	*last_phdr = NULL;
	Elf64_Addr	last_segment_end = 0;

	Elf64_Shdr	*section_header = (Elf64_Shdr *)(woody->map + elf_header->e_shoff);
	Elf64_Shdr	*sh_strtab = &section_header[elf_header->e_shstrndx];
	const char	*sh_strtab_p = woody->map + sh_strtab->sh_offset;
	Elf64_Shdr	*text_sh = NULL;

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
			Elf64_Addr segment_end = program_header[i].p_vaddr + program_header[i].p_memsz;
			if (segment_end > last_segment_end)
			{
				last_segment_end = segment_end;
				last_phdr = &program_header[i];
			}
		}
	}
	void *entry_point = (void *)(elf_header->e_entry);

	printf(".text found: offset=0x%lx, addr=0x%lx, size=0x%lx\n",
		(unsigned long)text_sh->sh_offset, (unsigned long)text_sh->sh_addr, (unsigned long)text_sh->sh_size);
	printf("Entry point at address: %p\n", entry_point);
	printf("Last PT_LOAD segment ends at address: 0x%lx\n", (unsigned long)last_segment_end);
	printf("Last program header PT_LOAD p_vaddr: 0x%lx\n\n", (unsigned long)last_phdr->p_vaddr);
	//munmap(woody->map, woody->size);
	return (EXIT_SUCCESS);
}

int	woody32(t_woody *woody)
{
	(void)woody;
	return (EXIT_SUCCESS);
}

int	cpy_file(t_woody *woody)
{
	int	fd_cpy = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0755);
	size_t	bytes_written = 0;

	if (fd_cpy < 0)
	{
		dprintf(2, "Failed to create copy file\n");
		return (EXIT_FAILURE);
	}
	while (bytes_written < woody->size)
	{
		printf("Writing... %zu / %zu bytes\r", bytes_written, woody->size);
		ssize_t result = write(fd_cpy, (char *)woody->map + bytes_written, woody->size - bytes_written);
		 if (result < 0)
		{
			perror("write copy file");
			dprintf(2, "Failed to write to copy file\n");
			close(fd_cpy);
			return (EXIT_FAILURE);
		}
		if (result == 0)
		{
			dprintf(2, "write returned 0 unexpectedly\n");
			close(fd_cpy);
			return (EXIT_FAILURE);
		}
		bytes_written += result;
	}
	close(fd_cpy);
	return (EXIT_SUCCESS);
}

