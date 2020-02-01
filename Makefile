CC=g++
CXXFLAGS=
LDFLAGS=
EXEC=simulation
SRC= simulation.cc parser.cc lexer.cc especemoleculaire.cc reaction.cc
OBJ= $(SRC:.cc=.o)

all: $(EXEC)

simulation: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

parser.cc: parser.y especemoleculaire.h reaction.h
	bison -d -o $@ $<

lexer.cc: lexer.l parser.h
	flex -o$@ $<

%.o : %.cc
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o