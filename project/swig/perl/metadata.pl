#!/usr/bin/env perl

use lib "blib/lib", "blib/arch";
use strict;
use librets;
use Data::Dumper;

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
print "Comment : " . $system->GetComments() . "\n";

my $resources = $metadata->GetAllResources();
foreach my $resource (@$resources)
{
    print $resource->GetStandardName() . "\n"
}

$session->Logout();
