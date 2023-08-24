#!/bin/env python
##
## CMO_driver Python Scripts
##
import string

def package_files(file_list,ofilename):
    #print file_list
    #print ofilename
    ofstr = open(ofilename,"w")
    for file in file_list:
      tfile = string.split(file,"/")
      if file[len(file)-1] == "/":
         tfile = tfile[len(tfile)-2]
      else:
         tfile = tfile[len(tfile)-1]
      ofstr.write("# CMO_DRIVER_BEGIN - " + tfile + "\n")
      ifstr = open(file,"r")
      ofstr.write(ifstr.readline())
      ifstr.close()
      ofstr.write("# CMO_DRIVER_END   - " + tfile + "\n")
    ofstr.close()
     
def unpackage_files(ifilename):
    #print ifilename
    ifstr = open(ifilename,"r")
    state = 0
    for line in ifstr.readlines(): 
      tokens = string.split(line)
      if state == 1:
         if tokens[0] == "#" and tokens[1] == "CMO_DRIVER_END":
            ofstr.close()
            state = 0
         else:
            ofstr.write(line)
      if state == 0 and tokens[0] == "#" and tokens[1] == "CMO_DRIVER_BEGIN":
         ofstr = open(tokens[3],"w")
         state = 1
