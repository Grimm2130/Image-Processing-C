CC=gcc
SRC_PATH=sources/
CFLAG=-g
TARGET:main
OBJS=GnuPlotter.o			\
	Utils.o					\
	Mask.o					\
	BinaryImage_Handler.o	\
	GreyscaleImageHandler.o	\
	RGBImageHandler.o		


main:main.o ${OBJS}
	${CC} ${CFLAG} main.o ${OBJS} -o main

main.o:main.c
	${CC} ${CFLAG} -c main.c -o main.o

BinaryImage_Handler.o:${SRC_PATH}BinaryImage_Handler.c
	${CC} ${CFLAG} -c ${SRC_PATH}BinaryImage_Handler.c -o BinaryImage_Handler.o

GreyscaleImageHandler.o:${SRC_PATH}GreyscaleImageHandler.c
	${CC} ${CFLAG} -c ${SRC_PATH}GreyscaleImageHandler.c -o GreyscaleImageHandler.o

RGBImageHandler.o:${SRC_PATH}RGBImageHandler.c
	${CC} ${CFLAG} -c ${SRC_PATH}RGBImageHandler.c -o RGBImageHandler.o

GnuPlotter.o:${SRC_PATH}GnuPlotter.c
	${CC} ${CFLAG} -c ${SRC_PATH}GnuPlotter.c -o GnuPlotter.o

Utils.o:${SRC_PATH}Utils.c
	${CC} ${CFLAG} -c ${SRC_PATH}Utils.c -o Utils.o

Mask.o:${SRC_PATH}Mask.c
	${CC} ${CFLAG} -c ${SRC_PATH}Mask.c -o Mask.o

clean:
	rm -rf *.o main *.txt

remove_copies:
	rm -rf TestImages/*copy.bmp