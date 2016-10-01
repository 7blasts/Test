# TODO: find more complete example!

from distutils.core import setup
import py2exe, scipy, matplotlib

setup(
    windows=['main.py'],
    options={
        "py2exe": {
            "packages": ["scipy", "matplotlib"],
            "excludes": [
                        '_gtkagg',
                        '_tkagg',
                        '_agg2',
                        '_cairo',
                        '_cocoaagg',
                        '_fltkagg',
                        '_gtk',
                        '_gtkcairo',
                        'tcl',
                        'matplotlib.numerix.fft',
                        'matplotlib.numerix.linear_algebra',
                        'matplotlib.numerix.random_array'
                        ],
            'dll_excludes': [
                'libgdk-win32-2.0-0.dll',
                'libgobject-2.0-0.dll',
                'libgdk_pixbuf-2.0-0.dll'
            ]
        }},
    data_files=matplotlib.get_py2exe_datafiles(),
)

