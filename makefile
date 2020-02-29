
SRC = main.cpp
OBJ = main.o general_includes.o permutation.o
CCC = g++
EXE = scp
OPT = -O3 -fomit-frame-pointer -fno-strict-aliasing -DNDEBUG
DEBUG = -g -DDEBUG

IFLAGS = -I DATASTRUCTURES/HEAP/ -I DATASTRUCTURES/ -I.
LFLAGS = -L. 

main.o: $(SRC)
	$(CCC) $(OPT) $(IFLAGS) -c $(SRC)

main: main.o
	$(CCC) -o $(EXE) $(OBJ)
	/bin/rm -rf main.o
	
clean:
	/bin/rm -rf main.o
	/bin/rm -rf $(EXE)
	touch out.out
	/bin/rm -rf *.out