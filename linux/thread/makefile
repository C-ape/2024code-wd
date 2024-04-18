SRCS:=$(wildcard *.c)
OBJS:=$(wildcard *.o)
EXES:=$(patsubst %.c,%,$(SRCS))
all:$(EXES)
%:%.c
	gcc $^ -o $@ -g -I ../include -lpthread
.PHONY:clean rebuild
clean:
	$(RM) $(EXES) $(OBJS)
rebuild:clean main
