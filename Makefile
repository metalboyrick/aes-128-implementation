make_aes: main.o aes.o
	gcc main.o aes.o -o main.out

main.o: main.c
	gcc -c main.c

aes.o: aes.c aes.h
	gcc -c aes.c

clean:
	rm *.out
	rm *.o