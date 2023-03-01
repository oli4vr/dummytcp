all:
	gcc -c tcpd.c -o tcpd.o -O3 -lpthread
	gcc dummysrv.c -o dummysrv -O3 tcpd.o -lpthread
	gcc dummycnt.c -o dummycnt -O3
clean:
	rm *.o
	rm dummysrv
	rm dummycnt
install:
	mkdir -p ~/bin
	cp dummysrv ~/bin
	cp dummycnt ~/bin
