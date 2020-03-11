#include <header.h>

void modestring(mode_t mode, char * buf)
{
  const char chars[] = "rwxrwxrwx";
  for (size_t i = 0; i < 9; i++) {
    buf[i] = (mode & (1 << (8 - i))) ? chars[i] : '-';
  }
  buf[9] = '\0';
}

void list_files(char *file)
{
  unsigned char *ptr;
  size_t size;
	t_exth		*hdr;
	size_t		total;
	t_extr_v	*files;
  char buf[10];

  if(map_file(file, &ptr, &size))
  {
    hdr = (t_exth *)ptr;
    if(hdr->magic == MAGIC)
    {
      total = 0;
      ptr += sizeof(t_exth);
      while (total++ < hdr->total)
      {
        files = (t_extr_v *)ptr;
        modestring(files->mode, buf);
        (files->type == 1) ? 0 : printf("%c%s\t%zu\t%s\n", '-', buf,
            files->size, path_fix(files->path) + 2);
        ptr += sizeof(t_extr_v);
      }
    }
  }

}
