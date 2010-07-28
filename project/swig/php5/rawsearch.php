<?php
include_once('librets.php');

try
{
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

    $request = $session->CreateSearchRequest("Property", "RES",
                                            "(ListPrice=300000-)");
    $request->SetSelect("ListingID,ListPrice,Beds,City");
    $request->SetLimit(SearchRequest_LIMIT_DEFAULT);
    $request->SetOffset(SearchRequest_OFFSET_NONE);
    $request->SetCountType(SearchRequest_RECORD_COUNT_AND_RESULTS);
    $request->SetStandardNames(true);

    $filename = "rawsearch.xml";
    $fd = fopen($filename, "w");
    
    // Do the search and get all the raw XML.
    $results = $session->SearchAsString($request);

    fwrite($fd, $results);

    fclose($fd);

    // Reread the file and inject the content into the resultset for parsing.
    $fd = fopen($filename, "r");
    $rawxml = fread($fd, filesize($filename));
    fclose($fd);

    $results = new SearchResultSet();

    $results->SetDataAsString($rawxml);

    print "Record count: " . $results->GetCount() . "\n";

    $columns = $results->GetColumns();

    while ($results->HasNext())
    {
        for ($i = 0; $i < $columns->size(); $i++)
        {
            print $columns->get($i) . ": " . $results->GetString($i) . "\n";
        }
        print "\n";
    }
}
catch (Execption $e)
{
    print "Exception: " . $e . "\n";
}

$logout = $session->Logout();

print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
