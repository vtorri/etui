#!/usr/bin/env python3

import sys

path = sys.argv[1]
with open(path + "version.h", "rt") as ver :
  content = ver.read()
v = content.find("FZ_VERSION ")
minor = int(content[v + 14:v+16])
print(str(minor))

