<?php
include_once('librets.php');

function dump_system($metadata)
{
  $system = $metadata->GetSystem();
  print "System ID: " . $system->GetSystemID() . "\n";
  print "Description: " . $system->GetSystemDescription() . "\n";
  print "Comments: " . $system->GetComments() . "\n";

}

function dump_foreign_keys($metadata)
{
  $foreign_keys = $metadata->GetAllForeignKeys();

  for ($i = 0; $i < $foreign_keys->size(); $i++)
  {
    $foreign_key = $foreign_keys->get($i);

    print "Foreign Key ID: " . $foreign_key->GetForeignKeyID() . "\n";
    print "  Parent Resource: " . $foreign_key->GetParentResourceID();
    print ", Class: " . $foreign_key->GetParentClassID();
    print ", Name: " . $foreign_key->GetParentSystemName() . "\n";
    print "  Child Resource: " . $foreign_key->GetChildResourceID();
    print ", Class: " . $foreign_key->GetChildClassID();
    print ", Name: " . $foreign_key->GetChildSystemName() . "\n";
  }
}

function dump_all_tables($metadata, $class)
{
  $tables = $metadata->GetAllTables($class);
  for ($i = 0; $i < $tables->size(); $i++)
  {
    $table = $tables->get($i);
    print "    Table Name: " . $table->GetSystemName() . " [" .
	  $table->GetStandardName() . "], DataType: " . enum_datatype($table->GetDataType()) ."\n";
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

function enum_datatype($datatype)
{
    switch ($datatype)
    {
      case MetadataTable::BOOLEAN:      return "Boolean";
      case MetadataTable::CHARACTER:    return "Character";
      case MetadataTable::DATE:         return "Date";
      case MetadataTable::DATE_TIME:    return "DateTime";
      case MetadataTable::TIME:         return "Time";
      case MetadataTable::TINY:         return "Tiny";
      case MetadataTable::SMALL:        return "Small";
      case MetadataTable::INT:          return "Int";
      case MetadataTable::LONG:         return "Long";
      case MetadataTable::DECIMAL:      return "Decimal";
      default: return "Unknown";
    }
}

try
{
    $session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

    if (!$session->Login("Joe", "Schmoe"))
    {
        print "Invalid Login\n";
        exit(1);
    }

    $session->SetIncrementalMetadata(false);
    $session->SetRetsVersion(RETS_1_5);

    $filename = "rawmetadata.xml";
    $fd = fopen($filename, "w");
    
    // Do the GetMetadata and get all the raw XML.
    $results = $session->GetMetadataAsString();

    fwrite($fd, $results);

    fclose($fd);

    $logout = $session->Logout();

    // Reread the file and inject the content into the metadata for parsing.
    $fd = fopen($filename, "r");
    $rawxml = fread($fd, filesize($filename));
    fclose($fd);

    $metadata = RetsMetadata::CreateMetadataFromString($rawxml);

    dump_system($metadata);

    dump_foreign_keys($metadata);

    dump_all_resources($metadata);
}
catch (Execption $e)
{
    print "Exception: " . $e . "\n";
}

?>

