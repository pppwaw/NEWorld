#!/usr/bin/python
import os  
import os.path  
import sys

AUTO_ADD = False;

if len(sys.argv) >= 2 and sys.argv[1] == "--auto-add":
    AUTO_ADD = True

# check newline at the end of file
for parent,dirnames,filenames in os.walk("./"):
    for rawfilename in filenames:
        suffix = os.path.splitext(rawfilename)[1][1:]
        if suffix != "cpp" and suffix != "h" and suffix != "hpp" and suffix != "txt" and suffix != "md": continue
        filename = os.path.join(parent, rawfilename)
        f = open(filename, AUTO_ADD and 'r' or 'a+')
        f.seek(-1,2)
        if f.read(1) != "\n":
            print("No newline at the end of file: " + filename)
            if AUTO_ADD:f.write('\n')
        f.close()

print("Finished!")

