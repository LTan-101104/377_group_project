_DEPS = my_malloc.h
_OBJ = my_malloc.o
_MOBJ = main.o
_TOBJ = test.o

APPBIN = allocator_app
TESTBIN = allocator_test

DEBUG = -DDEBUGMODE

IDIR = include
CC = g++
CFLAGS = -I$(IDIR) -I/opt/homebrew/include -Wall $(DEBUG) -Wextra -g -pthread -std=c++14
ODIR = obj
SDIR = src
LDIR = lib
TDIR = test
LIBS = -lm
XXLIBS = $(LIBS) -L/opt/homebrew/lib -lstdc++ -lgtest -lgtest_main -lpthread
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
MOBJ = $(patsubst %,$(ODIR)/%,$(_MOBJ))
TOBJ = $(patsubst %,$(ODIR)/%,$(_TOBJ)) 

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(TDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(APPBIN) $(TESTBIN) submission

$(APPBIN): $(OBJ) $(MOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(TESTBIN): $(TOBJ) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(XXLIBS)

submission:
	zip -r submission src lib include


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(APPBIN) $(TESTBIN)
	rm -f submission.zip
