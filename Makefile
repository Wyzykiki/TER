CC=g++ -g
CXXFLAGS=
LDFLAGS=
EXEC=popCentre entCentre affiche
SRC= parser.cc lexer.cc especemoleculaire.cc reaction.cc simulation.cc populationCentered.cc molecule.cc env_entite_centre.cc
OBJ= $(SRC:.cc=.o)

all: $(EXEC)

affiche: $(OBJ) Affiche.o
	$(CC) -o $@ $^ -lglut -lGLU -lGL

popCentre: $(OBJ) popMain.o
	$(CC) -o $@ $^ $(LDFLAGS)

entCentre: $(OBJ) entMain.o
	$(CC) -o $@ $^ $(LDFLAGS)

parser.cc: parser.y especemoleculaire.h reaction.h simulation.h molecule.h
	bison -d -o $@ $<

lexer.cc: lexer.l parser.hh
	flex -o$@ $<

%.o : %.cc %.h
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o
	rm -rf parser.cc
	rm -rf parser.hh
	rm -rf lexer.cc