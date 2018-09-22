EXTR = extrv
UNEXTR = unextrv
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinc
SRC = src/extract.c src/files.c src/utils.c \

OBJ = $(SRC:.c=.o)
all: $(EXTR) $(UNEXTR)

$(UNEXTR): $(OBJ)
	@gcc -c $(CFLAGS) utils.o extract.c -o $(UNEXTR)

$(EXTR): $(OBJ)
	@gcc -c $(CFLAGS) utils.o files.c -o $(EXTR)

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(EXTR) $(UNEXTR)

re: fclean all
