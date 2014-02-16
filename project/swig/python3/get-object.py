#!/usr/bin/env python3
import sys

import librets

try:
    session = librets.RetsSession("http://www.dis.com:6103/rets/login")
    if not session.Login("Joe", "Schmoe"):
        print("Invalid login")
        sys.exit(1)

    request = librets.GetObjectRequest("Property", "Photo")
    request.AddAllObjects("LN000001")

    content_type_suffixes = {'image/jpeg': 'jpg', 'image/gif': 'gif', 'text/xml': 'xml'}
    response = session.GetObject(request)
    object_descriptor = response.NextObject()
    while object_descriptor is not None:
        object_key = object_descriptor.GetObjectKey()
        object_id = object_descriptor.GetObjectId()
        content_type = object_descriptor.GetContentType()
        description = object_descriptor.GetDescription()
        print('{} object #{}'.format(object_key, object_id))

        suffix = content_type_suffixes[content_type]
        output_file_name = '{}-{}.{}'.format(object_key, object_id, suffix)
        with open(output_file_name, 'wb') as file:
            file.write(object_descriptor.GetData())

        object_descriptor = response.NextObject()

    session.Logout()
except librets.RetsException as e:
    print("Caught: " + e.GetMessage())
