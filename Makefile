EXE ?= start

CC := g++
DEBUG := -g
CFLAGS := -Wall -Wextra -Werror -std=c++11 -pedantic
LIBS := -lm

SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:%=./%.o)
DEPS := $(OBJS:.o=.d)

print-%  : ; @echo $* = $($*)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.cpp.o: %.cpp
	$(CC) -c $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm *.o $(EXE)
