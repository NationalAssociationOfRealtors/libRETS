#!/usr/bin/perl -w

# SRC is no logger used, but left for compatiblity
my $SRC = shift @ARGV;
my $OBJ = shift @ARGV;

while(<>)
{
    s:(\S+)\.o:${OBJ}/${1}.o ${OBJ}/${1}.d:;
    print;
}
