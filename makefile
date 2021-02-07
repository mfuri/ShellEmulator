src = $(wildcard *.c)
obj = $(src:.c=.o)

#LDFLAGS = -lGL -lglut -lpng -lz -lm

shell: $(obj)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) myprog