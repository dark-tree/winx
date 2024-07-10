#!/bin/env python3

import re

selected = None
exported = {}

def power(array):
	return [(a, b) for a in array for b in array if a != b]

def missing(a, b):
	return [i for i in a if not i in b]

with open("winx.h") as header:
	for line in header.readlines():
		line = line.strip()

		marker = re.search("^\/\/\s*BEGIN ([A-Z0-9]+) KEYS$", line);

		if marker:
			selected = marker.group(1)
			print(f"Scanning group '{selected}'...")
			exported[selected] = []
			continue

		define = re.search("^#\s*define WX[KB]_([A-Z_0-9]+)\s+0x([0-9a-fA-F]+)\s\/\/\s([a-zA-Z_0-9]+)$", line)

		if define and selected:
			defined = define.group(1)
			exported[selected].append(defined)
			continue

errors = []

for a, b in power(exported.keys()):
	for c in missing(exported[a], exported[b]):
		errors.append(f" * Missing '{c}' from '{b}' (pressent in '{a}')")

count = len(errors)

if count:
	print(f"\n{count} errors found!")

	for error in errors:
		print(error)
else:
	print(f"\nNo errors found!")

exit(count)
