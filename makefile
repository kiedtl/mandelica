NAME        = mandel

INC         = -I. -Isub/ccommon/

CC          = gcc
CFLAGS      = -O4 $(INC) -ggdb

$(NAME): $(NAME).o
	$(CC) $(CFLAGS) -o $@ $^
