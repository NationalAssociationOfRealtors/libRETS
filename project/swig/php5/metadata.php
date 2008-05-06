#!/usr/bin/php -q
<?
include_once('librets.php');

function dump_system($metadata)
{
  $system = $metadata->GetSystem();
  print "System ID: " . $system->GetSystemID() . "\n";
  print "Description: " . $system->GetSystemDescription() . "\n";
  print "Comments: " . $system->GetComments() . "\n";

}

function dump_all_tables($metadata, $class)
{
  $tables = $metadata->GetAllTables($class);
  for ($i = 0; $i < $tables->size(); $i++)
  {
    $table = $tables->get($i);
    print "    Table Name: " . $table->GetSystemName() . " [" .
	  $table->GetStandardName() . "]\n";
  }
}

function dump_all_classes($metadata, $resource)
{
  $resource_name = $resource->GetResourceID();
  $classes = $metadata->GetAllClasses($resource_name);
  for ($i = 0; $i < $classes->size(); $i++)
  {
    $class = $classes->get($i);
    print "\nResource Name: " . $resource_name . " [" .
          $resource->GetStandardName() . "]\n";
    print "  Class Name: " . $class->GetClassName() . " [" .
          $class->GetStandardName() . "]\n";
    dump_all_tables($metadata, $class);
  }
}

function dump_all_lookup_types($metadata, $lookup)
{
  $lookup_types = $metadata->GetAllLookupTypes($lookup);
  for ($i = 0; $i < $lookup_types->size(); $i++)
  {
    $lookup_type = $lookup_types->get($i);
    print "    Lookup Type: " . $lookup_type->GetValue() . " (" .
          $lookup_type->GetShortValue() . ", " .
	  $lookup_type->GetLongValue() . ")\n";
  }
}

function dump_all_lookups($metadata, $resource)
{
  $resource_name = $resource->GetResourceID();
  $lookups = $metadata->GetAllLookups($resource_name);
  for ($i = 0; $i < $lookups->size(); $i++)
  {
    $lookup = $lookups->get($i);
    print "\nResource Name: " . $resource_name . " [" .
          $resource->GetStandardName() . "]\n";
    print "  Lookup Name: " . $lookup->GetLookupName() . " (" .
          $lookup->GetVisibleName() . ")\n";
    dump_all_lookup_types($metadata, $lookup);
  }
}

function dump_all_resources($metadata)
{
  $resource = $metadata->GetAllResources();

  for ($i = 0; $i < $resource->size(); $i++)
  {
    $r = $resource->get($i);
    dump_all_classes($metadata, $r);
  }
  $resource = $metadata->GetAllResources();
  for ($i = 0; $i < $resource->size(); $i++)
  {
    $r = $resource->get($i);
    dump_all_lookups($metadata, $r);
  }
}

$session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

if (!$session->Login("Joe", "Schmoe"))
{
    print "Invalid Login\n";
    exit(1);
}

$session->SetIncrementalMetadata(false);
$session->SetRetsVersion(RETS_1_5);

$metadata = $session->GetMetaData();

dump_system($metadata);

dump_all_resources($metadata);

$logout = $session->Logout();

print "Billing info: " . $logout->GetBillingInfo() . "\n";
print "Logout message: " . $logout->GetLogoutMessage() . "\n";
print "Connect time: " . $logout->GetConnectTime() . "\n";

?>
