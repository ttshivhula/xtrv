#include <header.h>

char	*path_fix(char	*path)
{
	char	*ret;

	ret = (char *)malloc(sizeof(char) * strlen(path));
	if (!strncmp(path, "../", 3))
		strcpy(ret, path + 3);
	else if (!strncmp(path, "/", 1))
		strcpy(ret, path + 1);
	else if (!strncmp(path, ".../", 4))
		strcpy(ret, path + 4);
	else
		strcpy(ret, path);
	return (ret);
}

void	create_file(char *path, size_t size, int offset, int type, mode_t mode, unsigned char *ptr, char *key)
{
	int		fd;
	size_t		i;
	unsigned char	*data;
	
	if (type == 1)
		mkdir(path, mode);
	else
	{
		fd = open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
		i = 0;
		data = xor_cipher(ptr + offset, key, size); 
		while (i < size)
			i += write(fd, data + i, (i + 4096 > size) ? size - i : 4096);
		close(fd);
	}
}

void	extract(char *file, char *key)
{
	unsigned char	*ptr;
	size_t		size;
	t_exth		*hdr;
	size_t		total;
	t_extr_v	*files;
	unsigned char	*contents;
	char		*path;

	if (map_file(file, &ptr, &size))
	{
		contents = ptr;
		hdr = (t_exth *)ptr;
		if (hdr->magic == MAGIC)
		{
			total = 0;
			ptr += sizeof(t_exth);
			while (total++ < hdr->total)
			{
				files = (t_extr_v *)ptr;
				path = path_fix(files->path);
				printf("extracting: %s \n", path);
				create_file(path, files->size, files->offset, files->type, files->mode, contents, key);
				ptr += sizeof(t_extr_v);
			}
		}
		else
			printf("Not a valid XTRV file.\n");
	}
}

int	main(int c, char **v)
{
	if (c == 2)
		extract(v[1], KEY);
	else if (c == 4 && !strcmp(v[1], "-k"))
		extract(v[3], v[2]);
	else
	{
		printf("usage: %s file.xtrv\n", v[0]);
		printf("for protected files use: %s -k secretkey file.xtrv\n", v[0]);
	}
	return (0);
}
