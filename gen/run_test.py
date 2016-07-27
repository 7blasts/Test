# -*- coding: utf-8 -*-

"""    Yet another python based qt-project's tests engine:
- Check file path and arguments, if failed return error code 1         ERR 1
- Parse specified *.pro file use one from installed QT (-q qmake path) ERR 2
- Compile result with one from the installed compilers (-m make path)  ERR 3
- Run compiled with selected QT's and compiler's DLLs                  ERR 4
- Store resulted STDOUT, STDERR and return code to file
- Execute 'post.test.py' file with specified Python (-p python path)
- Collect results of the 'post.test.py' executing, on success return 0
Use --help to print commandline arguments help.
"""

import os
import sys
import platform
import pdb
import traceback
import re

from optparse import OptionParser
from subprocess import Popen, PIPE
from pprint import pprint  # IGNORE: pep-8

# Parse Makefile vars
#
executable_debug_path = ""
executable_release_path = ""
re_DESTDIR_TARGET = re.compile(".*\nDESTDIR_TARGET = (\S*)", re.DOTALL)
def get_DESTDIR_TARGET(buff):
    path = re_DESTDIR_TARGET.match(buff).groups()[0]
    return os.path.normpath(os.path.abspath(path))

qmake_args_debug = " ".join([
    '-spec win32-g++',
    '"CONFIG+=debug"',
    '"CONFIG+=qml_debug"',
])

stored_working_directory = None   # Initial Current Working Directory

parser = OptionParser()

parser.add_option(
    "-f", "--file", dest="pro", metavar="../src/test/test.pro",
    help="path to target QT project file"
)

parser.add_option(
    "-m", "--make", dest="make",
    help="path to c++ 'make' executable and DLLs",
    metavar='"C:/mingw69/bin/mingw64-make.exe"',
    default="C:/Qt/Qt5.6.0/Tools/mingw492_32/bin/mingw32-make.exe")

parser.add_option(
    "-q", "--qmake", dest="qmake",
    metavar='"E:/QT/QTxx/bin/qmake.exe"',
    help="path to 'qmake' executable and DLLs",
    default="C:/Qt/Qt5.6.0/5.6/mingw49_32/bin/qmake.exe")

parser.add_option(
    "-t", "--temp", dest="temp",
    metavar='"../temp/tests"',
    help="path to temporary directory",
    default="../tmp/tests")

parser.add_option(
    "-p", "--python", dest="python", metavar='"C:/Python27/python.exe"',
    help="path to python executable used in tests subprocess",
    default="")  # do not change python executable, use current

(options, args) = parser.parse_args([])


def setup_environment():
    """Call system specific setup_environment, if defined in this module"""
    method_name = "_".join(("setup_environment", platform.system().lower()))
    this_module = sys.modules[__name__]
    if hasattr(this_module, method_name):
        getattr(this_module, method_name)()
    else:
        print "WARNING: Undefined setup_environment method '%s'" % method_name
    # TO-DO: setup_environment_windows
    # TODO: setup_environment_posix


def setup_environment_windows():
    """ Prepare Windows environment (tested on WindowsXP only):
        Prepend qmake.exe folder path to windows environment variable "PATH"
    This make visible for windows QT DLLs needed to the run test application
    Also prepend make.exe folder path to windows environment variable "PATH"
    This make visible for windows compiler's runtime DLLs needed to the run
    compiled test application."""
    qmake_and_qt_dll_path = os.path.split(options.qmake)[0]
    make_and_cpp_dll_path = os.path.split(options.make)[0]
    if 'PATH' not in os.environ:
        os.environ['PATH'] = ""
    if qmake_and_qt_dll_path not in os.environ['PATH']:
        os.environ['PATH'] = ";".join(
            [qmake_and_qt_dll_path, os.environ['PATH']])
    if make_and_cpp_dll_path not in os.environ['PATH']:
        os.environ['PATH'] = ";".join(
            [make_and_cpp_dll_path, os.environ['PATH']])
    # Uncomment next line to print 'PATH' environment variable:
    # print "\n".join(("\t"+repr(i) for i in os.environ['PATH'].split(";")))

