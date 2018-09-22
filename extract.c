#include <header.h>

int			map_file(char *filename, unsigned char **content,
		size_t *size)
{
	int			fd;
	struct stat	info;

	fd = open(filename, O_RDONLY);
	fstat(fd, &info);
	if (fd == -1 || S_ISDIR(info.st_mode))
	{
		return (0);
	}
	*size = info.st_size;
	*content = mmap(0, *size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (*content != NULL)
		return (1);
	return (0);
}

void	create_file(char *path, size_t size, int offset, int type, mode_t mode, unsigned char *ptr)
{
	int fd;
	int i;
	
	if (type == 1)
		mkdir(path, mode);
	else
	{
		fd = open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
		i = 0;
		while (i < size)
			i += write(fd, ptr + offset + i, (i + 1024 > size) ? size - i : 1024);
		close(fd);
	}
}

void	extract(char *file)
{
	unsigned char	*ptr;
	size_t		size;
	t_exth		*hdr;
	int		total;
	t_extr_v	*files;
	unsigned char	*contents;

	if (map_file(file, &ptr, &size))
	{
		contents = ptr;
		hdr = (t_exth *)ptr;
		if (hdr->magic == 0x42a42a42a)
		{
			printf("proper: %d\n", hdr->total);
			total = 0;
			ptr += sizeof(t_exth);
			while (total++ < hdr->total)
			{
				files = (t_extr_v *)ptr;
				printf("extracting: %s \n", files->path);
				create_file(files->path, files->size, files->offset, files->type, files->mode, contents);
				ptr += sizeof(t_extr_v);
			}
		}
	}
}

int	main(int c, char **v)
{
	if (c == 2)
		extract(v[1]);
	return (0);
}
