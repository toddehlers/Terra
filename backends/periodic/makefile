CC = g++
CFLAGS = -W -Wall -O2

periodic : periodic.o getopt.o
	$(CC) $(CFLAGS) getopt.o periodic.o -o periodic

periodic.o : periodic.cc
	$(CC) $(CFLAGS) -c periodic.cc -o periodic.o

getopt.o : getopt.h getopt.cc
	$(CC) $(CFLAGS) -c getopt.cc -o getopt.o

clean :
	$(RM) core* *.o

distclean:
	$(RM) core* *.o periodic

