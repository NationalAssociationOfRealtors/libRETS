var librets = require("./build/Release/librets.node");

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    if (!session.Login("Joe", "Schmoe"))
    {
	console.log("Invalid login");
	process.exit(-1);
    }

    console.log("Action: " + session.GetAction());
    console.log("Requested RETS version: " + session.RetsVersionToString(session.GetRetsVersion()));
    console.log("Actual RETS version: " + session.RetsVersionToString(session.GetDetectedRetsVersion()));

    var request = session.CreateSearchRequest("Property", "RES", "(ListPrice=300000-)");
    request.SetStandardNames(false);
    request.SetSelect("ListingID,ListPrice,Bedrooms,City");
    request.SetLimit(librets.SearchRequest.LIMIT_DEFAULT);
    request.SetOffset(librets.SearchRequest.OFFSET_NONE);
    request.SetCountType(librets.SearchRequest.RECORD_COUNT_AND_RESULTS);
    request.SetFormatType(librets.SearchRequest.COMPACT);
    results = session.Search(request);

    while (results.HasNext())
    {
	console.log("ListingID: " + results.GetString("ListingID"));
	console.log("ListPrice: " + results.GetString("ListPrice"));
	console.log(" Bedrooms: " + results.GetString("Bedrooms"));
	console.log("     City: " + results.GetString("City"));
	console.log();
    }
}
catch(e)
{
    console.log("Exception: " + e);
}
finally
{
    session.Logout();
}
