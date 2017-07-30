EXE = run

CC = g++ 
DEBUG = -g
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
LIBS = -lm

all: $(EXE)

$(EXE): neuron.o main.cpp
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

neuron.o: neuron.cpp
	$(CC) -c $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm *.o $(EXE)
