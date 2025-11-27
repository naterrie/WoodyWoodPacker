#include "woody.h"

int	woody64(t_woody	*woody, t_woody_meta *metadata)
{
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

	metadata->text_offset = text_sh->sh_offset;
	metadata->text_size = text_sh->sh_size;
	metadata->original_entrypoint = elf_header->e_entry;

	unsigned long	stub_offset = last_phdr->p_offset + last_phdr->p_filesz;
	unsigned long	stub_vaddr = last_phdr->p_vaddr + last_phdr->p_memsz;

	(void)stub_offset;
	(void)stub_vaddr;
	// elf_header->e_entry = stub_vaddr
	// need to update last_phdr size to include stub

	// last_phdr->p_filesz +=
	// last_phdr->p_memsz +=

	// memcpy(woody->map + stub_offset, stubptr, stubsize);

	return (EXIT_SUCCESS);
}

int	woody32(t_woody *woody, t_woody_meta *metadata)
{
	Elf32_Ehdr	*elf_header = (Elf32_Ehdr *)woody->map;
	Elf32_Phdr	*program_header = (Elf32_Phdr *)(woody->map + elf_header->e_phoff);
	Elf32_Phdr	*last_phdr = NULL;
	Elf32_Addr	last_segment_end = 0;

	Elf32_Shdr	*section_header = (Elf32_Shdr *)(woody->map + elf_header->e_shoff);
	Elf32_Shdr	*sh_strtab = &section_header[elf_header->e_shstrndx];
	const char	*sh_strtab_p = woody->map + sh_strtab->sh_offset;
	Elf32_Shdr	*text_sh = NULL;

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
			Elf32_Addr segment_end = program_header[i].p_vaddr + program_header[i].p_memsz;
			if (segment_end > last_segment_end)
			{
				last_segment_end = segment_end;
				last_phdr = &program_header[i];
			}
		}
	}

	metadata->text_offset = text_sh->sh_offset;
	metadata->text_size = text_sh->sh_size;
	metadata->original_entrypoint = elf_header->e_entry;
	(void)last_phdr;
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

