# Simple Makefile
CXX = g++
CXXFLAGS= -Wall -pedantic-errors -g
MAC = -I/usr/include/malloc
MACSD = ../src-macsd
DATA = ../data

all:	debug-shapes debug-GO debug-www

debug-shapes: clean
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

debug-GO: clean
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
	g++ $(MAC) -Wall -o emocc-GO.dbg routines.o main.o shapes.o ontologia.o utils.o Parameter.o global.o
	echo "emocc-GO.dbg has been generated!"

debug-www: clean
	cp $(MACSD)/shapes/shapes.orig.cpp shapes.cpp
	cp $(MACSD)/shapes/shapes.orig.h shapes.h
	cp $(MACSD)/shapes/posibilidades.template posibilidades.template
	cp $(MACSD)/shapes/posibilidades.h posibilidades.h
	cp $(MACSD)/www/www.h www.h
	cp $(MACSD)/www/www.cpp www.cpp
	cp $(MACSD)/utils.h utils.h
	cp $(MACSD)/utils.cpp utils.cpp
	cp $(MACSD)/global.h global.h
	cp $(MACSD)/global.cpp global.cpp
	cp $(MACSD)/routines.c routines.c
	cp $(MACSD)/routines.h routines.h
	cp $(MACSD)/base.h base.h
	cp $(MACSD)/Parameter.cpp Parameter.cpp
	cp $(MACSD)/Parameter.h Parameter.h

	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_WWW shapes.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_WWW www.cpp
	gcc $(MAC) -g -O0 -c routines.c
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_WWW main.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_WWW utils.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_WWW global.cpp
	g++ $(MAC) -Wall -g -O0 -c Parameter.cpp
	g++ $(MAC) -Wall -o emocc-www.dbg routines.o main.o shapes.o www.o utils.o Parameter.o global.o
	echo "emocc-www.dbg has been generated!"

debug-vmap: clean
	cp $(MACSD)/shapes/shapes.orig.cpp shapes.cpp
	cp $(MACSD)/shapes/shapes.orig.h shapes.h
	cp $(MACSD)/shapes/posibilidades.template posibilidades.template
	cp $(MACSD)/shapes/posibilidades.h posibilidades.h
	cp $(MACSD)/vmap/vmap.h vmap.h
	cp $(MACSD)/vmap/vmap.cpp vmap.cpp
	cp $(MACSD)/utils.h utils.h
	cp $(MACSD)/utils.cpp utils.cpp
	cp $(MACSD)/global.h global.h
	cp $(MACSD)/global.cpp global.cpp
	cp $(MACSD)/routines.c routines.c
	cp $(MACSD)/routines.h routines.h
	cp $(MACSD)/base.h base.h
	cp $(MACSD)/Parameter.cpp Parameter.cpp
	cp $(MACSD)/Parameter.h Parameter.h

	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SCIENCEMAP shapes.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SCIENCEMAP vmap.cpp
	gcc $(MAC) -g -O0 -c routines.c
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SCIENCEMAP main.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SCIENCEMAP utils.cpp
	g++ $(MAC) -Wall -g -O0 -c -DVERSION=V_SCIENCEMAP global.cpp
	g++ $(MAC) -Wall -g -O0 -c Parameter.cpp
	g++ $(MAC) -Wall -o emocc-vmap.dbg routines.o main.o shapes.o vmap.o utils.o Parameter.o global.o
	echo "emocc-vmap.dbg has been generated!"

clean:
	rm -f *.o

