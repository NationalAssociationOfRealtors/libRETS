var librets = require("./build/Release/librets.node");

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    console.log("Logging into " + session.GetLoginUrl());

    if (!session.Login("Joe", "Schmoe"))
    {
	console.log("Login failed");
	process.exit(-1);
    }

    var loginResponse = session.GetLoginResponse();
    var urls = session.GetCapabilityUrls();

    console.log("Member name: " + loginResponse.GetMemeberName());
    console.log("Search URL: " + urls.GetSearchUrl());
    console.log("Action:")
    console.log(session.GetAction());

    var logout = session.Logout();

    console.log("Billing info: " + logout.GetBillingInfo());
    console.log("Connect time: " + logout.GetConnectTime());
    console.log("Message: " + logout.GetLogoutMessage());
}
catch(e)
{
    console.log("Exception: " + e);
}
