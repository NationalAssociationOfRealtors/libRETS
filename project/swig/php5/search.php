<?php
include_once('librets.php');

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid Login\n";
    exit(1);
}

print "Action: " . $session->GetAction() . "\n";
$version = "1.0";
if ($session->GetDetectedRetsVersion() == RETS_1_5)
{
    $version = "1.5";
}
print "RETS Version: " . $version . "\n";

$request = $session->CreateSearchRequest("Property", "ResidentialProperty",
                                        "(ListPrice=300000-)");
$request->SetSelect("ListingID,ListPrice,Beds,City");
$request->SetLimit(SearchRequest_LIMIT_DEFAULT);
$request->SetOffset(SearchRequest_OFFSET_NONE);
$request->SetCountType(SearchRequest_RECORD_COUNT_AND_RESULTS);
$request->SetStandardNames(true);
$results = $session->Search($request);

print "Record Count: " . $results->GetCount() . "\n\n";

$columns = $results->GetColumns();

while ($results->HasNext())
{
    for ($i = 0; $i < $columns->size(); $i++)
    {
        print $columns->get($i) . ": " . $results->GetString($i) . "\n";
    }
    print "\n";
}

$logout = $session->Logout();

print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
