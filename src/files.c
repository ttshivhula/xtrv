#include <header.h>

/* Temporarily adds files to the files linked list but at this point we only need the PATH and
 * type 1 for directories and 2 for files. We always add new file at the the begining since when
 * we recurse we have a specific order. so we need to keep the order because sorting the list
 * is just an overkill for such a simple implementation. 
 */

int	add_file(t_files **ptr, char *path, int type)
{
	t_files		*tmp;

	if (!*ptr)
	{
		*ptr = (t_files *)malloc(sizeof(t_files));
		strcpy((*ptr)->path, path);
		(*ptr)->type = type;
		(*ptr)->next = NULL;
		return (0);
	}
	tmp = (t_files *)malloc(sizeof(t_files));
	strcpy(tmp->path, path);
	tmp->type = type;
	tmp->next = *ptr;
	*ptr = tmp;
	return (0);
}

static int	dir_helper(t_files **files, struct dirent *p, char *path, int path_len)
{
	int			ret;
	char		*buf;
	size_t		len;
	struct stat statbuf;

	ret = -1;
	if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
		return (0);
	len = path_len + strlen(p->d_name) + 2;
	buf = malloc(len);
	if (buf)
	{
		snprintf(buf, len, "%s/%s", path, p->d_name);
		if (!stat(buf, &statbuf))
		{
			if (S_ISDIR(statbuf.st_mode))
				ret = check_dir(files, buf, strlen(buf));
			else
				ret = add_file(files, buf, 2);
		}
		free(buf);
	}
	return (ret);
}

/* Checks provided path if it is directory or just a file, if it is a file it will  add the file to
 * the list but if it is a directory it opens the dir and then add all files and directories inside
 * that directory recursively.
 */

int			check_dir(t_files **files, char *path, int path_len)
{
	DIR				*d;
	int				ret;
	struct dirent	*p;
	struct stat statbuf;

	ret = -1;
	d = opendir(path);
	if (d)
	{
		ret = 0;
		while (!ret && (p = readdir(d)))
			ret = dir_helper(files, p, path, path_len);
		closedir(d);
		if (!ret)
			ret = add_file(files, path, 1);
	}
	else
	{
		if (!stat(path, &statbuf))
			ret = add_file(files, path, 2);
	}
	return (ret);
}

/* Counts the directories and files in the files list, type is specified to check if we
 * want files or directory count.
 */

size_t	file_c(t_files *s, int type)
{
	size_t sz = 0;

	while (s)
	{
		if (s->type == type)
			sz++;
		s = s->next;
	}
	return (sz);
}


void	folder_structs(t_files *s, int fd)
{
	t_extr_v tmp;
	struct stat	info;

	while (s)
	{
		if (s->type == 1)
		{
			bzero(&tmp, sizeof(t_extr_v));
			stat(s->path, &info);
			tmp.mode = info.st_mode;
			tmp.type = 1;
			strcpy(tmp.path, s->path);
			write(fd, &tmp, sizeof(t_extr_v));
		}
		s = s->next;
	}
}


void	file_structs(t_files *s, int fd, t_main *m)
{
	t_extr_v tmp;
	struct stat	info;

	while (s)
	{
		if (s->type == 2)
		{
			bzero(&tmp, sizeof(t_extr_v));
			tmp.type = 2;
			stat(s->path, &info);
			tmp.size = info.st_size;
			tmp.mode = info.st_mode;
			tmp.offset = m->offset;
			m->offset += tmp.size;
			strcpy(tmp.path, s->path);
			write(fd, &tmp, sizeof(t_extr_v));
		}
		s = s->next;
	}
}

void	file_contents(t_files *s, int fd, char *key)
{
	size_t		size;
	unsigned char	*content;
	unsigned char	*encripted;
	size_t		i;
	
	while (s)
	{
		if (s->type == 2)
		{
			map_file(s->path, &content, &size);
			encripted = xor_cipher(content, key, size); 
			i = 0;
			while (i < size)
				i += write(fd, encripted + i, (i + 4096 > size) ? size - i : 4096);
		}
		s = s->next;
	}
}

void	combine(t_main *m, t_files *files, char *name)
{
	int	fd;
	t_exth	header;

	bzero(&header, sizeof(t_exth));
	fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
	header.magic = MAGIC;
	header.total = m->file_count + m->dir_count;
	write(fd, &header, sizeof(t_exth));
	folder_structs(files, fd);
	file_structs(files, fd, m);
	file_contents(files, fd, m->key);
}

int	main(int c, char **v)
{
	t_files	*files = NULL;
	t_main	m;
	int	i = 0;
	char	output[4096];
	int	flag;

	flag = 0;
	bzero(&m, sizeof(m));
	strcpy(output, "out.xtrv");
	if (c > 1)
	{	
		while (++i < c)
		{
			if (!strcmp(v[i], "-o"))
				flag = 1;
			else if (!strcmp(v[i], "-k"))
				flag = 2;
			else
			{
				if (flag == 1)
				{
					strcpy(output, v[i]);
					flag = 0;
				}
				else if (flag == 2)
				{
					strcpy(m.key, v[i]);
					flag = 0;
				}
				else
					check_dir(&files, v[i], strlen(v[i]));
			}
		}
		m.s_size = sizeof(t_extr_v);
		m.file_count = file_c(files, 2);
		m.dir_count = file_c(files, 1);
		m.offset = ((m.file_count + m.dir_count) * sizeof(t_extr_v)) + sizeof(t_exth);
		combine(&m, files, output);
	}
	else
	{
		printf("usage: %s file1 dir1 ... -o file.xtrv\n", v[0]);
		printf("to protect archive with key: %s file1 dir1 ... -o file.xtrv -k secretkey\n", v[0]);
	}
}
