all: libproxy.a

libproxy.a: ssl.o net.o
	ar r libproxy.a ssl.o net.o

ssl.o: ssl.c ssl.h
	gcc -c ssl.c

net.o: net.c net.h
	gcc -c net.c

