make_aes: main.o aes.o
	gcc -g main.o aes.o -o main.out

main.o: main.c
	gcc -g -c main.c

aes.o: aes.h aes.c 
	gcc -g -c aes.c

clean:
	rm *.out
	rm *.o