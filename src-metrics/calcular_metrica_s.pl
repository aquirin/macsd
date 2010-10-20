#!/usr/bin/perl

use strict;
use warnings;

sub dominancia {
    my ($refA, $refB) = @_;
    
    my @A = split(/\t/, $refA);
    my @B = split(/\t/, $refB);
#     print "@A\n@B\n";
    my $res = 0;
    
    if (($A[0] >= $B[0]) and ($A[1] >= $B[1]) and (($A[0] > $B[0]) or ($A[1] > $B[1]))) {
        $res = 1;
    }
    
    return $res;
}

my ($in) = @ARGV;

# Cargo el pareto
my $id = 0;
my @data = ();
my $soporte;
my $costos;
my %pareto_sop = ();
my %pareto_cos = ();
open(IN, $in);
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
            $pareto_sop{$id} = $soporte;
            $pareto_cos{$id} = $costos;
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
$pareto_sop{$id} = $soporte;
$pareto_cos{$id} = $costos;

# foreach my $i (keys %pareto_cos) {
#     print "NO: $pareto_cos{$i}\n";
# }
# print "\n";

# Calculo dominancia sin jaccard
my %pareto_sj = ();
my $domina;
my @set = sort keys %pareto_cos;
$i = 0;
while ($i <= $#set) {
    my $j = 0;
    $domina = 0;
    while (($j <= $#set) and (!$domina)) {
#         print "$i $j\n";
        $domina = dominancia($pareto_cos{$set[$j]},$pareto_cos{$set[$i]});
        $j++;
    }
    if (!$domina) {
#         print "DOM\n";
        $pareto_sj{$pareto_cos{$set[$i]}} = $pareto_cos{$set[$i]};
    }
    $i++;
}

# Ordeno por eje de las x costo[0] -> automaticamente se ordena en costo[1]
# foreach my $i (keys %pareto_sj) {
#     print "NO: $i $pareto_sj{$i}\n";
# }
# foreach my $i (sort keys %pareto_sj) {
#     print "SI: $i\n";
# }

# Calculo hypervolumen (estoy maximizando -> lo comparo con el (0,0)
my $sum = 0.;
my $ant = 0.;
foreach my $i (sort keys %pareto_sj) {
    my @data = split(/\t/, $pareto_sj{$i});
    my $area = ($data[0] - $ant) * $data[1];
    $ant = $data[0];
    
    $sum = $sum + $area;
}

print "S=$sum\n";
# 