def qmake():
    cmd = ' '.join((options.qmake, qmake_args_debug, options.project))
    process = Popen(cmd, stdout=PIPE, stderr=PIPE)
    stdout_data, stderr_data = process.communicate()
    if stdout_data:
        print "QMAKE STDOUT:"
        print stdout_data
    if stderr_data:
        print "QMAKE STDERR:"
        print stderr_data
    if process.returncode:
        sys.stderr.write('QMAKE ERROR %s on test "%s"\n' % (
            process.returncode, os.path.split(options.project)[-1]
        ))
    else:
        sys.stdout.write('QMAKE "%s" SUCCESS\n' % (
            os.path.split(options.project)[-1]
        ))
        global executable_release_path
        executable_release_path = get_DESTDIR_TARGET(
            file("Makefile.Release", "r").read()
        )
        global executable_debug_path
        executable_debug_path = get_DESTDIR_TARGET(
            file("Makefile.Debug", "r").read()
        )
    return process.returncode


def make_debug():
    """Run make we need only make, w/o jobs on stderr or stdout"""
    return_code = Popen(options.make+' Debug').wait()
    if return_code:
        sys.stderr.write('MAKE DEBUG ERROR=%s on "%s"\n' % (
            return_code, os.path.split(options.project)[-1]))
    else:
        sys.stdout.write('MAKE DEBUG "%s" SUCCESS' % (
            os.path.split(options.project)[-1]))
    return return_code


def make_release():
    """Run make we need only make, w/o jobs on stderr or stdout"""
    return_code = Popen(options.make+' Release').wait()
    if return_code:
        sys.stderr.write('ERROR: make Release %s on "%s"\n' % (
            return_code, os.path.split(options.project)[-1]))
    else:
        sys.stdout.write('SUCCESS: make Release "%s"\n' % (
            os.path.split(options.project)[-1]))
    return return_code


def qmake_project(test_path):
    # Run make we need only make, w/o jobs on stderr or stdout
    #
    """
    make_return_code = Popen(options.make).wait()
    print "MAKE RETURN CODE:", make_return_code
    if make_return_code:
        raise BaseException(
           '''MAKE_ERROR=%s path="%s"''' % (
               make_return_code, options.project
           )
        )
    """

    # After success "make" unit test, detect os-wide executable path.
    # Where is our compiled unit-test in file system depends qmake's
    # 'TARGET' variable, and can be very complicated, so we need to
    # parse qmake generated 'Makefile.Debug' or 'Makefile.Release'
    # file and extract real target path and file name.
    global executable_debug_path
    global executable_release_path

    print '''\nEXECUTE "%s"''' % executable_debug_path
    process = Popen(
        executable_debug_path
        , stdout=PIPE
        , stderr=PIPE
        , shell=True
    )
    unit_stdout, unit_stderr = process.communicate()
    if unit_stdout:
        print unit_stdout
    if unit_stderr:
        print "<STDERR>"
        print unit_stderr
    print "RETURN %d\n" % process.returncode

    print '''\nEXECUTE "%s"''' % executable_release_path
    process = Popen(
        executable_release_path
        , stdout=PIPE
        , stderr=PIPE
        , shell=True
    )
    unit_stdout, unit_stderr = process.communicate()
    if unit_stdout:
        print unit_stdout
    if unit_stderr:
        print "<STDERR>"
        print unit_stderr
    print "RETURN %d\n" % process.returncode

