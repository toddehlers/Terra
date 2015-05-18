CC = g++
CFLAGS = -W -Wall -g2
OBJS = ahe.o aft.o zft.o getopt.o age_prediction.o

age_prediction : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o age_prediction

age_prediction.o : utils.h age_prediction.cc
	$(CC) $(CFLAGS) -c age_prediction.cc -o age_prediction.o

ahe.o : ahe.cc
	$(CC) $(CFLAGS) -c ahe.cc -o ahe.o

aft.o : aft.cc
	$(CC) $(CFLAGS) -c aft.cc -o aft.o

zft.o : zft.cc
	$(CC) $(CFLAGS) -c zft.cc -o zft.o

getopt.o : getopt.h getopt.cc
	$(CC) $(CFLAGS) -c getopt.cc -o getopt.o

clean:
	$(RM) core* *.o age_prediction

distclean:
	$(RM) core* *.o age_prediction
