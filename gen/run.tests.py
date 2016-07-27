import sys
import run_test as rt

rt.options.make = "C:/Qt/Qt5.6.0/Tools/mingw492_32/bin/mingw32-make.exe"
rt.options.qmake = "C:/Qt/Qt5.6.0/5.6/mingw49_32/bin/qmake.exe"

tests = [
    "../src/xx_test_0000/xx_test_0000.pro",
    "../src/xx_test_0001/xx_test_0001.pro",
]
failed = []
passed = []
for test in tests:
    rt.options.project = test
    if rt.main():
        failed.append(test)
    else:
        passed.append(test)

sys.stdout.write("PASSED:\n")
sys.stdout.write("\n".join(passed)+"\n")

if len(failed):
    sys.stderr.write("FAILED:\n")
    sys.stderr.write("\n".join(failed)+"\n")
    sys.exit(1)

sys.stdout.write("\nALL OK\n")
sys.exit(0)

