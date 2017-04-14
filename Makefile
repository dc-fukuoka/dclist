CC     = gcc
CFLAGS = -g -MMD -MP -Wall
LFLAGS =
LIBS   =
OBJS   = dclist.o
BIN    = a.out

.SUFFIXES: .c.o
.PHONY: clean

ALL: $(BIN)

.c.o:
	$(CC) -c $(CFLAGS) $<

$(BIN): $(OBJS) $(LIBS)
	$(CC) $(LFLAGS) $(LIBS) $^ -o $@

clean:
	rm -f *.o *.d *~ $(BIN)

