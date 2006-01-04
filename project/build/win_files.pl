#!/usr/bin/perl -w

use strict;
use File::DosGlob 'glob';
use IO::Dir;

my ($make_var, $match_pattern, $subst_match, $subst_replace) = @ARGV;

my @files;
foreach my $file (glob($match_pattern)) {
  $file =~ s/$subst_match/qq{"$subst_replace"}/ee if defined($subst_match);
  push (@files, $file);
}

my $separator = "";
my $last = "";
print "# Automatically generated file list.  Edits will be overwritten.\n";
print "${make_var} = \\\n";
foreach my $file (sort @files) {
  print $separator . "\t$file";
  $separator = " \\\n";
  $last = "\n";
}
print $last;
