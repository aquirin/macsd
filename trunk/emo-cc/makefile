# Simple Makefile
CXX = g++
CXXFLAGS= -Wall -pedantic-errors -g
MAC = -I/usr/include/malloc
MACSD = ../src-macsd

all:	debug-shapes debug-go

debug-shapes: clean
	cp -f $(MACSD)/shapes/config.txt config.txt
	cp $(MACSD)/shapes/shapes.orig.cpp shapes.cpp
	cp $(MACSD)/shapes/shapes.orig.h shapes.h
	cp $(MACSD)/shapes/posibilidades.template posibilidades.template
	cp $(MACSD)/shapes/posibilidades.h posibilidades.h
	cp $(MACSD)/utils.h utils.h
	cp $(MACSD)/utils.cpp utils.cpp
	cp $(MACSD)/global.h global.h
	cp $(MACSD)/global.cpp global.cpp
	cp $(MACSD)/routines.c routines.c
	cp $(MACSD)/routines.h routines.h
	cp $(MACSD)/base.h base.h
	cp $(MACSD)/Parameter.cpp Parameter.cpp
	cp $(MACSD)/Parameter.h Parameter.h

	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SHAPE shapes.cpp
	gcc $(MAC) -g -O0 -c routines.c
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SHAPE main.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SHAPE utils.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SHAPE global.cpp
	g++ $(MAC) -Wall -g -O0 -c Parameter.cpp
	g++ $(MAC) -Wall -o emocc-shapes.dbg routines.o main.o shapes.o utils.o Parameter.o global.o
	echo "emocc-shapes.dbg has been generated!"

debug-go: clean
	cp -f $(MACSD)/go/config.txt config.txt
	cp $(MACSD)/shapes/shapes.orig.cpp shapes.cpp
	cp $(MACSD)/shapes/shapes.orig.h shapes.h
	cp $(MACSD)/shapes/posibilidades.template posibilidades.template
	cp $(MACSD)/shapes/posibilidades.h posibilidades.h
	cp $(MACSD)/go/ontologia.h ontologia.h
	cp $(MACSD)/go/ontologia.cpp ontologia.cpp
	cp $(MACSD)/utils.h utils.h
	cp $(MACSD)/utils.cpp utils.cpp
	cp $(MACSD)/global.h global.h
	cp $(MACSD)/global.cpp global.cpp
	cp $(MACSD)/routines.c routines.c
	cp $(MACSD)/routines.h routines.h
	cp $(MACSD)/base.h base.h
	cp $(MACSD)/Parameter.cpp Parameter.cpp
	cp $(MACSD)/Parameter.h Parameter.h

	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_GO shapes.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_GO ontologia.cpp
	gcc $(MAC) -g -O0 -c routines.c
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_GO main.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_GO utils.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_GO global.cpp
	g++ $(MAC) -Wall -g -O0 -c Parameter.cpp
	g++ $(MAC) -Wall -o emocc-go.dbg routines.o main.o shapes.o ontologia.o utils.o Parameter.o global.o
	echo "emocc-go.dbg has been generated!"

clean:
	rm -f *.o emocc-*
