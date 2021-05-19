make_aes: main.o aes.o
	gcc -O main.o aes.o -o main.out

windows: main.o aes.o
	gcc -O main.o aes.o -o main.exe

main.o: main.c
	gcc -c -O main.c

aes.o: aes.h aes.c 
	gcc -c -O aes.c

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *.exe.stackdump