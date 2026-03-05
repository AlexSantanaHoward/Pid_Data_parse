.c.o:
	gcc -c $<

srcfiles := $(wildcard *.c)
objfiles := $(addsuffix .o, $(basename $(srcfiles)))

Pid_Data_parse: $(objfiles)
	ld $(objfiles) -o $<
	
all: Pid_Data_parse

.PHONY: clean
.PHONY: rebuild
.PHONY: all

clean:
	rm -rf $(objfiles)

rebuild: clean all
