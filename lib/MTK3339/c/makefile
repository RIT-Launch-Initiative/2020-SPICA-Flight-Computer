objects = unittest.o MTK.o

all: $(objects)
	gcc -o program $(objects)

unittest.o: unittest.c MTK.c
	gcc -c unittest.c

MTK.o: MTK.c
	gcc -c MTK.c

run:
	./program

leak:
	valgrind --leak-check=yes ./program

clean:
	rm -f *.o
