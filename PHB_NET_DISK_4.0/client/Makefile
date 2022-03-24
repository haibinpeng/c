SOUR:=$(wildcard *.c)
ELF:=client
OBJS:=$(SOUR:%.c=%.o)
CFLAGS:=$(shell mysql_config --cflags)
$(ELF):$(OBJS)
	gcc $^ -o $@ $(CFLAGS) -g -lmysqlclient -lpthread -lcrypt 
clean:
	rm -rf $(ELF) $(OBJS)
