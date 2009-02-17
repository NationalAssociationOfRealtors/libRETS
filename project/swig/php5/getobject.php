<?php
include_once('librets.php');

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid Login\n";
    exit(1);
}

$content_type_suffixes = array ("image/jpeg" => "jpg",
                                "image/gif"  => "gif",
                                "text/xml" => "xml");
                                

$request = new GetObjectRequest("Property", "Photo");
$request->AddAllObjects("LN000001");

$results = $session->GetObject($request);

while ($object_descriptor = $results->NextObject())
{
    $object_key = $object_descriptor->GetObjectKey();
    $object_id = $object_descriptor->GetObjectId();
    $content_type = $object_descriptor->GetContentType();
    $description = $object_descriptor->GetDescription();

    print $object_key . " object #" . $object_id;
    if (strlen($description) > 0)
        print ", description: " . $description;
    print "\n";

    $suffix = $content_type_suffixes[$content_type];

    $file_name = $object_key . "-" . $object_id . "." . $suffix;

    $file = fopen ($file_name, "wb") or die ("Unable to create file " . $file_name);
    fwrite ($file, $object_descriptor->GetDataAsString());
    fclose ($file);
}

$logout = $session->Logout();

print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
