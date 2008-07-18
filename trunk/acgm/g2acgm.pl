#!/usr/bin/perl

use strict;
use warnings;

my ($in) = @ARGV;

my $i = 0;
my $cant = 0;
my $text;
my $text1 = "";
my $text2 = "";
my $text3 = "";
my @nodes = ();
my %map = ();
open(IN,$in);
while(<IN>) {
	if (/XP/) {
		if ($i > 0) {
		        @nodes = sort {($a =~ /node\([^,]*,([^,]*),/)[0] <=> ($b =~ /node\([^,]*,([^,]*),/)[0]} @nodes;
		        $text = join('',@nodes);
		        $cant = $#nodes + 1;
			print $text1, $cant, $text2, $text, $text3, "end\n";
			$text2 = "";
			$text3 = "";
			$cant = 0;
			@nodes = ();
			%map = ();
		}
		$i++;
		$text1 = "size($i,";
                $text2 = ");\n";
		$text2 .= "name($i,$in$i);\n";
	}
	if (/v ([0-9]+) (.*)/) {
	        $cant++;
	        my $x = $2;
	        $map{$1} = $cant;
	        $x =~ s/\(//g;
	        $x =~ s/\)//g;
	        $x =~ s/"//g;
	        $x = substr($x,0,8);
		$text = "node($i,$cant,$x";
                $text .= ");\n";
                push(@nodes, $text);
	}
	if (/[de] ([0-9]+) ([0-9]+) (.*)/) {
		$text3 .= "link($i,$map{$1},$map{$2},$3);\n";
	}
}
print $text1, $cant, $text2, $text3, "end\n";
close(IN);