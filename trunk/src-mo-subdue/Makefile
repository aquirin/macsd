# Makefile
#
# Subdue 5
#
CC =		gcc
#CFLAGS =	-Wall -O3
#LDFLAGS =	-O3
# debug CFLAGS and LDFLAGS
CFLAGS =	-g -pg -Wall -DMODEBUG
LDFLAGS =	-g -pg

MPICC =		mpicc
MPICFLAGS = 	-Wall -O3
MPILDFLAGS =	-O3

LDLIBS =	-lm
OBJS = 		compress.o discover.o dot.o evaluate.o extend.o graphmatch.o\
                graphops.o labels.o sgiso.o subops.o test.o utility.o \
                avl.o gendata.o incboundary.o inccomp.o incextend.o \
                incgraphops.o incutil.o io_net.o memory_allocation.o
TARGETS =	gm gprune graph2dot mdl mo_subdue sgiso subdue subs2dot test cvtest

all: $(TARGETS)
	cp $(TARGETS) ../bin

gm: gm_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o gm gm_main.o $(OBJS) $(LDLIBS)

gprune: gprune_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o gprune gprune_main.o $(OBJS) $(LDLIBS)

graph2dot: graph2dot_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o graph2dot graph2dot_main.o $(OBJS) $(LDLIBS)

mdl: mdl_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o mdl mdl_main.o $(OBJS) $(LDLIBS)
	
mo_subdue: mo_subdue.o $(OBJS)
	$(CC) $(LDFLAGS) -o mo_subdue mo_subdue.o $(OBJS) $(LDLIBS)

mpi_subdue: mpi_main.o mpi.o $(OBJS)
	$(MPICC) $(MPILDFLAGS) -o mpi_subdue mpi_main.o mpi.o $(OBJS) $(LDLIBS)

sgiso: sgiso_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o sgiso sgiso_main.o $(OBJS) $(LDLIBS)

subdue: main.o $(OBJS)
	$(CC) $(LDFLAGS) -o subdue main.o $(OBJS) $(LDLIBS)

subs2dot: subs2dot_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o subs2dot subs2dot_main.o $(OBJS) $(LDLIBS)

test: test_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o test test_main.o $(OBJS) $(LDLIBS)

cvtest: cvtest_main.o $(OBJS)
	$(CC) $(LDFLAGS) -o cvtest cvtest_main.o $(OBJS) $(LDLIBS)

mpi.o: mpi.c subdue.h
	$(MPICC) -c $(MPICFLAGS) mpi.c -o mpi.o

mpi_main.o: mpi_main.c subdue.h
	$(MPICC) -c $(MPICFLAGS) mpi_main.c -o mpi_main.o

%.o: %.c subdue.h
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp $(TARGETS) ../bin

install_mpi:
	cp mpi_subdue ../bin

clean:
	/bin/rm -f *.o $(TARGETS) mpi_subdue ../bin/*

