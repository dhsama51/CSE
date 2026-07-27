CC = gcc

OBJS = main.o bigIntIO.o bigIntPlusMinus.o bigIntMulSqrRed.o bigIntGlobal.o bigIntInv.o \
       ecadd.o ecdbl.o eccsm.o eccGlobal.o

main.exe: $(OBJS)
	$(CC) -g -o $@ $(OBJS)

%.o: %.c
	$(CC) -g -c $<

clean:
	rm -f *.o main.exe