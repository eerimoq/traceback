INC += \
	../../include \
	../../3pp/subprocess/include
SRC += \
	../../src/traceback.c \
	../../3pp/subprocess/src/subprocess.c \
	main.c
all:
	gcc $(INC:%=-I%) $(SRC) -O0 -g -no-pie -o main
	./main

clean:
	rm -f main
