OBJ = main.o
CFLAGS = -Wall

all: bin

bin: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
