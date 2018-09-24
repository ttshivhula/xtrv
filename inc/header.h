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

/* Limited PATH_MAX to 1024, even though POSIX PATH MAX is at 4096 but that
 * would have made the archived file very large, 1024 is good enough for
 * anyone ;)
 */

# define XTR_PATH 1024 /* Posible length of the PATH. */
# define MAGIC 0x42a42a42a /* Magic number used for identfying xtrv files. */

/*
 * Header writen on top of the file that specifies the magic number]
 * and total files/folders archived in the file.
 */

typedef	struct		s_exth
{
	long		magic;
	size_t		total;
}			t_exth;

/*
 * Header writen on the file that specifies the path, file type, mode
 * and offset. offset is used to show where the actual data of the file
 * starts on the archive.
 */

typedef	struct		s_extr_v
{
	char		path[XTR_PATH]; /*actual file path. */
	int		type; /* 1 for directories and 2 for files. */
	size_t		size; /* actual size of the file stored, always 0 for directories. */
	mode_t		mode; /*file or directory mode. */
	int		offset; /*offset showing where actual data is stored. */
}			t_extr_v;

/* Just a linked list to temporarily store files as they are received by the archiving program
 * this struct is not writen to the actual archive.
 * */

typedef struct		s_files
{
	char		path[XTR_PATH];
	int		type;
	struct s_files	*next;
}			t_files;

/* Holds information used by the archiving program, this structure is not written to the file.
 */

typedef struct		s_main
{
	char		key[XTR_PATH]; /*stores key used by the user for encription. */
	size_t		file_count; /* holds total number of files to be archived. */
	size_t		dir_count; /* holds total number of directories to be archived. */
	size_t		s_size; /* sizeof the t_extr_v struct. sizeof(t_extr_v). */
	int		offset; /* offset where all the file data is stored on the archive. */
}			t_main;

int			check_dir(t_files **files, char *path, int path_len);
int			map_file(char *filename, unsigned char **content,
		size_t *size);
unsigned char		*xor_cipher(unsigned char *ptr, char *key, size_t size);

#endif
