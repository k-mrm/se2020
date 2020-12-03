T1 = s2pl
T2 = s2pl-latch
T3 = gen_dataset

CC = g++
CFLAGS = -g -c -Wall -O0 -std=c++11
LDFLAGS = -lm -pthread

OBJ_T1 = s2pl.o
OBJ_T2 = s2pl-latch.o
OBJ_T3 = gen_dataset.o

all: $(T1) $(T2) $(T3) 

$(T1): $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^
$(T2): $(OBJ_T2)
	$(CC) $(LDFLAGS) -o $@ $^
$(T3): $(OBJ_T3)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1) $(T2) $(T3) 
