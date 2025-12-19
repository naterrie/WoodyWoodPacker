#include "woody.h"

int ft_strlen(const char *s)
{
	int len = 0;

	while (s[len] != '\0')
		len++;
	return (len);
}

void	*ft_memset(void *s,	int c, size_t len)
{
	char	*ptr;

	ptr = (char *)s;
	while (len > 0)
	{
		ptr[len - 1] = c;
		len--;
	}
	return (s);
}
