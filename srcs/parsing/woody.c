#include "woody.h"

int	analyze_file64(t_file	*file, t_file_meta *metadata)
{
	Elf64_Ehdr	*elf_header = (Elf64_Ehdr *)file->map;
	Elf64_Phdr	*program_header = (Elf64_Phdr *)(file->map + elf_header->e_phoff);
	Elf64_Phdr	*last_phdr = NULL;
	Elf64_Addr	last_segment_end = 0;

	Elf64_Shdr	*section_header = (Elf64_Shdr *)(file->map + elf_header->e_shoff);
	Elf64_Shdr	*sh_strtab = &section_header[elf_header->e_shstrndx];
	const char	*sh_strtab_p = file->map + sh_strtab->sh_offset;
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
		if (program_header[i].p_type == PT_LOAD  && (program_header[i].p_flags & PF_X))
		{
			Elf64_Addr segment_end = program_header[i].p_vaddr + program_header[i].p_memsz;
			if (segment_end > last_segment_end)
			{
				last_segment_end = segment_end;
				last_phdr = &program_header[i];
			}
		}
	}

	if (!last_phdr)
    {
        dprintf(2, "Failed to find last executable PT_LOAD\n");
        return (EXIT_FAILURE);
    }

	metadata->text_offset = text_sh->sh_offset;
	metadata->text_vaddr = text_sh->sh_addr;
	metadata->text_size = text_sh->sh_size;
	metadata->original_entrypoint = elf_header->e_entry;
	metadata->stub_offset = last_phdr->p_offset + last_phdr->p_filesz;
	metadata->stub_vaddr  = last_phdr->p_vaddr + last_phdr->p_memsz;

	return (EXIT_SUCCESS);
}

int	woody32(t_file *file, t_file_meta *metadata)
{
	(void)file;
	(void)metadata;
	return (1);
}

int	cpy_file(t_file *original_file, t_file *new_file, size_t final_size)
{
	int		fd;
	ssize_t	written = 0;

	fd = open(new_file->filename, O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd < 0)
	{
		dprintf(2, "Failed to create copy file\n");
		return (EXIT_FAILURE);
	}

	if (ftruncate(fd, final_size) < 0)
	{
		dprintf(2, "Failed to truncate copy file\n");
		close(fd);
		return (EXIT_FAILURE);
	}

	while (written < (ssize_t)original_file->size)
	{
		ssize_t ret = write(fd,
			(char *)original_file->map + written,
			original_file->size - written);

		if (ret <= 0)
		{
			dprintf(2, "Failed to write copy file\n");
			close(fd);
			return (EXIT_FAILURE);
		}
		written += ret;
	}

	close(fd);
	return (EXIT_SUCCESS);
}

