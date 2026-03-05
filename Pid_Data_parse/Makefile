#Defines
cc := gcc
oflags := -o
cflags := -Wall
exe := Pid_Data_parse


.c.o:
	$(cc) -c $(cflags) $<

srcfiles := $(wildcard *.c)
objfiles := $(addsuffix .o, $(basename $(srcfiles)))

Pid_Data_parse: $(objfiles)
	$(cc) $(oflags) $(exe) $(objfiles)
	
all: Pid_Data_parse

.PHONY: clean
.PHONY: rebuild
.PHONY: all

clean:
	rm -rf $(objfiles)

rebuild: clean all
