#!/usr/bin/env python

import hid
import time


for d in hid.enumerate(0, 0):
    if d['vendor_id'] != 0x1234 :
        continue

    keys = d.keys()
    keys.sort()
    for key in keys:
        print "%s : %s" % (key, d[key])
    print ""

