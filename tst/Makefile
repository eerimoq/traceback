INC += \
	. \
	../include
SRC += \
	narwhal.c \
	../src/traceback.c \
	main.c
CFLAGS += \
	-Werror \
	-O0 \
	-g \
	-Wall \
	-fprofile-arcs \
	-ftest-coverage \
	$(INC:%=-I%)

test:
	$(CC) $(CFLAGS) $(SRC) -o main
	./main
