XTR = xtrv
UNXTR = unxtrv
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinc
SRC = src/extract.c src/files.c src/utils.c src/list.c \

OBJ = $(SRC:.c=.o)
all: $(XTR) $(UNXTR)

$(UNXTR): $(OBJ)
	gcc $(CFLAGS) src/utils.o src/extract.o src/list.o -o $(UNXTR)

$(XTR): $(OBJ)
	gcc $(CFLAGS) src/utils.o src/files.o -o $(XTR)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(XTR) $(UNXTR)

install: 
	sudo cp $(XTR) /usr/bin
	sudo cp $(UNXTR) /usr/bin
	sudo cp man/man1/xtrv.1 /usr/share/man/man1
	sudo cp man/man1/unxtrv.1 /usr/share/man/man1

uninstall: 
	sudo rm /usr/bin/$(XTR)
	sudo rm /usr/bin/$(UNXTR)
	sudo rm /usr/share/man/man1/xtrv.1
	sudo rm /usr/share/man/man1/unxtrv.1

re: fclean all
