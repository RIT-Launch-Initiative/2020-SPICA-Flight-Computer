# objects = unittest.o MTK.o
objects = MTK.o

all: $(objects)
	gcc -o program $(objects)

# unittest.o: unittest.c MTK.c
# 	gcc -c unittest.c

MTK.o: MTK.cpp
	gcc -c MTK.cpp

run:
	./program

leak:
		valgrind --leak-check=yes ./program

clean:
	rm -f *.o
