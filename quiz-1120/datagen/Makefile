T1 = giant
T2 = s2pl

CC = g++
CFLAGS = -g -c -Wall -O0
LDFLAGS = -lm -pthread 

OBJ_T1 = giant.o
OBJ_T2 = s2pl.o

all: $(T1) $(T2) 

$(T1): $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^
$(T2): $(OBJ_T2)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1) $(T2) $(T3) 
