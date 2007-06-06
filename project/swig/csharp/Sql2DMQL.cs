using System;
using librets;

public class Sql2DMQL
{
  static void Main(string[] args)
  {
    SqlMetadata md = new SimpleSqlMetadata();
    SqlToDmqlCompiler compiler = new SqlToDmqlCompiler(md);
  }
}
