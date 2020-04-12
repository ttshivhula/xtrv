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

unsigned char		*encrypt_decrypt(unsigned char *ptr, char *key, size_t size, char *iv_key, int new)
{
  struct AES_ctx ctx;

  if (!key || !strlen(key))
    return (ptr);
  printf("key: %s\n", (uint8_t *)iv_key);
  if (new)
  {
    AES_init_ctx_iv(&ctx, (const uint8_t *)key, (const uint8_t *)iv_key);
    //AES_CBC_encrypt_buffer(&ctx, ptr, size);
    AES_CTR_xcrypt_buffer(&ctx, ptr, size);
    bzero(&ctx, sizeof(ctx));
  }
  else
  {
    AES_init_ctx_iv(&ctx, (const uint8_t *)key, (const uint8_t *)iv_key);
    //AES_CBC_decrypt_buffer(&ctx, ptr, size);
    AES_CTR_xcrypt_buffer(&ctx, ptr, size);
    bzero(&ctx, sizeof(ctx));
  }
	return (ptr);
}

char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}
