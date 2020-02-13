NAME        = mandel

SRC         = argoat.c util.c mandel.c
OBJ         = $(SRC:.c=.o)
HDR         = $(SRC:.c=.h)

INC         = -I. -Isub/ccommon/

CC          = gcc
CFLAGS      = -std=c99 -pedantic -O4 $(INC) -ggdb

all: $(NAME)

$(SRC): $(HDR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(NAME)

.PHONY: all clean
