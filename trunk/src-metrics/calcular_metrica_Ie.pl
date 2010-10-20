#!/usr/bin/perl

use strict;
use warnings;
use List::Util qw[min max];

my ($in1, $in2) = @ARGV;

# Cargo el pareto 1
my $id = 0;
my $soporte;
my $costos;
my %pareto_sop1 = ();
my %pareto_cos1 = ();
open(IN, $in1);
my $i = 1;
my $first = 1;
while (<IN>) {
#     ** Solucion no dominada 4**
# Tipo: 0
# Nodos: (1,object)(2,triangle)(3,object)(4,object)
# Ejes: (1,2,2)(1,3,1)(3,4,1)
# 
# 1 3 4 5 7
# 0.375   0.233333
    if (/\*\* Solucion no dominada (.*)\*\*/) {
        if ($first eq 0) {
            # Cargo datos
            $pareto_sop1{$id} = $soporte;
            $pareto_cos1{$id} = $costos;
        }
        else {
            $first = 0;
        }
        $id = $i;
	$i++;
    }
    elsif (/^[0-1].*\t/) {
        my $f = $_;
        chomp($f);
        $costos = $f;
    }
    elsif (/^[1-9]/) {
        my $f = $_;
        chomp($f);
        $soporte = $f;
    }
}
close(IN);
# Cargo datos
$pareto_sop1{$id} = $soporte;
$pareto_cos1{$id} = $costos;

# foreach my $i (keys %pareto_cos1) {
#     print "NO: $pareto_cos1{$i}\n";
# }
# print "\n";

# Cargo el pareto 2
$id = 0;
my %pareto_sop2 = ();
my %pareto_cos2 = ();
open(IN, $in2);
$i = 1;
$first = 1;
while (<IN>) {
#     ** Solucion no dominada 4**
# Tipo: 0
# Nodos: (1,object)(2,triangle)(3,object)(4,object)
# Ejes: (1,2,2)(1,3,1)(3,4,1)
# 
# 1 3 4 5 7
# 0.375   0.233333
    if (/\*\* Solucion no dominada (.*)\*\*/) {
        if ($first eq 0) {
            # Cargo datos
            $pareto_sop2{$id} = $soporte;
            $pareto_cos2{$id} = $costos;
        }
        else {
            $first = 0;
        }
        $id = $i;
	$i++;
    }
    elsif (/^[0-1].*\t/) {
        my $f = $_;
        chomp($f);
        $costos = $f;
    }
    elsif (/^[1-9]/) {
        my $f = $_;
        chomp($f);
        $soporte = $f;
    }
}
close(IN);
# Cargo datos
$pareto_sop2{$id} = $soporte;
$pareto_cos2{$id} = $costos;

# foreach my $i (keys %pareto_cos2) {
#     print "NO: $pareto_cos2{$i}\n";
# }
# print "\n";

# Calculo e-dominancia sin jaccard
my $epsilon = 0;
foreach my $i (sort keys %pareto_cos1) {
    my @data1 = split(/\t/, $pareto_cos1{$i});
    my @set2 = sort keys %pareto_cos2;

    my $sub_epsilon = 9;
    foreach my $j (sort keys %pareto_cos2) {
      my @data2 = split(/\t/, $pareto_cos2{$j});
      my $dist1 = $data2[0] - $data1[0];
      if ($dist1 < 0) {
	$dist1 = 0;
      }
      my $dist2 = $data2[1] - $data1[1];
      if ($dist2 < 0) {
	$dist2 = 0;
      }
#       print max($dist1,$dist2), "\n";
      if (max($dist1,$dist2) < $sub_epsilon) {
	$sub_epsilon = max($dist1,$dist2);
      }
    }

    if ($sub_epsilon > $epsilon) {
	$epsilon = $sub_epsilon;
    }
}

print "I_e=$epsilon\n";