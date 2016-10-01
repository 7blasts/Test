import os
# from PIL import Image, ImageFile
# from PIL import ImageShow
#
# path = "../tmp/sample.png"
# path = os.path.normpath(os.path.abspath(path))
#
# bg1 = Image.new('RGBA', (800, 600), (128, 128, 128, 0))
# bg1.save(path)  #, 'XPM')
# ImageShow.show(bg1)
# bg1.show()
#

def word_count(filename):
    """Count specified words in a text"""
    if os.path.exists(filename):
        if not os.path.isdir(filename):
            with open(filename) as f_obj:
                print(f_obj.read().lower().count('t'))
        else:
            print("is path to folder, not to file '%s'" % filename)
    else:
        print("path not found '%s'" % filename)

#dracula = 'C:\\Users\\HP\\Desktop\\Programming\\Python\\Python Crash Course\\TEXT files\\dracula.txt'
#siddhartha = 'C:\\Users\\HP\\Desktop\\Programming\\Python\\Python Crash Course\\TEXT files\\siddhartha.txt'

# word_count("C:\\public\\INSTALL\\where_is_photoshop_in_windows.py")
# word_count("C:\\public\\INSTALL")
# word_count("C:\\public\\INSTALL\\wh--ere_is_photoshop_in_windows.py")
    import os
    path = "public\\INSTALL\\"
    print("Initial unmodified join return: '%s'" % os.path.join('.', path) )
    native_os_path_join = os.path.join
    def modified_join(*args, **kwargs):
        return native_os_path_join(*args, **kwargs).replace('\\', '/')
    os.path.join = modified_join
    print("Modified join return: '%s'" % os.path.join('.', path) )

