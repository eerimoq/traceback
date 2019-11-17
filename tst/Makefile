INC += \
	. \
	../include
SRC += \
	nala.c \
	../src/traceback.c \
	main.c
CFLAGS += \
	-Werror \
	-O1 \
	-no-pie \
	-g \
	-Wall \
	-fprofile-arcs \
	-ftest-coverage \
	$(INC:%=-I%)

test:
	$(CC) $(CFLAGS) $(SRC) -o main
	./main
