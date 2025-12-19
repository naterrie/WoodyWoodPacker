#ifndef WOODY_H
# define WOODY_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <elf.h>

# define FILENAME "woody"

extern unsigned char srcs_stub_stub_bin[];
extern unsigned long srcs_stub_stub_bin_len;

typedef struct s_file
{
	int		fd;
	size_t	size;
	void	*map;
	char	*filename;
}	t_file;

typedef struct	s_file_meta {
	uint64_t	text_offset;
	uint64_t	text_vaddr;
	uint64_t	text_size;
	uint64_t	original_entrypoint;

	uint64_t	stub_offset;
	uint64_t	stub_vaddr;
	uint64_t	stub_size;

	uint64_t	text_padding;
	uint64_t	required_filesize;

	uint32_t	key[4];
}	t_file_meta;

/* file */
int			check_file_format(t_file *file, int open_flags, int mmap_prot, int mmap_flags);
int			cpy_file(t_file *original_file, t_file *new_file, size_t final_size);

/* elf */
int			check_elf_header(t_file *file);
int			analyze_file64(t_file	*file, t_file_meta *metadata);
int			woody32(t_file	*file, t_file_meta *metadata);

/* crypto */
void		generate_key(uint32_t key[4]);
void		xtea_encrypt_buff(void *buffer, size_t size, const uint32_t key[4]);

/* libft */
int			ft_strlen(const char *s);
void		*ft_memset(void *s,	int c, size_t len);

#endif
