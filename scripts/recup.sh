#!/bin/bash

# Goal: download all the maps (NET files) for all the years and all the countries.

unset PATH
export PATH=/bin:/usr/bin/
export http_proxy=http://proxy.ecsc.softcomputing.es:3128

FOLDER=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/maps/sjr
rm -f $FOLDER/wget.log $FOLDER/wget.err $FOLDER/file.tmp

## STEP 1 : Get all the maps from the website, and put them in ./net

for COUNTRY in `cat $FOLDER/countries.txt | cut -f1 | tr '\n' ' '`
do
	for YEAR in `seq 1995 2007`
	do
		COUNTRYNAME=`cat $FOLDER/countries.txt | grep "^$COUNTRY" | cut -f2 | tr -d "," | tr -s " " | tr " " "_"`
		URL="http://www.scimagojr.com/inestable2/con_mapgen_b.php?un=c&year=$YEAR&country=$COUNTRY&type=c#"
		echo "Get $COUNTRYNAME-$YEAR.net ..."
		# Get the file from the web of SJR
		wget --output-document="$FOLDER/file.tmp" $URL >> $FOLDER/wget.log 2>> $FOLDER/wget.err
		# Check if there is some data
		if [ "`cat $FOLDER/file.tmp`" == "nodata" ]
		then
			# No data! Delete the file
			rm -f "$FOLDER/file.tmp"
		else
			cat "$FOLDER/file.tmp" | sed "s/<br>/\n/g" > "$FOLDER/net/$COUNTRYNAME-$YEAR.net"
			rm -f "$FOLDER/file.tmp"
		fi
		sleep 5
	done
done

## STEP 2 : Zip everything

#rm -f $FOLDER/sjrmaps.zip
#zip -rq9j $FOLDER/sjrmaps.zip $FOLDER/net/*

## STEP 3 : Send it to the FTP server

#cd $FOLDER/ && /home/ECSC/aquirin/ftp/send.sh sjrmaps.zip
