#ifndef WOODY_H
#define WOODY_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>

typedef struct s_woody
{
	int		fd;
	size_t	size;
	void	*map;
	struct stat st;
} t_woody;

int		check_file_format(t_woody *woody, const char *filename);
void	xtea_encrypt(uint32_t values[2], const uint32_t key[4]);
void	xtea_decrypt(uint32_t values[2], const uint32_t key[4]);

#endif
