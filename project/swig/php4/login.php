#!/usr/bin/php -q
<?
include_once('librets.php');

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid Login\n";
    exit(1);
}

$loginResponse = $session->GetLoginResponse();
$capurls = $session->GetCapabilityUrls();

print "Member name: " . $loginResponse->GetMemberName() . "\n";
print "Search URL: " . $capurls->GetSearchUrl() . "\n";


$logout = $session->Logout();

#print "Billing info: " . $logout->GetBillingInfo() . "\n";
#print "Logout message: " . $logout->GetLogoutMessage() . "\n";
#print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
