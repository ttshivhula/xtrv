#include <header.h>

void	extract(char *file)
{
	unsigned char	*ptr;
	size_t		size;
	t_exth		*hdr;
	size_t		total;
	t_extr_v	*files;
	unsigned char	*contents;

	if (map_file(file, &ptr, &size))
	{
		contents = ptr;
		hdr = (t_exth *)ptr;
		if (hdr->magic == 0x42a42a42a)
		{
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
