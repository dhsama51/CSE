CC = gcc

OBJS = main.o bigIntPlusMinus.o bigIntIO.o bigIntMulSqrRed.o bigIntTest.o bigIntGlobal.o bigIntInv.o

main.exe: $(OBJS)
	$(CC) -g -o $@ $(OBJS)

%.o: %.c
	$(CC) -g -c $<

clean:
	rm -f *.o main.exe