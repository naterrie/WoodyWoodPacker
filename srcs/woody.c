#include "woody.h"

int	woody64(t_woody woody)
{
	(void)woody;
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)woody.map;
	Elf64_Phdr *program_header;
	
	return (EXIT_SUCCESS);
}

int	woody32(t_woody woody)
{
	(void)woody;
	return (EXIT_SUCCESS);
}
