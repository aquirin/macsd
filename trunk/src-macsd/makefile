all:	all-debug all-release

all-debug:	debug-shape debug-vmap debug-go 

all-release:	release-set release-matrix release-vector


debug-shape:	clean
	cp -f shapes/config.txt config.txt
	cp shapes/shapes.orig.cpp shapes.cpp
	cp shapes/shapes.orig.h shapes.h
	cp shapes/posibilidades.template posibilidades.template
	cp shapes/posibilidades.h posibilidades.h
	g++ -g -O0 -c -DVERSION=V_SHAPE shapes.cpp
	gcc -g -O0 -c routines.c
	g++ -g -O0 -c -DVERSION=V_SHAPE hormigas.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE NDominatedSet.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE macs.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE main.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE aco.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE utils.cpp
	g++ -g -O0 -c -DVERSION=V_SHAPE global.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-shape.dbg routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o global.o
	echo "exe-shape.dbg has been generated!"
# 	g++ -g -O0 -c -DVERSION=V_SHAPE shapes/load.cpp
# 	g++ -o exe-load_shape routines.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o load.o global.o
# 	echo "exe-load_shape has been generated!"

debug-vmap:	clean
# 	cp -f vmap/config.txt config.txt
	cp shapes/shapes.orig.cpp shapes.cpp
	cp shapes/shapes.orig.h shapes.h
	cp shapes/posibilidades.template posibilidades.template
	cp shapes/posibilidades.h posibilidades.h
	cp vmap/vmap.cpp vmap.cpp
	cp vmap/vmap.h vmap.h
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP shapes.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP vmap.cpp
	gcc -g -O0 -c routines.c
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP hormigas.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP NDominatedSet.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP macs.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP main.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP aco.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP utils.cpp
	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP global.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-vmap.dbg routines.o main.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o vmap.o global.o
	echo "exe-vmap.dbg has been generated!"
# 	g++ -g -O0 -c -DVERSION=V_SCIENCEMAP vmap/load.cpp
# 	g++ -o exe-load_vmap routines.o macs.o shapes.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o load.o vmap.o global.o
# 	echo "exe-load_vmap has been generated!"
	
debug-www:	clean
	cp shapes/shapes.orig.cpp shapes.cpp
	cp shapes/shapes.orig.h shapes.h
	cp shapes/posibilidades.template posibilidades.template
	cp shapes/posibilidades.h posibilidades.h
	cp www/www.h www.h
	cp www/www.cpp www.cpp
	g++ -g -O0 -c -DVERSION=V_WWW shapes.cpp
	g++ -g -O0 -c -DVERSION=V_WWW www.cpp
	gcc -g -O0 -c routines.c
	g++ -g -O0 -c -DVERSION=V_WWW hormigas.cpp
	g++ -g -O0 -c -DVERSION=V_WWW NDominatedSet.cpp
	g++ -g -O0 -c -DVERSION=V_WWW macs.cpp
	g++ -g -O0 -c -DVERSION=V_WWW main.cpp
	g++ -g -O0 -c -DVERSION=V_WWW aco.cpp
	g++ -g -O0 -c -DVERSION=V_WWW utils.cpp
	g++ -g -O0 -c -DVERSION=V_WWW global.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-www.dbg routines.o main.o macs.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o www.o global.o shapes.o
	echo "exe-www.dbg has been generated!"
# 	g++ -g -O0 -c -DVERSION=V_WWW www/load.cpp
# 	g++ -o exe-load_www routines.o macs.o hormigas.o NDominatedSet.o aco.o utils.o Parameter.o load.o www.o global.o shapes.o
# 	echo "exe-load_www has been generated!"

debug-go:	clean
	cp -f go/config.txt config.txt
	cp shapes/shapes.orig.cpp shapes.cpp
	cp shapes/shapes.orig.h shapes.h
	cp shapes/posibilidades.template posibilidades.template
	cp shapes/posibilidades.h posibilidades.h
	cp go/ontologia.h ontologia.h
	cp go/ontologia.cpp ontologia.cpp
	g++ -g -O0 -c -DVERSION=V_GO shapes.cpp
	g++ -g -O0 -c -DVERSION=V_GO ontologia.cpp
	gcc -g -O0 -c routines.c
	g++ -g -O0 -c -DVERSION=V_GO hormigas.cpp
	g++ -g -O0 -c -DVERSION=V_GO NDominatedSet.cpp
	g++ -g -O0 -c -DVERSION=V_GO macs.cpp
	g++ -g -O0 -c -DVERSION=V_GO main.cpp
	g++ -g -O0 -c -DVERSION=V_GO aco.cpp
	g++ -g -O0 -c -DVERSION=V_GO utils.cpp
	g++ -g -O0 -c -DVERSION=V_GO global.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-go.dbg routines.o main.o macs.o shapes.o hormigas.o ontologia.o NDominatedSet.o aco.o utils.o Parameter.o global.o
	echo "exe-go.dbg has been generated!"
# 	g++ -g -O0 -c -DVERSION=V_GO go/load.cpp
# 	g++ -o exe-load routines.o macs.o shapes.o hormigas.o ontologia.o NDominatedSet.o aco.o utils.o Parameter.o load.o global.o
# 	echo "exe-load_go has been generated!"
	
	
release-set:	clean
	cp go/shapes.set.cpp go/shapes.cpp
	cp go/shapes.set.h go/shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/shapes.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/ontologia.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP hormigas.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP NDominatedSet.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP macs.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP main.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP aco.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP utils.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-set routines.o main.o macs.o shapes.o hormigas.o ontologia.o NDominatedSet.o aco.o utils.o Parameter.o
	echo "exe-set has been generated!"
	
	
release-matrix:	clean
	cp go/shapes.matrix.cpp go/shapes.cpp
	cp go/shapes.matrix.h go/shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/shapes.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/ontologia.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP hormigas.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP NDominatedSet.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP macs.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP main.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP aco.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP utils.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-matrix routines.o main.o macs.o shapes.o hormigas.o ontologia.o NDominatedSet.o aco.o utils.o Parameter.o
	echo "exe-matrix has been generated!"

	
release-vector:	clean
	cp go/shapes.vector.cpp go/shapes.cpp
	cp go/shapes.vector.h go/shapes.h
	gcc -O2 -c routines.c
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/shapes.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP go/ontologia.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP hormigas.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP NDominatedSet.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP macs.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP main.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP aco.cpp
	g++ -O2 -c -DVERSION=V_SCIENCEMAP utils.cpp
	g++ -g -O0 -c Parameter.cpp
	g++ -o exe-vector routines.o main.o macs.o shapes.o hormigas.o ontologia.o NDominatedSet.o aco.o utils.o Parameter.o
	echo "exe-vector has been generated!"


clean:
	rm -f *.o exe exe-shape.dbg exe-vmap.dbg exe-go.dbg exe-load exe-set exe-matrix exe-vector exe-load_vmap exe-load_shape


exe-old:
	./exe rocio1 rocio1.out 1 10 10 0.1 0.2 0.5 0.4 0.5 5 3
	./exe /home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/maps/sjr/net.macsd/France-2000.net.pf.sub France-2000.out 1 10 10 0.1 0.2 0.5 0.4 0.5 295 1
	valgrind -v --leak-check=yes --show-reachable=yes ./exe.dbg rocio1 rocio1.out 1 10 10 0.1 0.2 0.5 0.4 0.5 5 3


exe-shape:	debug-shape
	./exe-shape.dbg


exe-vmap:	debug-vmap
	./exe-vmap.dbg


exe-go: debug-go
	./exe-go.dbg
