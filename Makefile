CC=gcc
CFLAG=-g
TARGET:main
OBJS=ImageHandler.o			\
	GnuPlotter.o			\
	Utils.o					\
	Mask.o

main:main.o ${OBJS}
	${CC} ${CFLAG} main.o ${OBJS} -o main

main.o:main.c
	${CC} ${CFLAG} -c main.c -o main.o

ImageHandler.o:ImageHandler.c
	${CC} ${CFLAG} -c ImageHandler.c -o ImageHandler.o

GnuPlotter.o:GnuPlotter.c
	${CC} ${CFLAG} -c GnuPlotter.c -o GnuPlotter.o

Utils.o:Utils.c
	${CC} ${CFLAG} -c Utils.c -o Utils.o

Mask.o:Mask.c
	${CC} ${CFLAG} -c Mask.c -o Mask.o

clean:
	rm -rf *.o main *.txt

remove_copies:
	rm -rf TestImages/*copy.bmp