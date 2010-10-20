#!/usr/bin/perl

use strict;
use warnings;

my (@files) = @ARGV;

my $actual = shift(@files);

my $text = join(' ', @files);

`cat $text > all`;

my $num = `perl calcular_metrica_s.pl $actual`;

$num =~ /S=(.*)/;
$num = $1;

my $den = `perl calcular_metrica_s.pl all`;

$den =~ /S=(.*)/;
$den = $1;

print $num, ' ' , $den, ' ', $num / $den, "\n";