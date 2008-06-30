#!/bin/bash

# MACSD
MACSD=/home/ECSC/aquirin/AQUIRIN/program/macsd/src/exe-set
NET_DIR=/home/ECSC/aquirin/AQUIRIN/program/macsd/script/net
RES_DIR=/home/ECSC/aquirin/AQUIRIN/program/macsd/script/res

# EXTRA (local)
KFAST=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/MST-Pathfinder/mst-pathfinder
CHECKDB=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/CheckDB/checkdb
ORIG_NET_DIR=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/maps/sjr

# EXTRA (distant)
#KFAST=/home/ECSC/aquirin/macsd/extra/MST-Pathfinder/mst-pathfinder
#CHECKDB=/home/ECSC/aquirin/macsd/extra/CheckDB/checkdb
#ORIG_NET_DIR=/home/ECSC/aquirin/macsd/extra/sjr

TRUE="0 -lt 1"
FALSE="1 -lt 1"

##################################################

if [ $FALSE ]
then
	echo "0 -- Clean the previous data folder"
	rm -rf $NET_DIR/*
	#mkdir $NET_DIR
fi

##################################################

if [ $FALSE ]
then
	echo "0.1 -- Create a global dictionnary of the node's names"
	cat $ORIG_NET_DIR/*.net | grep ellipse | cut -f2 -d'"' | sort | uniq > $RES_DIR/node_names.1.txt
	gawk '{ print FNR ";\"" $0 "\"" }' $RES_DIR/node_names.1.txt > $RES_DIR/node_names.2.txt
fi

##################################################

if [ $FALSE ]
then
	echo "1 -- Copy from $ORIG_NET_DIR to $NET_DIR all the maps needed (180 maps)"
	for C in `cat countries.macsd.txt`
	do
		for Y in `echo 1996 2000 2005`
		do
			cp $ORIG_NET_DIR/$C-$Y.net $NET_DIR
		done
	done
fi

##################################################

if [ $FALSE ]
then
	echo "2 -- Generate the PF maps"
	for M in `ls $NET_DIR/*.net`
	do
		$KFAST $M > $M.pf
	done
fi

##################################################

if [ $FALSE ]
then
	echo "3 -- Layout one maps"
	#./layout_one_map.sh "$NET_DIR/Spain-2000.net.pf"
	#./layout_one_map.sh "$NET_DIR/Austria-2005.net.pf"
	for M in `echo "$NET_DIR/Finland-2005.net.pf $NET_DIR/China-2005.net.pf $NET_DIR/United_States-2005.net.pf $NET_DIR/France-2000.net.pf $NET_DIR/Canada-2005.net.pf $NET_DIR/Germany-2005.net.pf"`
	do
		./layout_one_map.sh "$M"
	done
fi

##################################################

if [ $FALSE ]
then
	echo "4 -- Convert everything in the Subdue format and using a dictionnary (same node name => same node index)"
	rm -f $RES_DIR/checkdb.err
	for M in `ls $NET_DIR/*.pf`
	do
		( $CHECKDB -o subdue,directed,empty,sorted -f $M -i "" -t $RES_DIR/node_names.2.txt > $M.sub ) 2>> $RES_DIR/checkdb.err
		echo "" >> $M.sub
	done
fi

##################################################

if [ $FALSE ]
then
	echo "5 -- Make a big file with the Subdue format"
	
	LISTE="$NET_DIR/Finland-2005.net.pf.sub $NET_DIR/China-2005.net.pf.sub $NET_DIR/United_States-2005.net.pf.sub"		# Some maps with loops
	rm -f $RES_DIR/bigfile1.dat
	for M in $LISTE
	do
		echo "% graph '$M'" >> $RES_DIR/bigfile1.dat
		cat $M >> $RES_DIR/bigfile1.dat
	done
	
	LISTE="$NET_DIR/France-2000.net.pf.sub $NET_DIR/Canada-2005.net.pf.sub $NET_DIR/Germany-2005.net.pf.sub"		# Some maps without loops
	rm -f $RES_DIR/bigfile2.dat
	for M in $LISTE
	do
		echo "% graph '$M'" >> $RES_DIR/bigfile2.dat
		cat $M >> $RES_DIR/bigfile2.dat
	done
	
	LISTE=`ls $NET_DIR/*.sub`		# Everything
	rm -f $RES_DIR/bigfile3.dat
	for M in $LISTE
	do
		echo "% graph '$M'" >> $RES_DIR/bigfile3.dat
		cat $M >> $RES_DIR/bigfile3.dat
	done
fi

##################################################

if [ $TRUE ]
then
	echo "6 -- Run MACSD"
	NUMNODES=`cat $RES_DIR/node_names.2.txt | wc -l`
	NUMEDGES=1
	rm -f $RES_DIR/exe.1.log $RES_DIR/exe.1.err
	#NUMNODES=`cat $RES_DIR/bigfile1.dat | grep "^v " | cut -f2 -d' ' | sort | uniq | wc -l`
	#NUMEDGES=`cat $RES_DIR/bigfile1.dat | grep "^d " | cut -f4 -d' ' | sort | uniq | wc -l`
	#echo $MACSD $RES_DIR/bigfile1.dat $RES_DIR/bigfile1.dat.out 1 1000 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES
	#( ( time $MACSD $RES_DIR/bigfile1.dat $RES_DIR/bigfile1.dat.out 1 1000 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES ) > $RES_DIR/exe.1.log 2> $RES_DIR/exe.1.err ) &
	
	rm -f $RES_DIR/exe.2.log $RES_DIR/exe.2.err
	#NUMNODES=`cat $RES_DIR/bigfile2.dat | grep "^v " | cut -f2 -d' ' | sort | uniq | wc -l`
	#NUMEDGES=`cat $RES_DIR/bigfile2.dat | grep "^d " | cut -f4 -d' ' | sort | uniq | wc -l`
	#echo $MACSD $RES_DIR/bigfile2.dat $RES_DIR/bigfile2.dat.out 1 1000 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES
	#( ( time $MACSD $RES_DIR/bigfile2.dat $RES_DIR/bigfile2.dat.out 1 1000 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES ) > $RES_DIR/exe.2.log 2> $RES_DIR/exe.2.err ) &
	
	rm -f $RES_DIR/exe.3.*.log $RES_DIR/exe.3.*.err
	#NUMNODES=`cat $RES_DIR/bigfile3.dat | grep "^v " | cut -f2 -d' ' | sort | uniq | wc -l`
	#NUMEDGES=`cat $RES_DIR/bigfile3.dat | grep "^d " | cut -f4 -d' ' | sort | uniq | wc -l`
	for run in `seq 1 1`
	do
		echo $MACSD $RES_DIR/bigfile3.dat $RES_DIR/bigfile3.dat.out $run 100 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES
		( ( time $MACSD $RES_DIR/bigfile3.dat $RES_DIR/bigfile3.dat.out.$run $run 100 10 0.1 0.2 0.5 0.4 0.5 1 $NUMNODES $NUMEDGES ) > $RES_DIR/exe.3.$run.log 2> $RES_DIR/exe.3.$run.err )
	done
fi
