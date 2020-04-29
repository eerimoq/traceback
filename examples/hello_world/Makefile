INC += ../../include
INC += ../../3pp/subprocess/include
INC += ../../3pp/whereami/src
SRC += ../../src/traceback.c
SRC += ../../3pp/subprocess/src/subprocess.c
SRC += ../../3pp/whereami/src/whereami.c
SRC += main.c

all:
	gcc $(INC:%=-I%) $(SRC) -O0 -g -no-pie -o main
	./main

clean:
	rm -f main
