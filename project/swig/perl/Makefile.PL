use ExtUtils::MakeMaker;
use Config;

if ($Config{'osname'} !~ /cygwin/)
{
    $cflags = `../../../librets-config-inplace --cflags`;
    $libs = `../../../librets-config-inplace --libs`;
}

$cc = $Config{'cc'};
$ld = $Config{'ld'};
if (($Config{'osname'} =~ /darwin/) &&  ($Config{'osname'} =~ /darwin/))
{
    $cc =~ s/gcc/cc/;
    $ld =~ s/gcc/cc/;
    $cc =~ s/cc/c\+\+/;
    $ld =~ s/cc/c\+\+/;
}
elsif ($Config{'osname'} =~ /linux/)
{
    $cc =~ s/gcc/g\+\+/;
    $ld =~ s/gcc/g\+\+/;
}
elsif ($Config{'osname'} =~ /freebsd/)
{
    $cc =~ s/cc/g\+\+/;
    $ld =~ s/cc/g\+\+/;
}

WriteMakefile(
    NAME            => 'librets',
    INC             => $cflags,
    CC              => $cc,
    LD              => $ld,
    PM              => {'librets.pm' => '$(INST_LIBDIR)/librets.pm'},
    dynamic_lib     => { OTHERLDFLAGS => $libs},
    OBJECT          => 'librets_wrap.o'
);
