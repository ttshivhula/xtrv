#include <header.h>

/* Removes the dots from the path to make sure that when extracting the directories are created
 * on the current folder.
 */

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

/* It checks if the current pointed data is a file or directory, and creates the directory/file using the
 * mode specified in the mode variable to make sure that files keep their original modes, if it is a file
 * then we use the key to decrypt data stored at PTR + OFFSET, and the write the original data to the 
 * created file.
 */

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
		contents = ptr; /* contents stores the mapped ptr, because files->offset, makes an assumption that we didn't move the ptr. */
		hdr = (t_exth *)ptr; /* Takes maped data from file and cast to header struct. */
		if (hdr->magic == MAGIC) /*check the magic number to see if the current file is of xtrv type. */
		{
			total = 0;
			ptr += sizeof(t_exth); /* skips the xtrv header to go to the start of the xtrv structures. */
			while (total++ < hdr->total)
			{
				files = (t_extr_v *)ptr; /*use the same ptr to access the T_EXTR_V structures that has info about archived files. */
				path = path_fix(files->path);
				printf("extracting: %s \n", path);
				create_file(path, files->size, files->offset, files->type, files->mode, contents, key);
				ptr += sizeof(t_extr_v); /* moves ptr by the size of t_extr_v, because we can't simply increment the ptr. */
			}
		}
		else
			printf("Not a valid XTRV file.\n");
	}
}

int	main(int c, char **v)
{
	if (c == 2)
		extract(v[1], NULL);
	else if (c == 4 && !strcmp(v[1], "-k"))
		extract(v[3], v[2]);
  else if (c == 3 && !strcmp(v[1], "-l"))
  {
    list_files(v[2]);
  }
	else
	{
		printf("usage: %s file.xtrv\n", v[0]);
		printf("for protected files use: %s -k secretkey file.xtrv\n", v[0]);
	}
	return (0);
}
