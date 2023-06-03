CC=gcc
CFLAG=-g
TARGET:main
OBJS=ImageHandler.o			\
	GnuPlotter.o			\
	Utils.o

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

clean:
	rm -rf *.o main *.txt

remove_copies:
	rm -rf TestImages/*copy.bmp