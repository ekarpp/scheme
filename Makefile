CCFLAGS := -O0 -std=gnu99 -W -g
PROG := scheme
SRCS := main.c src/*.c

all: $(PROG) tests

$(PROG): $(SRCS)
	$(CC) $(CCFLAGS) -o $@ $(SRCS)


tests: $(PROG)
	for t in $(shell ls tests); do \
		./scheme tests/$$t; \
	done

clean:
	rm -f $(PROG) *.o
