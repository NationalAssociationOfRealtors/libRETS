#!/usr/bin/perl

use lib "blib/lib", "blib/arch";
use strict;
use librets;
use Data::Dumper;

my %extentions =
(
 'image/jpeg' => "jpg",
 'image/gif' => "gif"
);
 
my $session = new librets::RetsSession(
    "http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid login\n";
    exit 1;
}

my $request = new librets::GetObjectRequest("Property", "Photo");

$request->AddAllObjects("LN000001");

my $response = $session->GetObject($request);

print Dumper($response);

my $objectDescriptor = $response->NextObject();
    
while ($objectDescriptor)
{
    my $objectKey = $objectDescriptor->GetObjectKey();
    my $objectId = $objectDescriptor->GetObjectId();
    my $contentType = $objectDescriptor->GetContentType();
    my $description = $objectDescriptor->GetDescription();

    print $objectKey . " object #" . $objectId;
    if ($description ne "")
    {
        print ", description: " . $description;
    }
    print "\n";

    print Dumper($contentType);
    my $ext = $extentions{$contentType};
    my $outputFilename = $objectKey . "-" . $objectId . "." . $ext;

    open(OUT, ">", $outputFilename) || die ("Couldn't open output file");
    binmode(OUT);

    my $resultdata = $objectDescriptor->GetDataAsString();
    print Dumper($resultdata);
    print length($resultdata) . "\n";
    syswrite(OUT, $resultdata);

    close(OUT);

    $objectDescriptor = $response->NextObject();
}

$session->Logout();
