EXE ?= start

# "python-config --include" or "pythonX.Y-config --include"
# export LD_LIBRARY_PATH=/home/samc/anaconda3/lib:$LD_LIBRARY_PATH
PY_CFLAGS := -I/home/samc/anaconda3/include/python3.6m -isystem/home/samc/anaconda3/lib/python3.6/site-packages/numpy/core/include
PY_LDFLAGS := -L/home/samc/anaconda3/lib -lpython3.6m

CC := g++
DEBUG := -g
#CFLAGS := -Wall -Wextra -Werror -std=c++11 -pedantic $(PY_CFLAGS)
CFLAGS := -Wall -Wextra -std=c++11 -pedantic -D_GLIBCXX_USE_CXX11_ABI=0 $(PY_CFLAGS) 
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
