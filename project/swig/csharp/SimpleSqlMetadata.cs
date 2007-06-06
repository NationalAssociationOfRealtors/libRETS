
using librets;

public class SimpleSqlMetadata : SqlMetadata
{
  public override bool IsLookupColumn(string tableName, string columnName)
  {
    return false;
  }
}
