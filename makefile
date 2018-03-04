main: main.o list.o
	gcc -o s-talk -pthread list.o main.o

main.o: main1.c
	gcc -c main1.c -o main.o

clean:
	rm *.o main
