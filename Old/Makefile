all: proxy_airlock

proxy: proxy.c lib/libproxy.a
	gcc -o proxy proxy.c lib/*.o -lssl -lcrypto

proxy_airlock: proxy_airlock.c lib/libproxy.a
	gcc -o proxy_airlock proxy_airlock.c lib/*.o -lssl -lcrypto

lib/libproxy.a:
	cd lib
	make

