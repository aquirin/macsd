#!/usr/bin/perl

use warnings;
use strict;

my (@files) = @ARGV;

foreach my $file (@files) {
  my $nod_ed = 0;
  open(IN, $file);
  while (<IN>) {
  #   *vertices 235
    if (/\*vertices.*/) {
      print "XP\n";
    }
    elsif (/\**edges/) {
      $nod_ed = 1;
    }
    elsif (($nod_ed == 0) and (/^([0-9]*) (\".*\").*/)) {
  #   1 " Multidisciplinary " ellipse x_fact 50 y_fact 50 fos 10 bw 0.0 ic LSkyBlue
      print "v $1 $2\n";
    }
    elsif (($nod_ed == 1) and (/^([0-9]*) ([0-9]*) .*/)) {
  # 1 30 32471.000000
      print "d $1 $2 \"\"\n";
    }
  }
  close(IN);
  print "\n";
}