def main():
    """ Supported strategy: - Each non-absolute file path are path based
    on sys.argv[0] path. So, current working directory, before check
    path-based commandline arguments, must be changed to sys.argv[0]
    file parent directory."""

    # Check sys.argv[0]. Set/restore working directory
    #
    global stored_working_directory
    if not stored_working_directory:
        stored_working_directory = os.getcwd()
        stored_working_directory = os.path.abspath(stored_working_directory)
    else:
        os.chdir(stored_working_directory)

    # Change working directory to check relative-path-based commandline args
    #
    base_dir = os.path.split(os.path.abspath(sys.argv[0]))[0]
    os.chdir(base_dir)

    # project -f --file, check error: not exists critical option
    #
    if not options.project:
        sys.stderr.write("ERROR: Path to target QT project are not exists\n")
        print parser.format_help()
        return 1
    options.project = os.path.abspath(os.path.normpath(options.project))
    if not os.path.exists(options.project):
        sys.stderr.write("ERROR: File not found '%s'\n" % options.project)
        return 1

    # Python path, optional argument -p --python
    #
    if not options.python:
        options.python = sys.executable
    options.python = os.path.abspath(os.path.normpath(options.python))
    if not os.path.exists(options.python):
        print "ERROR: File not found '%s'\n" % options.python
        return 1

    # Temporary folder, optional argument -t --temp
    #
    if not options.temp:
        options.temp = '../tmp'
    options.temp = os.path.abspath(os.path.normpath(options.temp))
    if not os.path.exists(options.temp):
        os.makedirs(options.temp)

    # Path to compiler's make executable -m --make
    #
    options.make = os.path.abspath(os.path.normpath(options.make))
    if not os.path.exists(options.make):
        sys.stderr.write("ERROR: File not found '%s'\n" % options.make )
        return 1

    # Path to selected QT's qmake executable
    #
    options.qmake = os.path.abspath(os.path.normpath(options.qmake))
    if not os.path.exists(options.qmake):
        print "ERROR: File not found '%s'\n" % options.qmake
        return 1

    # Each used options's path now is an absolute path and each path exists.
    # --------------------------------------------------------

    # Call platform-depended method for setup system environment
    #
    setup_environment()

    # Detect project's file name. Generate test's build folder in temp dir
    #
    build_dir = "build_" + os.path.split(options.project)[-1].split('.pro')[0]
    build_dir = os.path.join(options.temp, build_dir)
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    os.chdir(build_dir)

    # Use selected qmake to build Makefile, Makefile.Debug, Makefile.Release
    #
    qmake_return_code = qmake()
    if qmake_return_code:
        return 2

    # Use selected make to build test's Debug executable
    #
    make_debug_return_code = make_debug()
    if make_debug_return_code:
        return 3

    # Use selected make to build test's Release executable
    #
    make_release_return_code = make_release()
    if make_release_return_code:
        return 3

    # After success "make" unit test, detect os-wide executable path.
    # Where is our compiled unit-test in file system depends qmake's
    # 'TARGET' variable, and can be very complicated, so we need to
    # parse qmake generated 'Makefile.Debug' or 'Makefile.Release'
    # file and extract real target path and file name.
    global executable_debug_path
    print '''\nEXECUTE "%s"''' % executable_debug_path
    process = Popen(
        executable_debug_path
        , stdout=PIPE
        , stderr=PIPE
        , shell=True
    )
    unit_stdout, unit_stderr = process.communicate()
    if unit_stdout:
        print unit_stdout
    if unit_stderr:
        print "<STDERR>"
        print unit_stderr
    print "RETURN %d\n" % process.returncode

    global executable_release_path
    print '''\nEXECUTE "%s"''' % executable_release_path
    process = Popen(
        executable_release_path
        , stdout=PIPE
        , stderr=PIPE
        , shell=True
    )
    unit_stdout, unit_stderr = process.communicate()
    if unit_stdout:
        print unit_stdout
    if unit_stderr:
        print "<STDERR>"
        print unit_stderr
    print "RETURN %d\n" % process.returncode

if __name__ == '__main__':
    (options, args) = parser.parse_args()
    sys.exit(main())
