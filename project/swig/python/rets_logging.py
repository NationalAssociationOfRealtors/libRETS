#!/usr/bin/env python
from __future__ import print_function
from argparse import ArgumentParser
import sys

import librets

try:
    p = ArgumentParser()
    p.add_argument('log_file')
    args = p.parse_args()

    session = librets.RetsSession("http://www.dis.com:6103/rets/login")
    session.SetHttpLogName(args.log_file)

    if not session.Login("Joe", "Schmoe"):
        sys.exit("Invalid login")

    logout = session.Logout()
    session.Cleanup()

except librets.RetsException as e:
    print("Caught:", e.GetMessage())
