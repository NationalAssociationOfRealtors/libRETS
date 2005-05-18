#!/usr/bin/perl -w

my $SRC = shift @ARGV;
my $OBJ = shift @ARGV;

while(<>)
{
    if (m:(\S+)\.o\:\s+${SRC}/(\S+)\.(cc?)(.*):)
    {
        print "${OBJ}/${2}.o ${OBJ}/${2}.d: ".
            "${SRC}/${2}.${3}${4}\n";
    }
    else
    {
        print;
    }
}
