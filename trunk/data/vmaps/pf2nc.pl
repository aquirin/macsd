#!/usr/bin/perl

use warnings;
use strict;

my ($dir) = @ARGV;

my @list = ();
opendir(DIR, $dir);
foreach my $file (readdir(DIR)) {
  print "$file\n";

  if ($file =~ /(.*net.*)\.pf$/) {
    push(@list, "$dir/$1.g");
    `perl /home/rromero/Oscar/memetico/macsd/scripts/pf2g.pl $dir/$file > $dir/$1.g`;
  }
}
closedir(DIR);

foreach my $i (@list) {
  print "$i\n";

  my $text = "#!bash\n#\$ -N $i\n#\$ -q larga\n#\$ -o $i.txt\n#\$ -e $i\_err.txt\n#\$ -cwd\n\n../../../scripts/cycle_remover_undirected $i > $i\_nc\n";

  open(OUT, ">$i.qsub");
  print OUT $text;
  close(OUT);
}
