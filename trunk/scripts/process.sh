#!/bin/bash

# Get a lot of stats from the DB

CHECKDB=~/AQUIRIN/program/VisualScienceMaps/CheckDB/checkdb
KFAST=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/MST-Pathfinder/mst-pathfinder
LOG=./process.log

rm -f $LOG
printf "\n\n*** BY COUNTRY ***\n\n" >> $LOG
printf "COUNTRY COUNT\n" >> $LOG
for COUNTRY in `ls ./net/ | cut -f1 -d'-' | sort | uniq`
do
	COUNT=`ls ./net/$COUNTRY*.net | wc -l`
	printf "%s %d\n" $COUNTRY $COUNT >> $LOG
done

printf "\n\n*** BY YEAR ***\n\n" >> $LOG
printf "YEAR COUNT\n" >> $LOG
for YEAR in `ls ./net/ | cut -f2 -d'-' | cut -f1 -d'.' | sort | uniq`
do
	COUNT=`ls ./net/*$YEAR.net | wc -l`
	printf "%d %d\n" $YEAR $COUNT >> $LOG
done

printf "\n\n*** BY FILE ***\n\n" >> $LOG
printf "FILENAME LINES BYTES num_nodes num_edges graph_type symmetry num_doubled num_diff_doubled central_node central_name cycles_map cycles_PFNET\n" >> $LOG
for MAP in `ls ./net`
do
	LINES=`wc -l ./net/$MAP | cut -f1 -d' '`
	BYTES=`wc -c ./net/$MAP | cut -f1 -d' '`
	NODES=`$CHECKDB -i "" -n num_nodes -p -f ./net/$MAP 2> /dev/null`
	EDGES=`$CHECKDB -i "" -n num_edges -p -f ./net/$MAP 2> /dev/null`
	TYPE=`$CHECKDB -i "" -n graph_type -p -f ./net/$MAP 2> /dev/null`
	SYM=`$CHECKDB -i "" -n symmetry -p -f ./net/$MAP 2> /dev/null`
	NUMDBL=`$CHECKDB -i "" -n num_doubled -p -f ./net/$MAP 2> /dev/null`
	NUMDDBL=`$CHECKDB -i "" -n num_diff_doubled -p -f ./net/$MAP 2> /dev/null`
	CENTRAL_NODE=`$CHECKDB -i "" -n central -f ./net/$MAP 2> /dev/null | tail -n 1 | cut -f2 -d' '`
	CENTRAL_NAME=`$CHECKDB -i "" -n central -f ./net/$MAP 2> /dev/null | tail -n 1 | cut -f2 -d"'" | tr -d '\n' | tr ' ' '_'`
	$KFAST ./net/$MAP > ./tmp.pf
	CYCLES_MAP=`$CHECKDB -i "" -n cycles -f ./net/$MAP 2> /dev/null`
	CYCLES_PF=`$CHECKDB -i "" -n cycles -f ./tmp.pf 2> /dev/null`
	printf "%s %d %d %d %d %s %d %s %s %s %s %d %d\n" $MAP $LINES $BYTES $NODES $EDGES $TYPE $SYM $NUMDBL $NUMDDBL $CENTRAL_NODE $CENTRAL_NAME $CYCLES_MAP $CYCLES_PF >> $LOG
done
rm -f ./tmp.pf
