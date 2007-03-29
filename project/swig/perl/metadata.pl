#!/usr/bin/env perl

use lib "blib/lib", "blib/arch";
use strict;
use librets;
use Data::Dumper;

# Here we define some helper functions
sub dumpAllTables
{
    my $metadata = shift;
    my $class = shift;

    my $tables = $metadata->GetAllTables($class);
    foreach my $table (@$tables)
    {
        print "Table name: " . $table->GetSystemName() . " [" .
            $table->GetStandardName() . "]\n";
        print "\tType: " . $table->GetDataType() . "\n";
        print "\tUnique: " . $table->IsUnique() . "\n";
        print "\tMax Length: " . $table->GetMaximumLength() . "\n";
    }
}

sub dumpAllClasses
{
    my $metadata = shift;
    my $resource = shift;

    my $classes = $metadata->GetAllClasses($resource->GetResourceID());
    foreach my $class (@$classes)
    {
        print "Class name: " . $class->GetClassName() . " [" .
            $class->GetStandardName() . "]\n";
        dumpAllTables($metadata, $class);
    }
}

sub dumpAllLookupTypes
{
    my $metadata = shift;
    my $lookup = shift;

    my $lookupTypes = $metadata->GetAllLookupTypes($lookup);
    foreach my $lt (@$lookupTypes)
    {
        print "Lookup value: " . $lt->GetValue() . " (" .
            $lt->GetShortValue() . ", " . $lt->GetLongValue() . ")\n";
    }
}

sub dumpAllLookups
{
    my $metadata = shift;
    my $resource = shift;

    my $lookups = $metadata->GetAllLookups($resource->GetResourceID());
    foreach my $lookup (@$lookups)
    {
        print "Lookup name: " . $lookup->GetLookupName() . " (" .
            $lookup->GetVisibleName() . ")\n";
        dumpAllLookupTypes($metadata, $lookup);
        print "\n";
    }
}
    

my $session = new librets::RetsSession(
    "http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid login\n";
    exit 1;
}

my $metadata = $session->GetMetadata();
my $system = $metadata->GetSystem();
print "System ID: " . $system->GetSystemID() . "\n";
print "Desription: " . $system->GetSystemDescription() . "\n";
print "Comment : " . $system->GetComments() . "\n\n";

my $resources = $metadata->GetAllResources();
foreach my $resource (@$resources)
{
    print "Resource name: " . $resource->GetResourceID() . " [" .
        $resource->GetStandardName() . "]\n";
    dumpAllClasses($metadata, $resource);
    dumpAllLookups($metadata, $resource);
    print "\n";
    
}

$session->Logout();
