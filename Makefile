EXTR = extrv
UNEXTR = unextrv
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinc
SRC = src/extract.c src/files.c src/utils.c \

OBJ = $(SRC:.c=.o)
all: $(EXTR) $(UNEXTR)

$(UNEXTR): $(OBJ)
	@gcc $(CFLAGS) src/utils.o src/extract.o -o $(UNEXTR)

$(EXTR): $(OBJ)
	@gcc $(CFLAGS) src/utils.o src/files.o -o $(EXTR)

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(EXTR) $(UNEXTR)

re: fclean all
