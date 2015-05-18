CC = g++
CFLAGS = -W -Wall -O2

transient : transient.o getopt.o
	$(CC) $(CFLAGS) getopt.o transient.o -o transient

transient.o : transient.cc
	$(CC) $(CFLAGS) -c transient.cc -o transient.o

getopt.o : getopt.h getopt.cc
	$(CC) $(CFLAGS) -c getopt.cc -o getopt.o

clean :
	$(RM) core* *.o

distclean:
	$(RM) core* *.o transient

