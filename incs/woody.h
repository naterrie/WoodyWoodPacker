#ifndef WOODY_H
#define WOODY_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>

typedef struct woody
{
	int		fd;
	size_t	size;
	void	*map;
	struct stat st;
} woody_t;

int check_file_format(woody_t *woody, const char *filename);

#endif
