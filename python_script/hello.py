import subprocess
import os
import shutil
from subprocess import check_output

print "Start Script"

# run a exe application here
f = open('C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\test.txt','a')
out = subprocess.check_output(['C:\\Python34\\python', 'C:\\Users\\chenyua\\OPNET_Project\\python_script\\ha.py'])
print >> f, out
f.close()
print "asdf: " + out

# generate new files 
testfile = open("C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\test0.txt", 'w')
testfile = open("C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\test1.txt", 'w')
testfile = open("C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\test2.txt", 'w')

# os.remove() will remove a file.

# os.rmdir() will remove an empty directory.

# shutil.rmtree() 

# remove old files
os.remove("C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\test1.txt")

# copy files from folder to folder
srcfile = 'C:\\Users\\chenyua\\OPNET_Project\\Yu_Chen_SCB\\AD4\\G0\\ARnode_1.txt'
dstdir = 'C:\\Users\\chenyua\\OPNET_Project\\python_script\\Test\\'
shutil.copy(srcfile, dstdir)















