#ifndef WOODY_H
# define WOODY_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <string.h>
# include <sys/mman.h>
# include <elf.h>
# include <sys/stat.h>

# define FILENAME "woody"

extern unsigned char srcs_stub_stub_bin[];
extern unsigned long srcs_stub_stub_bin_len;

typedef struct s_woody
{
	int		fd;
	size_t	size;
	void	*map;
}	t_woody;

typedef struct	s_woody_meta {
	uint64_t	text_offset;
	uint64_t	text_size;
	uint64_t	original_entrypoint;
	uint32_t	key[4];
}	t_woody_meta;

int			check_file_format(t_woody *woody, const char *filename);
int			check_elf_header(t_woody *woody);
void		generate_key(uint32_t key[4]);
void		xtea_encrypt_buff(void *buffer, size_t size, const uint32_t key[4]);
int			ft_strlen(const char *s);
int			woody64(t_woody	*woody, t_woody_meta *metadata);
int			woody32(t_woody	*woody, t_woody_meta *metadata);
int			cpy_file(t_woody *woody);

#endif
