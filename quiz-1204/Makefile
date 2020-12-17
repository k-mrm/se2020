T3 = kr

CC = g++
CFLAGS = -g -c -Wall -O0
LDFLAGS = -lm -pthread 

OBJ_T3 = kr.o

all: $(T3)

$(T3): $(OBJ_T3)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T3) 
