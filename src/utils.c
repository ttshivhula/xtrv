#include <header.h>

int			map_file(char *filename, unsigned char **content,
		size_t *size)
{
	int			fd;
	struct stat	info;

	fd = open(filename, O_RDONLY);
	fstat(fd, &info);
	if (fd == -1 || S_ISDIR(info.st_mode))
		return (0);
	*size = info.st_size;
	*content = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (*content != NULL)
		return (1);
	return (0);
}

unsigned char		*xor_cipher(unsigned char *ptr, char *key, size_t size)
{	
	int		key_len;
	size_t		i;
	
	key_len = strlen(key);
	i = -1;
	while (++i < size)
		ptr[i] = ptr[i] ^ key[i % key_len];
	return (ptr);
}
