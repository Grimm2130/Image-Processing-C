CC=gcc
CFLAG=-g
TARGET:main
OBJS=ImageHandler.o

main:main.o ${OBJS}
	${CC} ${CFLAG} main.o ${OBJS} -o main

main.o:main.c
	${CC} ${CFLAG} -c main.c -o main.o

ImageHandler.o:ImageHandler.c
	${CC} ${CFLAG} -c ImageHandler.c -o ImageHandler.o

clean:
	rm -rf *.o main

remove_copies:
	rm -rf TestImages/*copy.bmp