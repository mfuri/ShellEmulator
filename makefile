CFLAGS:= -std=gnu11 
src = $(wildcard *.c)
obj = $(src:.c=.o)

#LDFLAGS = -lGL -lglut -lpng -lz -lm

shell: $(obj)
	$(CC) ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) myprog
