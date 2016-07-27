<<<<<<< HEAD
import os, sys
from subprocess import Popen, PIPE

print>>sys.stdout, "Hello from %s" % sys.argv[0]

for n, i in enumerate(sys.argv[1:]):
  print "%s:%s" % (n+1, repr(i))

print>>sys.stderr, "Test error in stderr"

print os.environ['path']

print "QMAKE"
p0 = Popen(["qmake", "--version"], shell=True, stdout=PIPE, stderr=PIPE)
print p0.communicate()[0]

print "G++"
p1 = Popen(["g++", "--version"], shell=True, stdout=PIPE, stderr=PIPE)
print p1.communicate()[0]
=======
import os, sys
from subprocess import Popen, PIPE

print>>sys.stdout, "Hello from %s" % sys.argv[0]

for n, i in enumerate(sys.argv[1:]):
  print "%s:%s" % (n+1, repr(i))

print>>sys.stderr, "Test error in stderr"

print os.environ['path']

print "QMAKE"
p0 = Popen(["qmake", "--version"], shell=True, stdout=PIPE, stderr=PIPE)
print p0.communicate()[0]

print "G++"
p1 = Popen(["g++", "--version"], shell=True, stdout=PIPE, stderr=PIPE)
print p1.communicate()[0]
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
