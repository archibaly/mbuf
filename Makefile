CC = gcc
CFLAGS = -Wall

EXE = mbuf

$(EXE): main.o mbuf.o writen.o
	$(CC) -o $@ $^

main.o: main.c
mbuf.o: mbuf.c list.h
writen.o: writen.c

clean:
	rm -f *.o $(EXE)
