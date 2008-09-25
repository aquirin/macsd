#!/usr/bin/perl

use strict;
use warnings;

sub dominancia {
    my ($refA, $refB) = @_;
    
    my @A = split(/\t/, $refA);
    my @B = split(/\t/, $refB);
#     print "A @A\nB @B\n";
    my $res = 0;
    
    if (($A[0] >= $B[0]) and ($A[1] >= $B[1]) and (($A[0] > $B[0]) or ($A[1] > $B[1]))) {
        $res = 1;
    }
    
    return $res;
}

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
open(INA, $a);
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
open(INB, $b);
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
            
my $domina = 0;
my $jaccard = 0;
my $sum = 0;

# print "PARETO A\n";
# foreach my $i (sort keys %paretoA_cos) {
#     print "$i $paretoA_cos{$i}\n";
# }
# 
# print "PARETO B\n";
# foreach my $i (sort keys %paretoB_cos) {
#     print "$i $paretoB_cos{$i}\n";
# }

# Calculo dominancia
foreach my $i (sort keys %paretoA_cos) {
    my $j = 0;
    $domina = 0;
    my @set = sort keys %paretoB_cos;
#     print @set, "\n";
    while (($j <= $#set) and (!$domina)) {
#         print "X: $i $set[$j]\n";
        $domina = &dominancia($paretoB_cos{$set[$j]},$paretoA_cos{$i});
        if ($domina) {
#            print "!$paretoB_sop{$set[$j]},$paretoA_sop{$i}!\n"; 
           if (jaccard($paretoB_sop{$set[$j]},$paretoA_sop{$i}) < 0.5) {
                $domina = 0;
            }
            else
             {
                        print "!$paretoB_sop{$set[$j]},$paretoA_sop{$i}!\n";
                        print "!$paretoB_cos{$set[$j]},$paretoA_cos{$i}!\n\n";
            }
        }
        $j++;
    }
    if ($domina) {
#         print "DOM\n";
        $sum++;
    }
}

my @set = sort keys %paretoA_cos;
print "C(X'',X')=", ($sum / ($#set + 1)), "\n";

$sum = 0;

# Calculo dominancia
foreach my $i (sort keys %paretoB_cos) {
    my $j = 0;
    $domina = 0;
    my @set = sort keys %paretoA_cos;
    while (($j <= $#set) and (!$domina)) {
#         print "$i $set[$j]\n";
        $domina = &dominancia($paretoA_cos{$set[$j]},$paretoB_cos{$i});
        if ($domina) {
#             print "!$paretoB_sop{$i},$paretoA_sop{$set[$j]}!\n";
            if (jaccard($paretoA_sop{$set[$j]},$paretoB_sop{$i}) < 0.5) {
                $domina = 0;
            }
            else {
                        print "!$paretoB_sop{$i},$paretoA_sop{$set[$j]}!\n";
                        print "!$paretoB_cos{$i},$paretoA_cos{$set[$j]}!\n\n";
            }
        }
        $j++;
    }
    if ($domina) {
#         print "DOM\n";
        $sum++;
    }
}

@set = sort keys %paretoB_cos;
print "C(X',X'')=", ($sum / ($#set + 1)), "\n";

