SOUR:=$(wildcard *.c)
ELF:=GreatServer
OBJS:=$(SOUR:%.c=%.o)
CFLAGS:=-I/usr/include/mysql -g
$(ELF):$(OBJS)
	gcc $^ -o $@ $(CFLAGS) -lmysqlclient -lpthread -lcrypt -g
clean:
	rm -rf $(ELF) $(OBJS)
