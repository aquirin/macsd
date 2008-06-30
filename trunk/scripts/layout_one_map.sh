#!/bin/bash

# Layout one map (the given file has to be a NET file (.net, .pf)

CHECKDB=/home/ECSC/aquirin/AQUIRIN/program/VisualScienceMaps/CheckDB/checkdb
MYMAP="$1"
$CHECKDB -f $MYMAP -o dot -i "" > $MYMAP.dot

# Add a title
printf "graph G {\n" > /tmp/search.tmp
printf "graph G {\n" > /tmp/replace.tmp
printf "label=\"FILENAME=$MYMAP\";\n" >> /tmp/replace.tmp
printf "labelloc=\"top\";\n" >> /tmp/replace.tmp
printf "fontsize=100;\n\n" >> /tmp/replace.tmp
./replace_text.pl $MYMAP.dot /tmp/search.tmp /tmp/replace.tmp

dot -Gratio=fill -Gsize=15,15 -Kneato -Tps2 -o $MYMAP.ps $MYMAP.dot
ps2pdf14 $MYMAP.ps $MYMAP.pdf
acroread $MYMAP.pdf&
