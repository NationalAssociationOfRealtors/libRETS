#!/usr/bin/perl -w

use strict;
use IO::Dir;

my $dir = new IO::Dir ".";
my @cpp_files;

while (defined($_ = $dir->read)) {
  push (@cpp_files, "$1.obj") if (/(.*).cpp$/);
}

my $separator = "";
my $last = "";
print "# Automatically generated file list.  Edits will be overwritten.\n";
print "X_OBJS = \\\n";
foreach my $file (sort @cpp_files) {
  print $separator . "\t\$\(DIROBJ\)\\$file";
  $separator = " \\\n";
  $last = "\n";
}
print $last;
