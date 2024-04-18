SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
server:$(OBJS)
	gcc $^ -o $@ -lpthread -lmysqlclient -lcrypt
%.o:%.c
	gcc -c $^ -o $@ -g -I ../include -O0
.PHONY:clean rebuild
clean:
	$(RM) $(EXES) $(OBJS)
rebuild:clean main
