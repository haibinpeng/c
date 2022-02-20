out:=thread_poll_server
SRCS:=$(wildcard *.c) #拿到当前目录下所有的c文件
OBJS:=$(patsubst %.c,%.o,$(SRCS))
#预定义变量
CC:=gcc
CFLAGS:=-g
#CFLAGS:=-g -D DEBUG #开启调试开关
$(out):$(OBJS)
	gcc $^ -o $@ -pthread

#.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	rm -rf $(OBJS) $(out)
