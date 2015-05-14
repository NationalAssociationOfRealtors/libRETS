var librets = require("./build/Release/librets.node");
var util = require('util');
var fs = require('fs');

var content_type_suffixes = {
    "image/jpeg": "jpg",
    "image/gif": "gif",
    "text/xml": "xml"
};

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    if (!session.Login("Joe", "Schmoe"))
    {
	console.log("Invalid login");
	process.exit(-1);
    }

    var get_object_request = new librets.GetObjectRequest("Property", "Photo");
    get_object_request.AddAllObjects("LN000001");

    var get_object_response = session.GetObject(get_object_request);

    var object_descriptor = get_object_response.NextObject();

    while (object_descriptor)
    {
	var object_key =  object_descriptor.GetObjectKey();
	var object_id = object_descriptor.GetObjectId();
	var content_type = object_descriptor.GetContentType();
	var description = object_descriptor.GetDescription();
	console.log(util.format("%s object #%d", object_key, object_id));
	if (description)
	{
	    console.log(util.format(", description: %s"));
	}
	console.log("");

	var suffix = content_type_suffixes[content_type]
	var output_file_name = util.format("%s-%d.%s", object_key, object_id,
					   suffix);
	console.log(output_file_name);

	var odata = object_descriptor.GetData();
	console.log("odata length is " + odata.length);
	// console.log(util.inspect(odata));

	fd = fs.openSync(output_file_name, 'w');
	fs.writeSync(fd, new Buffer(odata), 0, odata.length);
	fs.closeSync(fd);

        object_descriptor = get_object_response.NextObject();
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
