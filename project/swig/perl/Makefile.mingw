use ExtUtils::MakeMaker;
use Config;

$Config{'osname'} =~ /MSWin32/ || die "This only works for MinGW";

sub sanitize($_)
{
    @tokens = split(' ', shift);

    foreach (@tokens)
    {
        # Change drive letter format for Includes
        s/^-I\/([a-z])/-I$1:/;
        # Change drive letter format for libs
        s/^-L\/([a-z])/-L$1:/;
        # Change drive letter format for straight lines beginning /c/...
        s/^\/([a-z])/$1:/;
        # Change forward slashed to back slashes
        s/\//\\/g;
    }
    return join(" ", @tokens);
}

$cflags = `sh ../../../librets-config-inplace --cflags`;
$libs = `sh ../../../librets-config-inplace --libs`;

# Remove -fpic if it appears in cflags
$cflags =~ s/-fpic//i;

# Sanitize path names
$cflags = sanitize($cflags);
$libs = sanitize($libs);

$cc = $Config{'cc'};
$ld = $Config{'ld'};

WriteMakefile(
    NAME            => 'librets',
    INC             => $cflags,
    CC              => $cc,
    LD              => $ld,
    PM              => {'librets.pm' => '$(INST_LIBDIR)/librets.pm'},
    dynamic_lib     => { OTHERLDFLAGS => $libs},
    OBJECT          => 'librets_wrap.obj'
);
