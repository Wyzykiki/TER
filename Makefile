CC=g++ -g
CXXFLAGS=
LDFLAGS=
EXEC=simulation
SRC= parser.cc lexer.cc especemoleculaire.cc reaction.cc simulation.cc populationCentered.cc molecule.cc env_entite_centre.cc entMain.cc
OBJ= $(SRC:.cc=.o)

all: $(EXEC)

simulation: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.cc: %.h

parser.cc: parser.y especemoleculaire.h reaction.h simulation.h molecule.h
	bison -d -o $@ $<

lexer.cc: lexer.l parser.hh
	flex -o$@ $<

%.o : %.cc
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o
	rm -rf parser.cc
	rm -rf parser.hh
	rm -rf lexer.cc