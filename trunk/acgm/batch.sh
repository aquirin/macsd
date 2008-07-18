#!/bin/bash

# Exemple of an execution of ACGM.

perl ./g2acgm.pl ../data/bigfile2/bigfile2.dat > bigfile2.acgm
./acgm.exe -subgraph -in bigfile2.acgm -sup 0.0000001 -out
