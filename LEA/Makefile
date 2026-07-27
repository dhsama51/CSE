CC = gcc

OBJS = LEA.o LEAGlobal.o LEAEncDec.o LEAPad.o LEAMode.o LEAMOVS.o

LEA.exe: $(OBJS)
	$(CC) -g -o $@ $(OBJS)

%.o: %.c
	$(CC) -g -c $<

clean:
	rm -f *.o LEA.exe