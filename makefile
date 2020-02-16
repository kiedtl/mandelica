NAME        = mandel

WARN        = -Wall -Wextra -pedantic -Wmissing-prototypes \
              -Wold-style-definition -Wno-unused-parameter

SRC         = argoat.c util.c mandel.c
OBJ         = $(SRC:.c=.o)
HDR         = $(SRC:.c=.h)

INC         = -Isub/ccommon/

CC          = gcc
CFLAGS      = -std=c99 -O4 $(WARN) $(INC) -ggdb
LDFLAGS     =

all: $(NAME)

$(SRC): $(HDR)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(NAME)

.PHONY: all clean
