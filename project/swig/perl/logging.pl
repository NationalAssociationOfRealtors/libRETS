#!/usr/bin/perl

use lib "blib/lib", "blib/arch";
use strict;
use librets;

my $rets = new librets::RetsSession(
    "http://demo.crt.realtors.org:6103/rets/login");

my $numArgs = $#ARGV + 1;

if ($numArgs)
{
    $rets->SetHttpLogName($ARGV[0]);
}

if (!$rets->Login("Joe", "Schmoe"))
{
    print "Invalid login\n";
    exit 1;
}


my $logout =  $rets->Logout();
print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";
