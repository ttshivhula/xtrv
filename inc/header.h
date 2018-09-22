#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/mman.h>

# define MAGIC 0x42a42a42a

typedef	struct		s_exth
{
	long		magic;
	size_t		total;
}			t_exth;

typedef	struct		s_extr_v
{
	char		path[4096];
	int		type;
	size_t		size;
	mode_t		mode;
	int		offset;
}			t_extr_v;

typedef struct		s_files
{
	char		path[4096];
	int		type;
	struct s_files	*next;
}			t_files;

typedef struct		s_main
{
	size_t		file_count;
	size_t		dir_count;
	size_t		s_size;
	int		offset;
}			t_main;

int			check_dir(t_files **files, char *path, int path_len);
int			map_file(char *filename, unsigned char **content,
		size_t *size);

#endif
