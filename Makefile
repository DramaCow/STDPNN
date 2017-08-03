EXE ?= start

# "python-config --include" or "pythonX.Y-config --include"
PY_CFLAGS := -I/home/samc/anaconda3/include/python3.6m -I/home/samc/anaconda3/include/python3.6m
PY_LDFLAGS := -L/home/samc/anaconda3/lib -lpython3.6m

CC := gcc-4.7
DEBUG := -g
CFLAGS := -Wall -Wextra -Werror -std=c++11 -pedantic $(PY_CFLAGS) #$(INCLUDE) $(PY_LIBS)
LIBS := -lm $(PY_LDFLAGS)

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
