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

typedef struct s_woody
{
	int		fd;
	size_t	size;
	void	*map;
	struct stat st;
}	t_woody;

typedef struct s_woody_meta {
    uint64_t text_offset;
    uint64_t text_size;
    uint64_t entrypoint;
    uint32_t key[4];
    uint32_t padding;
} t_woody_meta;

int			check_file_format(t_woody *woody, const char *filename);
int			ft_strlen(const char *s);
void		xtea_encrypt_buff(void *buffer, char *path, size_t size, const uint32_t key[4], int padding);
void		generate_key(uint32_t key[4]);
void		stub(t_encrypt_data *enc_data);

#endif
