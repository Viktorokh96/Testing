#! /usr/bin/perl -W

open(INPUT, "hashed.txt") or die "Can't open the file!";
open(OUT, ">notcomp.txt") or die "Can't open the file!";

@lines = <INPUT>;

$dist{$_} = 1 foreach (@lines);

print OUT ( keys %dist );

