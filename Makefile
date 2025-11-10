flag=-W -Wall -std=c89 -pedantic -O2 -c

snake: build/main.o
	gcc -o snake build/main.o

build/main.o: src/main.c
	gcc $(flag) src/main.c -o build/main.o


clean:
	rm build/*.o
	rm snake



