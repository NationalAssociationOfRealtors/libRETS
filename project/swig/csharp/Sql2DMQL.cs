using System;
using librets;

public class Sql2DMQL
{
  static void Main(string[] args)
  {
    SqlMetadata md =  new SimpleSqlMetadata();
    SqlToDmqlCompiler compiler = new SqlToDmqlCompiler(md);
    compiler.sqlToDmql("select * from data:Property:RES where ListPrice >= 30000;");
    
    DmqlQueryPtr query = compiler.GetDmqlQuery();
    Console.WriteLine ("Resource: " + query.GetResource());
    Console.WriteLine("Class: " + query.GetClass());
    Console.WriteLine ("Query: " + query.GetCriterion().ToDmqlString());
   
  }
}
