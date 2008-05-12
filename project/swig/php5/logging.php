#!/usr/bin/php -q
<?
include_once('librets.php');

class TestLogger extends RetsHttpLogger
{
  public $last_type;
  public $file_name;

  function TestLogger($filename)
  {
      print "in the constructor\n";
      $last_type = "";
      $file_name = $filename;
      parent::__construct($filename);
  }

  function logHttpData($type, $data)
  {
      if ($type = RECEIVED && $last_type != RECEIVED)
      {
	  $data .= "\n<<< RECEIVED\n";
      }
      else 
      if ($type == SENT && $last_type != SENT)
      {
	  $data .= "\n>>> SENT\n";
      }
      else
      if ($type == INFORMATIONAL)
      {
          $data .= "* ";
      }
      $last_type = $type;
  }
}

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

$logger = new TestLogger("foobar");
print gettype($logger) . "\n";
debug_zval_dump($logger);
print_r($logger);
print serialize($logger) . "\n";
$session->SetHttpLogger($logger);

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
