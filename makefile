NAME        = mandel

INC         = -I. -Isub/ccommon/

CC          = gcc
CFLAGS      = -O4 $(INC)

$(NAME): $(NAME).o
	$(CC) $(CFLAGS) -o $@ $^
