#!/usr/bin/perl

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
my $version = "1.0";
$version = "1.5" if ($rets->GetDetectedRetsVersion() == $librets::RETS_1_5);
print "RETS Version: $version\n";

my $request = $rets->CreateSearchRequest("Property", "RES",
					 "(ListPrice=300000-)");
$request->SetSelect("ListingID,ListPrice,Beds,City");
$request->SetLimit($librets::SearchRequest::LIMIT_DEFAULT);
$request->SetOffset($librets::SearchRequest::OFFSET_NONE);
$request->SetStandardNames(1);
$request->SetCountType($librets::SearchRequest::RECORD_COUNT_AND_RESULTS);
my $results = $rets->Search($request);

print "Record count: " . $results->GetCount() . "\n\n";
while ($results->HasNext())
{
    foreach my $column (@$results->GetColumns())
    {
	    print $column . ": " . $results->GetString($column) . "\n";
    }
    print "\n";
}

my $logout =  $rets->Logout();
print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";
