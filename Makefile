make_aes: main.o aes.o
	gcc -g main.o aes.o -o main.out

windows: main.o aes.o
	gcc -g main.o aes.o -o main.exe

main.o: main.c
	gcc -g -c main.c

aes.o: aes.h aes.c 
	gcc -g -c aes.c

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *.exe.stackdump