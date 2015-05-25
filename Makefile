OBJ = main.o print_to_file.o interface.o
CFLAGS = -Wall

all: bin

bin: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o bin -lncurses -lcdk

%.o : %.c
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
