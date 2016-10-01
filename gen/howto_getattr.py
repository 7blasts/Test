import sys

def foo(): sys.stdout.write("foo called\n")
def foo1(): sys.stdout.write("foo1 called\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:            # prevent IndexError
        sys.stdout.write("Use commandline argument: foo or foo1\n")
        sys.exit()                   # Print --help and exit
    name = sys.argv[1]               # Get name (str is)
    m = sys.modules["__main__"]      # Get main module instance obj
    if hasattr(m, name):             # Check attribute exists
        a = getattr(m, name)         # Get attribute by str-name
        if hasattr(a, '__call__'):   # Verify callable is?
            a()                      # Call
        else:
            sys.stderr.write("ERROR: is not some callable '%s'\n" % name)
            sys.exit(1)
    else:
        sys.stderr.write("ERROR: has no attr named '%s'\n" % name)
        sys.exit(1)
