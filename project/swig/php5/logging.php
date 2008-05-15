<?php
include_once('librets.php');

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

$session->SetHttpLogger("logfile");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid Login\n";
    exit(1);
}

$loginResponse = $session->GetLoginResponse();
$capurls = $session->GetCapabilityUrls();

print "Member name: " . $loginResponse->GetMemberName() . "\n";
print "Search URL: " . $capurls->GetSearchUrl() . "\n";

print "Action: " . $session->GetAction() . "\n";
$version = "1.0";
if ($session->GetDetectedRetsVersion() == RETS_1_5)
{
    $version = "1.5";
}

print "RETS Version: " . $version . "\n";

$logout = $session->Logout();

print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
