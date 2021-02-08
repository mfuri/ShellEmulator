CFLAGS:= -std=gnu11 
src = $(wildcard *.c)
obj = $(src:.c=.o)

#LDFLAGS = -lGL -lglut -lpng -lz -lm

shell: $(obj)
	gcc -std=gnu11 -o shell *.c

.PHONY: clean
clean:
	rm -f $(obj) myprog
