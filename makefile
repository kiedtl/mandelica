NAME        = mandel

INC         = -I. -Isub/ccommon/

CC          = gcc
CFLAGS      = -O0 $(INC) -ggdb

$(NAME): $(NAME).o
	$(CC) $(CFLAGS) -o $@ $^
