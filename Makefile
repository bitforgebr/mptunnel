CC = gcc
CFLAGS = -g -Wall -I/usr/include/libev -O2
LDFLAGS = -g  -lev -pthread -O2

all: mpclient mpserver .locale udpclient udpserver
	$(shell sh ./make-locale.sh)

.locale: locale/zh_CN.po

mpclient: client.o net.o mptunnel.o rbtree.o
	$(CC) $^  -o mpclient $(LDFLAGS)

mpserver: server.c mptunnel.o net.o rbtree.o
	$(CC) $^  -o mpserver $(LDFLAGS)

udpclient: udpclient.c
	$(CC) $^  -o udpclient $(LDFLAGS)

udpserver: udpserver.c
	$(CC) $^  -o udpserver $(LDFLAGS)


SOURCE = $(wildcard *.c)
	sinclude $(SOURCE:.c=.d)
	
%.d: %.c
	$(CC) -MT "$*.o $*.d" -MM $(CFLAGS) $< > $@



 
clean:
	rm -f *.o
	rm -f *.d
	rm -f *.exe
	rm -f mpclient mpserver
	find -name "*.mo" -exec rm -v {} \;
