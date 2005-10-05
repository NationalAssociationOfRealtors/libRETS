use ExtUtils::MakeMaker;
use Config;

$cflags = `../../../librets-config-inplace --cflags`;
$libs = `../../../librets-config-inplace --libs`;
$cc = $Config{'cc'};
$ld = $Config{'ld'};
$cc =~ s/cc/c\+\+/;
$ld =~ s/cc/c\+\+/;

WriteMakefile(
    NAME            => 'librets',
    INC             => $cflags,
    CC              => $cc,
    LD              => $ld,
    PM              => {'librets.pm' => '$(INST_LIBDIR)/librets.pm'},
    dynamic_lib     => { OTHERLDFLAGS => $libs},
    OBJECT          => 'librets_wrap.o'
);
