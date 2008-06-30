all:	clean debug release-set release-matrix release-vector

debug:
	gcc -g -O0 -c routines.c
	g++ -g -O0 -c shapes.cpp
	g++ -g -O0 -c hormigas.cpp
	g++ -g -O0 -c NDominatedSet.cpp
	g++ -g -O0 -c macs.cpp
	g++ -g -O0 -c main.cpp
	g++ -g -O0 -c aco.cpp
	g++ -g -O0 -c utils.cpp
	g++ -o exe.dbg routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o

release-set:
	cp shapes.set.cpp shapes.cpp
	cp shapes.set.h shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c shapes.cpp
	g++ -O2 -c hormigas.cpp
	g++ -O2 -c NDominatedSet.cpp
	g++ -O2 -c macs.cpp
	g++ -O2 -c main.cpp
	g++ -O2 -c aco.cpp
	g++ -O2 -c utils.cpp
	g++ -o exe-set routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o
	
release-matrix:
	cp shapes.matrix.cpp shapes.cpp
	cp shapes.matrix.h shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c shapes.cpp
	g++ -O2 -c hormigas.cpp
	g++ -O2 -c NDominatedSet.cpp
	g++ -O2 -c macs.cpp
	g++ -O2 -c main.cpp
	g++ -O2 -c aco.cpp
	g++ -O2 -c utils.cpp
	g++ -o exe-matrix routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o

release-vector:
	cp shapes.vector.cpp shapes.cpp
	cp shapes.vector.h shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c shapes.cpp
	g++ -O2 -c hormigas.cpp
	g++ -O2 -c NDominatedSet.cpp
	g++ -O2 -c macs.cpp
	g++ -O2 -c main.cpp
	g++ -O2 -c aco.cpp
	g++ -O2 -c utils.cpp
	g++ -o exe-vector routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o


clean:
	rm -f *.o exe exe-set exe-matrix exe-vector exe.dbg

exe:
	./exe rocio1 rocio1.out 1 10 10 0.1 0.2 0.5 0.4 0.5 0 5 3
	./exe /home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/maps/sjr/net.macsd/France-2000.net.pf.sub France-2000.out 1 10 10 0.1 0.2 0.5 0.4 0.5 0 295 1
	valgrind -v --leak-check=yes --show-reachable=yes ./exe.dbg rocio1 rocio1.out 1 10 10 0.1 0.2 0.5 0.4 0.5 0 5 3
