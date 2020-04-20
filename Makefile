CCFLAGS := -O0 -std=gnu99 -W
PROG := scheme
#OBJS := main.o interpretor.o lexer.o parser.o exec.o hashmap.o
SRCS := main.c src/*.c

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CCFLAGS) -o $@ $(SRCS)

#all: objs $(PROG)

#objs:
#	$(CC) $(CCFLAGS) -c $(SRCS)

#$(PROG): $(OBJS)
#	$(CC) $(CCFLAGS) $(OBJS) -o $@

clean:
	rm -f $(PROG) *.o
