#!/usr/bin/perl

use strict;
use warnings;

sub dominancia {
    my ($refA, $refB) = @_;
    
    my @A = @$refA;
    my @B = @$refB;
#     print "@A\n@B\n";
    my $res = 0;
    
    if (($A[0] > $B[0]) and ($A[1] > $B[1]) and (($A[0] >= $B[0]) or ($A[1] >= $B[1]))) {
        $res = 1;
    }
    
    return $res;
}

my ($in) = @ARGV;

# Cargo el pareto
my $id = 0;
my @data = ();
my @soporte = ();
my @costos = ();
my %pareto_sop = ();
my %pareto_cos = ();
open(IN, $in);
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
            my @q = @soporte;
            my @r = @costos;
            $pareto_sop{$id} = \@q;
            $pareto_cos{$id} = \@r;
        }
        else {
            $first = 0;
        }
        $id = $1;
    }
    elsif (/^[1-9]/) {
        @soporte = split(/ /, $_);
    }
    elsif (/^[0-1]/) {
        @costos = split(/\t/, $_);
    }
}
close(IN);
# Cargo datos
my @q = @soporte;
my @r = @costos;
$pareto_sop{$id} = \@q;
$pareto_cos{$id} = \@r;

# foreach my $i (keys %pareto_cos) {
#     print "NO: $i $pareto_cos{$i}\n";
# }
# print "\n";

# Calculo dominancia sin jaccard
my %pareto_sj = ();
my $domina;
my @set = sort keys %pareto_cos;
my $i = 0;
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
        my $ref = $pareto_cos{$set[$i]};
        my @data = @$ref;
        my $text = join(',', @data);
        $pareto_sj{$text} = $pareto_cos{$set[$i]};
    }
    $i++;
}

# Ordeno por eje de las x costo[0] -> automaticamente se ordena en costo[1]
# foreach my $i (keys %pareto_sj) {
#     print "NO: $i $pareto_sj{$i}\n";
# }
# foreach my $i (sort keys %pareto_sj) {
#     print "SI: $i $pareto_sj{$i}\n";
# }

# Calculo hypervolumen (estoy maximizando -> lo comparo con el (0,0)
my $sum = 0.;
my $ant = 0.;
foreach my $i (sort keys %pareto_sj) {
    my $ref = $pareto_sj{$i};
    my @data = @$ref;
    my $area = ($data[0] - $ant) * $data[1];
    $ant = $data[0];
    
    $sum = $sum + $area;
}

print "S=$sum\n";
# 
