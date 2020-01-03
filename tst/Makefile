INC += .
INC += ../include
INC += ../3pp/subprocess/include

SRC += nala.c
SRC += ../src/traceback.c
SRC += ../3pp/subprocess/src/subprocess.c
SRC += main.c

CFLAGS += -Werror
CFLAGS += -O0
CFLAGS += -no-pie
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -ftest-coverage
CFLAGS += $(INC:%=-I%)
CFLAGS += -c

LDFLAGS += -ftest-coverage
LDFLAGS += -no-pie
LDFLAGS += -g

OBJ = $(SRC:%.c=%.o)
EXE = main

test: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJ) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	echo "CC $^"
	$(CC) $(CFLAGS) $^ -o $@
