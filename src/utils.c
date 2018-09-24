#include <header.h>

/*
 * 	calls mmap and maps the file in memory and returns the pointer to the mapped file
 * 	to content and also the size of the file. returns 0 on failure and 1 if successful.
 */

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

/* Runs basic XOR CIPHER encryption on the pointer(ptr) using the key(key) and returns
 * the encrypted version of the pointer.
 */

unsigned char		*xor_cipher(unsigned char *ptr, char *key, size_t size)
{	
	int		key_len;
	size_t		i;
	
	key_len = key ? strlen(key) : 0;
	if (!key_len)
		return (ptr);
	i = -1;
	while (++i < size)
		ptr[i] = ptr[i] ^ key[i % key_len];
	return (ptr);
}
