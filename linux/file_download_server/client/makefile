SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
client:$(OBJS)
	gcc $^ -o $@ -lpthread -g -lcrypt
%.o:%.c
	gcc -c $^ -o $@ -g -I ../include
.PHONY:clean rebuild
clean:
	$(RM) $(EXES) $(OBJS)
rebuild:clean main
