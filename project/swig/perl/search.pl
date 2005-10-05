#!/usr/bin/env perl

use lib "blib/lib", "blib/arch";
use strict;
use librets;

my $rets = new librets::RetsSession(
    "http://demo.crt.realtors.org:6103/rets/login");

if (!$rets->Login("Joe", "Schmoe"))
{
    print "Invalid login\n";
    exit 1;
}

print "Action: " . $rets->GetAction() . "\n";

my $request = $rets->CreateSearchRequest("Property", "ResidentialProperty",
					 "(ListPrice=300000-)");

my $results = $rets->Search($request);
my @columns = qw{ListingID ListPrice Beds City};
while ($results->HasNext())
{
    foreach my $column (@columns)
    {
	print $column . ": " . $results->GetString($column) . "\n";
    }
    print "\n";
}

my $logout =  $rets->Logout();
print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";
