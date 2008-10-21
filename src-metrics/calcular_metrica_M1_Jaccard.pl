#!/usr/bin/perl

use strict;
use warnings;

sub jaccard {
    my ($refA, $refB) = @_;
    
    my @A = split(/ /, $refA);
    my @B = split(/ /, $refB);
#     print "\tA @A\n\tB @B\n";
    # Busco interseccion
    my %hashB = ();
    foreach my $i (@B) {
        $hashB{$i} = 1;
    }
    my $inter = 0;
    foreach my $i (@A) {
        if (exists $hashB{$i}) {
            $inter++;
        }
    }
    
    return ($inter / ($#A + 1 + $#B + 1 - $inter));
}

my ($a, $b) = @ARGV;

# Cargo ambos paretos
my $id = 0;
my @data = ();
my $soporte;
my $costos;
my %paretoA_sop = ();
my %paretoA_cos = ();
open(INA, $a) or die "1";
my $first = 1;
while (<INA>) {
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
            $paretoA_sop{$id} = $soporte;
            $paretoA_cos{$id} = $costos;
        }
        else {
            $first = 0;
        }
        $id = $1;
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

close(INA);
# Cargo datos
$paretoA_sop{$id} = $soporte;
$paretoA_cos{$id} = $costos;
            
$id = 0;
@data = ();
my %paretoB_sop = ();
my %paretoB_cos = ();
$soporte = "";
$costos = "";
open(INB, $b) or die "2";
$first = 1;
while (<INB>) {
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
#             print "$soporte $costos\n";
            $paretoB_sop{$id} = $soporte;
            $paretoB_cos{$id} = $costos;
        }
        else {
            $first = 0;
        }
        $id = $1;
    }
    elsif (/^[0-1].*\t/) {
        my $f = $_;
        chomp($f);
        $costos = $f;
#         print "$id $_";
    }
    elsif (/^[1-9]/) {
        my $f = $_;
        chomp($f);
        $soporte = $f;
    }
}
close(INB);
# Cargo datos
$paretoB_sop{$id} = $soporte;
$paretoB_cos{$id} = $costos;
            
# print "PARETO A\n";
# foreach my $i (sort keys %paretoA_cos) {
#     print "$i $paretoA_cos{$i}\n";
# }
# 
# print "PARETO B\n";
# foreach my $i (sort keys %paretoB_cos) {
#     print "$i $paretoB_cos{$i}\n";
# }

# Calculo distancia entre valores (M1 en objetivos)
my $sum = 0;
foreach my $i (sort keys %paretoA_cos) {
    my $j = 0;
    my @set = sort keys %paretoB_cos;
    my $min = 999999.0;
    foreach my $j (@set) {
        if (jaccard($paretoB_sop{$j},$paretoA_sop{$i}) >= 0.5) {                
            my @info1 = split(/\t/, $paretoA_cos{$i});
            my @info2 = split(/\t/, $paretoB_cos{$j});
            my $x2 = ($info1[0] - $info2[0])*($info1[0] - $info2[0]);
            my $y2 = ($info1[1] - $info2[1])*($info1[1] - $info2[1]);
            if (sqrt($x2+$y2) <= $min) {
                $min = sqrt($x2+$y2);
            }
        }
    }
#     print $min, "\n";
    $sum += $min;
}

my @set = sort keys %paretoA_cos;
print "M1 = " . $sum / ($#set + 1) . "\n";
