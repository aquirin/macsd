all:	scripts

scripts:	clean
	g++ -DVERSION=V_SHAPE -I../src-macsd/ -I../src-macsd/shapes/ -o sacar_dominados sacar_nodominados.cpp ../src-macsd/shapes.o ../src-macsd/NDominatedSet.o ../src-macsd/hormigas.o
	echo "sacar_dominados has been generated!"
	g++ -DVERSION=V_SHAPE -I../src-macsd/ -I../src-macsd/shapes/ -o exhaustive_shapes exhaustive_shapes.cpp ../src-macsd/shapes.o ../src-macsd/NDominatedSet.o ../src-macsd/hormigas.o
	echo "exhaustive_shapes has been generated!"
	g++ -DVERSION=V_SHAPE -I../src-macsd/ -I../src-macsd/shapes/ -o generar_DB generar_DB.cpp ../src-macsd/shapes.o ../src-macsd/NDominatedSet.o ../src-macsd/hormigas.o
	echo "generar_DB has been generated!"

clean:
	rm -f *.o sacar_dominados exhaustive_shapes generar_DB
